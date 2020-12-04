#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/mutex.h> //changed from asm/semaphore.h
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h> //copy_to_user
#include <linux/mm.h>// virt_to_page

 
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("pohuang");
MODULE_DESCRIPTION("A Simple Hello World module");
 

int scull_major = 0;
int scull_minor = 0;
int scull_nr_devs = 2;
int first = 0;
module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
module_param(scull_nr_devs, int, S_IRUGO);

struct scull_dev
{
    struct cdev cdev;
    char *p ;
    int size;
    struct mutex mutex; // semaphore is replaced with mutex after 2.6.16
    /* Other Lock Choices 
     * struct semaphore sem;
     * struct rw_semaphore rw_sem;
     * struct completion c;
     * struct spinlock_t lock;
     * atomic_t v;
     * seqlock_t lock; 
    */
   
};

int scull_open(struct inode *mynode, struct file *myfile)
{
    struct cdev *c_dev = mynode->i_cdev;
    struct scull_dev *pdev = (struct scull_dev*)(c_dev);
    myfile->private_data = pdev;
    printk("scull_open size = %d\n",pdev->size);
    return 0;
}

ssize_t scull_read(struct file *pfile, char __user *pbuff, size_t num, loff_t *ploff)
{
    struct scull_dev *pdev = (struct scull_dev*)(pfile->private_data);
    // Acquire the lock
    // Equivilant to down_interruptible in ldd3 book
    if (mutex_lock_interruptible(&pdev->mutex))
         return -ERESTARTSYS;
    num = *ploff + num < pdev->size ? num : pdev->size - *ploff;
    if (num > 0)
    {
        if (copy_to_user(pbuff, pdev->p + *ploff, num) != 0)
        {
            printk("copy error!\n");
            goto out;
        }
    }
    *ploff += num;
out:
    // Release the lock
    // Equivilant to up in ldd3 book
    mutex_unlock(&pdev->mutex);
    return num;
}

ssize_t scull_write(struct file *pfile, const char __user *pbuff, size_t num, loff_t *ploff){
    struct scull_dev *pdev = (struct scull_dev*)(pfile->private_data);
    printk("write test\n");
    // Acquire the lock
    if (mutex_lock_interruptible(&pdev->mutex))
                 return -ERESTARTSYS;

    /* Different versions of down
     * down(&pdev->mutex);
     * down_interruptible(&pdev->mutex);
     * down_trylock(&pdev->mutex);
     */
    if (pdev->p) ClearPageReserved(virt_to_page(pdev->p));
    free_page(pdev->p);
    pdev->p = (char*)__get_free_page(GFP_KERNEL);
    if (copy_from_user(pdev->p, pbuff, num) != 0)
    {
        printk("copy error!\n");
        goto out;
    }
    pdev->size = num;
    *ploff = num;
    printk("off:%d, value:%x\n", (int)*ploff, (int)*(pdev->p));
out:
    // Release the lock
    mutex_unlock(&pdev->mutex);
    return num;
}

static int scull_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    struct scull_dev *pdev = (struct scull_dev *)(vma->vm_file->private_data);
    struct page *ppage = virt_to_page(pdev->p);
    get_page(ppage);
    vmf->page = ppage;
        return 0;
}


static int scull_map(struct file *pfile, struct vm_area_struct *vma)
{
        struct scull_dev *pdev = (struct scull_dev*)(pfile->private_data);
        struct page* ppage = virt_to_page(pdev->p);
        printk("map\n");
        SetPageReserved(ppage);
        remap_pfn_range(vma, vma->vm_start, (__pa(pdev->p)) >> PAGE_SHIFT, vma->vm_end - vma->vm_start, vma->vm_page_prot);

        return 0;
}

struct file_operations scull_fops = {
        .owner = THIS_MODULE,
        .read = scull_read,
        .open = scull_open,
        .write = scull_write,
        .mmap = scull_map,
};


struct scull_dev *scull_devices;
static int scull_setup_cdev(struct scull_dev *dev, int index)
{
    int err, devno = MKDEV(scull_major, scull_minor + index);
    
    cdev_init(&dev->cdev, &scull_fops);
    err = cdev_add(&dev->cdev, devno, 1);
    if (err)
    {
        printk("cannot add cdev");
        cdev_del(&dev->cdev);
        return err;
    }
    // Equivilant to init_MUTEX in ldd3 book
    mutex_init(&dev->mutex);
    return 0;
}

void scull_cleanup_module(void)
{
    int i;

    if (scull_devices) {
        for (i = 0; i < scull_nr_devs; i++) {
            cdev_del(&scull_devices[i].cdev);
        }
        kfree(scull_devices);
    }

    unregister_chrdev_region(scull_major, scull_nr_devs);


}
 
int __init scull_init(void)
{
    int result, i;
    dev_t dev = 0;
    // Static way
    if (scull_major) {
        dev = MKDEV(scull_major, scull_minor);
        result = register_chrdev_region(dev, scull_nr_devs, "scull");
    // Dynamic way
    } else {
        result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,
                "scull");
        scull_major = MAJOR(dev);
    }
    // Catch Error
    if (result < 0) {
        printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
        return result;
    }
    // Create nr scull devices
    scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);
    if (!scull_devices) {
        result = -ENOMEM;
        goto fail;  
    }
    // Initialize to 0
    memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));

    
    for (i = 0; i < scull_nr_devs; i++)
    {
        result = scull_setup_cdev(&scull_devices[i], i);
        if (result < 0)
        {
            goto fail;
        }
            
    }
    return 0;

    fail:
        scull_cleanup_module();
        return result;
}
 

module_init(scull_init);
module_exit(scull_cleanup_module);

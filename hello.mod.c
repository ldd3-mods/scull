#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x28950ef1, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x15692c87, __VMLINUX_SYMBOL_STR(param_ops_int) },
	{ 0xf0fdf6cb, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x9a025cd5, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0x5f675a65, __VMLINUX_SYMBOL_STR(cdev_add) },
	{ 0xdacd8618, __VMLINUX_SYMBOL_STR(cdev_init) },
	{ 0xd2b09ce5, __VMLINUX_SYMBOL_STR(__kmalloc) },
	{ 0x29537c9e, __VMLINUX_SYMBOL_STR(alloc_chrdev_region) },
	{ 0xd8e484f0, __VMLINUX_SYMBOL_STR(register_chrdev_region) },
	{ 0x7485e15e, __VMLINUX_SYMBOL_STR(unregister_chrdev_region) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0x3fa89e8f, __VMLINUX_SYMBOL_STR(cdev_del) },
	{ 0x71de9b3f, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0xe84cb310, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0xadd2aceb, __VMLINUX_SYMBOL_STR(__get_page_tail) },
	{ 0x6343a556, __VMLINUX_SYMBOL_STR(get_zone_device_page) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0x4ed12f73, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x77e2f33, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x93fca811, __VMLINUX_SYMBOL_STR(__get_free_pages) },
	{ 0x4302d0eb, __VMLINUX_SYMBOL_STR(free_pages) },
	{ 0x97651e6c, __VMLINUX_SYMBOL_STR(vmemmap_base) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0x41dae671, __VMLINUX_SYMBOL_STR(mutex_lock_interruptible) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "3D4B7FE7A21B866F61A22C4");
MODULE_INFO(rhelversion, "7.8");
#ifdef RETPOLINE
	MODULE_INFO(retpoline, "Y");
#endif
#ifdef CONFIG_MPROFILE_KERNEL
	MODULE_INFO(mprofile, "Y");
#endif

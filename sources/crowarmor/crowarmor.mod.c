#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x18\x00\x00\x00\x17\x10\xd2\x35"
	"try_module_get\0\0"
	"\x18\x00\x00\x00\x09\x4b\x54\x0c"
	"class_destroy\0\0\0"
	"\x14\x00\x00\x00\x2a\xc1\x09\x1b"
	"pcpu_hot\0\0\0\0"
	"\x1c\x00\x00\x00\x6e\x64\xf7\xb3"
	"kthread_should_stop\0"
	"\x14\x00\x00\x00\xbb\x6d\xfb\xbd"
	"__fentry__\0\0"
	"\x18\x00\x00\x00\x1d\xe7\x79\xe3"
	"wake_up_process\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x14\x00\x00\x00\xb6\xce\x57\x71"
	"module_put\0\0"
	"\x28\x00\x00\x00\xb3\x1c\xa2\x87"
	"__ubsan_handle_out_of_bounds\0\0\0\0"
	"\x18\x00\x00\x00\xe0\x2f\xeb\x65"
	"device_create\0\0\0"
	"\x18\x00\x00\x00\x87\xb8\x75\x90"
	"class_create\0\0\0\0"
	"\x24\x00\x00\x00\xce\xce\x0e\x67"
	"__x86_indirect_thunk_rbx\0\0\0\0"
	"\x18\x00\x00\x00\x40\x9c\x70\x41"
	"kthread_stop\0\0\0\0"
	"\x1c\x00\x00\x00\xca\x39\x82\x5b"
	"__x86_return_thunk\0\0"
	"\x20\x00\x00\x00\x54\x6a\x0c\xb1"
	"kthread_create_on_node\0\0"
	"\x1c\x00\x00\x00\xa2\xb0\x96\xa4"
	"__register_chrdev\0\0\0"
	"\x18\x00\x00\x00\xa1\x5f\x4d\xda"
	"device_destroy\0\0"
	"\x18\x00\x00\x00\x24\x54\xca\xfc"
	"register_kprobe\0"
	"\x1c\x00\x00\x00\x90\x64\x02\x63"
	"unregister_kprobe\0\0\0"
	"\x24\x00\x00\x00\xa8\xf9\x62\x03"
	"__x86_indirect_thunk_r12\0\0\0\0"
	"\x10\x00\x00\x00\xf9\x82\xa4\xf9"
	"msleep\0\0"
	"\x1c\x00\x00\x00\xc0\xfb\xc3\x6b"
	"__unregister_chrdev\0"
	"\x18\x00\x00\x00\xb5\x28\xc2\x17"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "084A0F1620471820EFD0575");

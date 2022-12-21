#include <linux/init.h> /* Used for module_init/exit() macros */
#include <linux/kernel.h> /* Define KERN_ERR */
#include <linux/module.h> /* Used for all modules */
 
static int __init lkp_init(void)
{
 printk("<1> Hello, World! from the kernel space...\n");
 return 0;
}

static void __exit lkp_cleanup(void)
{
 printk("<1> Goodbye, World! Leaving kernel space...\n");
}
 
 module_init(lkp_init);
 module_exit(lkp_cleanup);
 
 MODULE_LICENSE("GPL v2");
 MODULE_AUTHOR("Jean Zonta");
 MODULE_DESCRIPTION("A Simple Hello World module");
 MODULE_VERSION ("0.1");

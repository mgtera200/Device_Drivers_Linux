/*
 * Author: Eng. Mostafa Tera
 * Date: 29/4/2024
 */


#include "file_operations.h"

/*
 * MODULE_LICENSE: Specifies the license for the module.
 * GPL (General Public License) is used here.
 */
MODULE_LICENSE("GPL");

/*
 * MODULE_AUTHOR: Specifies the author of the module.
 */
MODULE_AUTHOR("MOSTAFA TERA");

/*
 * MODULE_DESCRIPTION: Description of the module.
 */
MODULE_DESCRIPTION("Hello from teraaa");

/*
 * Struct: platform_device
 * -----------------------
 * Structure representing the platform device.
 */
struct platform_device platDevice = {
    .name = "LED_RED_2",
    .id = 1,
};

/*
 * Function: teraINIT
 * -------------------
 * Initialization function for the module.
 */
static int __init teraINIT(void)
{
    printk("LED_RED_2 Device has been inserted successfully\n");
    platform_device_register(&platDevice);
    return 0;
}

/*
 * Function: teraDEINIT
 * ---------------------
 * Deinitialization function for the module.
 */
static void __exit teraDEINIT(void)
{
    printk("LED_RED_2 Device has been removed successfully\n");
    platform_device_unregister(&platDevice);
}

/*
 * Macro: module_init
 * ------------------
 * Marks the entry point for initializing a kernel module.
 */
module_init(teraINIT);

/*
 * Macro: module_exit
 * -------------------
 * Marks the exit point for cleaning up a kernel module.
 */
module_exit(teraDEINIT);

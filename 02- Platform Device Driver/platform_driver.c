/*
 * Author: Eng. Mostafa Tera
 * Date: 29/4/2024
 */

#include "file_operations.h"

/*
 * DRIVER_NAME: Name of the driver module.
 */
#define DRIVER_NAME "teraDriver"

/*
 * DRIVER_CLASS: Name of the driver class.
 */
#define DRIVER_CLASS "tera_class"

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
 * Struct: mydata
 * --------------
 * Structure to hold device-related information.
 */
struct mydata
{
    dev_t my_device_nr;         // Device number
    struct cdev cdev_object;    // Represents a character device
                                // and provides a way to manage and interact with it
    struct file_operations fops; // File operations structure
    struct class *my_class;     // Pointer to the device class
} teraData_st = {
    .fops = {
        .owner = THIS_MODULE,   // Owner of the file operations
        .open = driver_open,    // Function pointer to the open function
        .release = driver_close, // Function pointer to the close function
        .read = driver_read,    // Function pointer to the read function
        .write = driver_write   // Function pointer to the write function
    }
};

/*
 * Static array of platform device IDs.
 */
static struct platform_device_id device_id[] =
    {
        [LED_RED] = {.name = "LED_RED"},
        [LED_RED_2] = {.name = "LED_RED_2"},
};

/*
 * Function: prob_device
 * ---------------------
 * Probe function for the platform driver. Called when a device is detected.
 * 
 * Parameters:
 * - sLED_P: Pointer to the platform device structure representing the detected device.
 * 
 * Returns:
 * - 0 on success, otherwise an error code.
 */
int prob_device(struct platform_device *sLED_P)
{
    /*
     * Print a message indicating the detection of the device.
     */
    printk("%s device_detected", sLED_P->name);

    /*
     * Check the name of the detected device and perform corresponding actions.
     */
    if (strcmp(sLED_P->name, "LED_RED") == 0)
    {
        /*
         * Attempt to request GPIO pin 2 for the LED_RED device.
         */
        if (gpio_request(2, "LED_RED_pin"))
        {
            printk("Cannot allocate GPIO pin 2\n");
        }
        else
        {
            printk("GPIO pin 2 allocated successfully\n");
        }

        /*
         * Set GPIO pin 2 to be an output pin for the LED_RED device.
         */
        if (gpio_direction_output(2, 0))
        {
            printk("Cannot set the GPIO pin 2 to be output\n");
            gpio_free(2);
        }
        else
        {
            printk("GPIO pin 2 set to be output\n");
        }
    }
    else if (strcmp(sLED_P->name, "LED_RED_2") == 0)
    {
        /*
         * Attempt to request GPIO pin 3 for the LED_RED_2 device.
         */
        if (gpio_request(3, "LED_RED_2_pin"))
        {
            printk("Cannot allocate GPIO pin 3\n");
        }

        /*
         * Set GPIO pin 3 to be an output pin for the LED_RED_2 device.
         */
        if (gpio_direction_output(3, 0))
        {
            printk("Cannot set the GPIO pin 3 to be output\n");
            gpio_free(3);
        }
    }
    else if (strcmp(sLED_P->name, "LED_GREEN") == 0)
    {
        /*
         * Attempt to request GPIO pin 4 for the LED_GREEN device.
         */
        if (gpio_request(4, "LED_GREEN_pin"))
        {
            printk("Cannot allocate GPIO pin 4\n");
        }

        /*
         * Set GPIO pin 4 to be an output pin for the LED_GREEN device.
         */
        if (gpio_direction_output(4, 0))
        {
            printk("Cannot set the GPIO pin 4 to be output\n");
            gpio_free(4);
        }
    }

    /*
     * Create a device file for the detected device.
     */
    if (device_create(teraData_st.my_class, NULL, teraData_st.my_device_nr + (sLED_P->id), NULL, sLED_P->name) == NULL)
    {
        printk("Can not create device file!\n");
    }
    else
    {
        printk("Device file Created!\n");
    }

    return 0;
}


/*
 * Function: device_remove
 * ------------------------
 * Function called when removing a platform device.
 * 
 * Parameters:
 * - sLED_P: Pointer to the platform device structure representing the device to be removed.
 * 
 * Returns:
 * - 0 on success, otherwise an error code.
 */
int device_remove(struct platform_device *sLED_P)
{
    /*
     * Check the name of the device and perform corresponding cleanup actions.
     */
    if (strcmp(sLED_P->name, "LED_RED") == 0)
    {
        /*
         * Set GPIO pin 2 to 0 and release it.
         */
        gpio_set_value(2, 0);
        gpio_free(2);
    }
    else if (strcmp(sLED_P->name, "LED_RED_2") == 0)
    {
        /*
         * Set GPIO pin 3 to 0 and release it.
         */
        gpio_set_value(3, 0);
        gpio_free(3);
    }
    else if (strcmp(sLED_P->name, "LED_GREEN") == 0)
    {
        /*
         * Set GPIO pin 4 to 0 and release it.
         */
        gpio_set_value(4, 0);
        gpio_free(4);
    }

    /*
     * Destroy the device file associated with the device.
     */
    device_destroy(teraData_st.my_class, teraData_st.my_device_nr + (sLED_P->id));

    /*
     * Print a message indicating that the device removal function was called.
     */
    printk("device_remove");

    return 0;
}

/*
 * Struct: platform_driver_data
 * ----------------------------
 * Structure holding platform driver data.
 */
struct platform_driver platform_driver_data =
    {
        .probe = prob_device,
        .remove = device_remove,
        .id_table = device_id,
        .driver = {
            .name = "mydriver"
        }
};

/*
 * Function: teraINIT
 * -------------------
 * Initialization function for the module.
 */
static int __init teraINIT(void)
{
    printk("PLatform driver inserted\n");

    // Allocate character device region
    if (alloc_chrdev_region(&teraData_st.my_device_nr, 0, 3, DRIVER_NAME) < 0)
    {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }

    // Initialize the character device
    cdev_init(&teraData_st.cdev_object, &teraData_st.fops);

    // Add the character device to the kernel
    if (cdev_add(&teraData_st.cdev_object, teraData_st.my_device_nr, 3) == -1)
    {
        printk("Adding the device to the kernel failed!\n");
    }

    // Create device class
    if (((teraData_st.my_class = class_create(DRIVER_CLASS)) == NULL))
    {
        printk("Device class can not be created!\n");
        goto ClassError;
    }

    // Register platform driver
    platform_driver_register(&platform_driver_data);
    return 0;

ClassError:
    unregister_chrdev_region(teraData_st.my_device_nr, 1);
    return -1;
}

/*
 * Function: teraDEINIT
 * --------------------
 * Deinitialization function for the kernel module.
 * 
 * This function unregisters the platform driver, destroys the device class,
 * deletes the character device object, unregisters the device numbers, and prints
 * a goodbye message.
 */
static void __exit teraDEINIT(void)
{
    /*
     * Unregister the platform driver.
     */
    platform_driver_unregister(&platform_driver_data);

    /*
     * Destroy the device class.
     */
    class_destroy(teraData_st.my_class);

    /*
     * Delete the character device object.
     */
    cdev_del(&teraData_st.cdev_object);

    /*
     * Unregister the device numbers.
     */
    unregister_chrdev_region(teraData_st.my_device_nr, 1);

    /*
     * Print a goodbye message.
     */
    printk("Goodbye from tera \n");
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

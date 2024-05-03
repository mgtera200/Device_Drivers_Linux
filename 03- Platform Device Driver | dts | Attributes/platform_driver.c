/*
 * Author: Eng. Mostafa Tera
 * Date: 3/5/2024
 */

#include "file_operations.h"

/* Name of the driver module */
#define DRIVER_NAME "teraDriver"

/* Name of the driver class */
#define DRIVER_CLASS "tera_class"

/* Specifies the license for the module (GPL - General Public License) */
MODULE_LICENSE("GPL");

/* Specifies the author of the module */
MODULE_AUTHOR("MOSTAFA TERA");

/* Description of the module */
MODULE_DESCRIPTION("Hello from teraaa");

/* Structure to hold device-related information */
struct mydata
{
    dev_t my_device_nr;          /* Device number */
    struct cdev cdev_object;     /* Represents a character device and provides a way to manage and interact with it */
    struct file_operations fops; /* File operations structure */
    struct class *my_class;      /* Pointer to the device class */
} teraData_st = {
    .fops = {
        .owner = THIS_MODULE,    /* Owner of the file operations */
        .open = driver_open,     /* Open function for the device */
        .release = driver_close, /* Close function for the device */
        .read = driver_read,     /* Read function for the device */
        .write = driver_write    /* Write function for the device */
    }};

// Buffer size and permission arrays for LED nodes
int buff_size[2] = {0};
int perm[2] = {0};
int direction_pin2 = 1;
int direction_pin3 = 1;
char *global_label;

// Device IDs for LED nodes defined in Device Tree Source (DTS)
const struct of_device_id platDeviceIdDTS[4] = {
    {.compatible = "tera,led1"},
    {.compatible = "tera,led2"},
    {}
};

/*
 * Function: teraShow1
 * --------------------
 * Function to read the value of LED nodes.
 *
 * Parameters:
 * - dev: Pointer to the device structure.
 * - attr: Pointer to the device attribute structure.
 * - buf: Buffer to store the read value.
 *
 * Returns:
 * - Length of the read value.
 */
ssize_t teraShow1(struct device *dev, struct device_attribute *attr, char *buf)
{
    size_t len; // Variable to store length of read value
    char *label; // Variable to store device label

    // Restore the data saved earlier in probe function to use it
    label = dev_get_drvdata(dev); // Retrieve label from driver data

    // Check the label and set the value accordingly
    if (strcmp(label, "redled_1") == 0) // Check if label is "redled_1"
    {
        if (direction_pin2 == 1) // Check if pin direction is output
        {
            strcpy(buf, "1"); // Set buffer to "1"
            len = strlen("1"); // Update length
        }
        else if (direction_pin2 == 0) // Check if pin direction is input
        {
            strcpy(buf, "0"); // Set buffer to "0"
            len = strlen("0"); // Update length
        }
    }
    else if (strcmp(label, "redled_2") == 0) // Check if label is "redled_2"
    {
        if (direction_pin3 == 1) // Check if pin direction is output
        {
            strcpy(buf, "1"); // Set buffer to "1"
            len = strlen("1"); // Update length
        }
        else if (direction_pin3 == 0) // Check if pin direction is input
        {
            strcpy(buf, "0"); // Set buffer to "0"
            len = strlen("0"); // Update length
        }
    }
    return len; // Return length of read value
}

/*
 * Function: teraStore1
 * ---------------------
 * Function to store the direction attribute of LED nodes.
 *
 * Parameters:
 * - dev: Pointer to the device structure.
 * - attr: Pointer to the device attribute structure.
 * - buf: Buffer containing the input string.
 * - count: Number of bytes in the buffer.
 *
 * Returns:
 * - Number of bytes written.
 */
ssize_t teraStore1(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
    // Define constants for string representations of directions
    const char *direction_output = "output"; // Define string constant for "output"
    const char *direction_input = "input";   // Define string constant for "input"
    char *label;                              // Variable to store device label

    // Restore the data saved earlier in the probe function to use it
    label = dev_get_drvdata(dev); // Retrieve label from driver data

    // Check the label and set the direction accordingly
    if (strcmp(label, "redled_1") == 0) // Check if label is "redled_1"
    {
        // Check if the input string matches "output"
        if (strncmp(buf, direction_output, strlen(direction_output)) == 0)
        {
            int current_gpio_value = gpio_get_value(2); // Get current GPIO value
            gpio_direction_output(2, current_gpio_value); // Set GPIO pin direction as output
            direction_pin2 = 1; // Update direction flag
            printk("gpio direction is set to output for redled_1\n"); // Print message
        }
        // Check if the input string matches "input"
        else if (strncmp(buf, direction_input, strlen(direction_input)) == 0)
        {
            gpio_direction_input(2); // Set GPIO pin direction as input
            direction_pin2 = 0; // Update direction flag
            printk("gpio direction is set to input for redled_1\n"); // Print message
        }
        else
        {
            return -EINVAL; // Return error if input is neither "output" nor "input"
        }
    }
    else if (strcmp(label, "redled_2") == 0) // Check if label is "redled_2"
    {
        // Check if the input string matches "output"
        if (strncmp(buf, direction_output, strlen(direction_output)) == 0)
        {
            int current_gpio_value = gpio_get_value(3); // Get current GPIO value
            gpio_direction_output(3, current_gpio_value); // Set GPIO pin direction as output
            direction_pin3 = 1; // Update direction flag
            printk("gpio direction is set to output for redled_2\n"); // Print message
        }
        // Check if the input string matches "input"
        else if (strncmp(buf, direction_input, strlen(direction_input)) == 0)
        {
            gpio_direction_input(3); // Set GPIO pin direction as input
            direction_pin3 = 0; // Update direction flag
            printk("gpio direction is set to input for redled_2\n"); // Print message
        }
        else
        {
            return -EINVAL; // Return error if input is neither "output" nor "input"
        }
    }
    return count; // Return the number of bytes written
}

// Function to show the value attribute of LED nodes
ssize_t teraShow2(struct device *dev, struct device_attribute *attr, char *buf)
{
    char *label;
    // Restore the data i saved earlier in probe function to use it
    label = dev_get_drvdata(dev);
    size_t len;
    if (strcmp(label, "redled_1") == 0)
    {
        unsigned int gpio = 2; // GPIO pin number
        int pin_value;

        // Get the current value of the GPIO pin
        pin_value = gpio_get_value(gpio);
        if (pin_value < 0)
        {
            return pin_value; // Return error code
        }

        // Convert pin value to string format
        const char *pin_value_str = (pin_value == 0) ? "0" : "1";

        // Copy pin value string to user buffer
        len = strlen(pin_value_str);
        if (len >= PAGE_SIZE)
        {
            return -ENOSPC; // Buffer overflow
        }
        strcpy(buf, pin_value_str);
    }
    else if (strcmp(label, "redled_2") == 0)
    {
        unsigned int gpio = 3; // GPIO pin number
        int pin_value;

        // Get the current value of the GPIO pin
        pin_value = gpio_get_value(gpio);
        if (pin_value < 0)
        {
            return pin_value; // Return error code
        }

        // Convert pin value to string format
        const char *pin_value_str = (pin_value == 0) ? "0" : "1";

        // Copy pin value string to user buffer
        len = strlen(pin_value_str);
        if (len >= PAGE_SIZE)
        {
            return -ENOSPC; // Buffer overflow
        }
        strcpy(buf, pin_value_str);
    }
    return len;
}

// Attributes for LED nodes
struct device_attribute myDevsAttr[2] =
    {
        [0] = {
            .attr = {
                .name = "direction",
                .mode = S_IRUSR | S_IWUSR,
            },
            .show = teraShow1,
            .store = teraStore1},
        [1] = {.attr = {.name = "value", .mode = S_IRUSR}, .show = teraShow2, .store = NULL}};

// Probe function for the platform driver. Called when a device is detected.
int prob_device(struct platform_device *sLED_P)
{
    struct device *dev = &sLED_P->dev;
    int ret, led_value, gpio_pin;
    const char *label;

    enum leds
    {
        redled_1,
        redled_2
    } node_status;

    printk("The probe function is called\n");

    // Check for required device properties
    if (!device_property_present(dev, "label"))
    {
        printk("Error, Device property 'label' not found\n");
        return -1;
    }
    if (!device_property_present(dev, "led_value"))
    {
        printk("Error, Device property 'led_value' not found\n");
        return -1;
    }
    if (!device_property_present(dev, "gpio_pin"))
    {
        printk("Error, Device property 'gpio_pin' not found\n");
        return -1;
    }
    if (!device_property_present(dev, "buff_size"))
    {
        printk("Error, Device property 'buff_size' not found\n");
        return -1;
    }
    if (!device_property_present(dev, "perm"))
    {
        printk("Error, Device property 'perm' not found\n");
        return -1;
    }

    // Read device properties
    ret = device_property_read_string(dev, "label", &label);
    if (ret)
    {
        printk("Error, couldn't read 'label'\n");
        return -1;
    }
    // Print the label of the device
    printk("label is %s\n", label);
    // Set a global_label pointer to the label string
    global_label = (char *)label;
    // Store global_label in dev structure associated to the device called prob function
    dev_set_drvdata(dev, global_label);

    // Determine LED node status based on label
    if (strcmp(label, "redled_1") == 0)
    {
        node_status = redled_1;
    }
    else if (strcmp(label, "redled_2") == 0)
    {
        node_status = redled_2;
    }

    // Read LED properties
    ret = device_property_read_u32(dev, "led_value", &led_value);
    if (ret)
    {
        printk("Error, couldn't read 'led_value' for %s\n", label);
        return -1;
    }
    printk("led_value is %d\n", led_value);

    ret = device_property_read_u32(dev, "gpio_pin", &gpio_pin);
    if (ret)
    {
        printk("Error, couldn't read 'gpio_pin' for %s\n", label);
        return -1;
    }
    printk("gpio_pin is %d\n", gpio_pin);

    // Read additional properties
    ret = device_property_read_u32(dev, "buff_size", &buff_size[node_status]);
    if (ret)
    {
        printk("Error, couldn't read 'buff_size' for %s\n", label);
        return -1;
    }
    printk("buff_size is %d\n", buff_size[node_status]);

    ret = device_property_read_u32(dev, "perm", &perm[node_status]);
    if (ret)
    {
        printk("Error, couldn't read 'perm'\n");
        return -1;
    }
    printk("perm is %d\n", perm[node_status]);

    // Request and configure GPIO pin
    if (gpio_request(gpio_pin, "LED_pin"))
    {
        printk("Cannot allocate GPIO pin %d\n", gpio_pin);
    }
    else
    {
        printk("GPIO pin %d allocated successfully\n", gpio_pin);
    }
    if (gpio_direction_output(gpio_pin, led_value))
    {
        printk("Cannot set the GPIO pin %d to be output\n", gpio_pin);
        gpio_free(gpio_pin);
    }
    else
    {
        printk("GPIO pin %d set to be output\n", gpio_pin);
    }

    // Create device file for the detected device
    if (device_create(teraData_st.my_class, NULL, teraData_st.my_device_nr + (node_status), NULL, label) == NULL)
    {
        printk("Can not create device file for %s!\n", label);
    }
    else
    {
        printk("Device file Created for %s!\n", label);
    }

    // Create attributes for the device
    printk("Creating the attributes for %s \n", label);
    int i = 0;
    for (i = 0; i < 2; i++)
    {
        int retval = device_create_file(dev, &(myDevsAttr[i]));
        if (retval)
        {
            printk("Failed to create sysfs attribute file\n");
            return retval;
        }
    }
    printk("Done Creating the attributes for %s \n", label);
    return 0; // Return success
}

// Function called when removing a platform device
int device_remove(struct platform_device *sLED_P)
{
    // Reset and free GPIO pins
    gpio_set_value(2, 0);
    gpio_free(2);
    gpio_set_value(3, 0);
    gpio_free(3);

    // Destroy device files associated with the device
    int i = 0;
    for (i = 0; i < 2; i++)
    {
        device_destroy(teraData_st.my_class, teraData_st.my_device_nr + i);
    }

    printk("device_remove"); // Print a message indicating device removal
    return 0;                // Return success
}

// Structure holding platform driver data
struct platform_driver platform_driver_data =
    {
        .probe = prob_device,
        .remove = device_remove,
        .driver = {
            .name = "mydriver",
            .of_match_table = platDeviceIdDTS}};

// Initialization function for the module
static int __init teraINIT(void)
{
    printk("PLatform driver inserted\n");

    // Allocate character device region
    if (alloc_chrdev_region(&teraData_st.my_device_nr, 0, 2, DRIVER_NAME) < 0)
    {
        printk("Device Nr. could not be allocated!\n");
        return -1;
    }

    // Initialize the character device
    cdev_init(&teraData_st.cdev_object, &teraData_st.fops);

    // Add the character device to the kernel
    if (cdev_add(&teraData_st.cdev_object, teraData_st.my_device_nr, 2) == -1)
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

// Deinitialization function for the kernel module
static void __exit teraDEINIT(void)
{
    // Unregister the platform driver
    platform_driver_unregister(&platform_driver_data);

    // Destroy the device class
    class_destroy(teraData_st.my_class);

    /* Delete the character device object */
    cdev_del(&teraData_st.cdev_object);

     /* Unregister the device numbers */
    unregister_chrdev_region(teraData_st.my_device_nr, 1);

    /* Print a goodbye message */
    printk("Goodbye from tera \n");
}

/* Marks the entry point for initializing a kernel module */
module_init(teraINIT);

/* Marks the exit point for cleaning up a kernel module */
module_exit(teraDEINIT);


/*
 * Author: Eng. Mostafa Tera
 * Date: 29/4/2024
 */

#include "file_operations.h"

static char RED_1[3] = {0};
static char RED_2[3] = {0};
static char GREEN[3] = {0};

int global_minor;

/*
 * Enum: devices
 * -------------
 * Enumerates the devices.
 */
enum devices
{
    RED_1_e = 2,
    RED_2_e,
    GREEN_e
} status;

/*
 * Function: driver_open
 * ---------------------
 * Called when the device file is opened.
 * 
 * Parameters:
 * - device_file: Pointer to the inode structure representing the device file.
 * - instance: Pointer to the file structure representing the opened file instance.
 * 
 * Returns:
 * - 0 on success, otherwise an error code.
 */
int driver_open(struct inode *device_file, struct file *instance)
{
    /*
     * Extract the major and minor numbers from the device identifier.
     */
    dev_t dev_id = device_file->i_rdev;
    int major = MAJOR(dev_id);
    int minor = MINOR(dev_id);

    /*
     * Store the minor number in a global variable.
     */
    global_minor = minor;

    /*
     * Associate the global minor number with the file instance.
     */
    instance->private_data = &global_minor;

    /*
     * Print a message indicating the device file is opened.
     */
    printk("Opened device with major number %d and minor number %d\n", major, minor);

    return 0;
}


/*
 * Function: driver_close
 * ----------------------
 * Called when the device file is closed.
 */
int driver_close(struct inode *device_file, struct file *instance)
{
    printk("close FUNCTION was called!\n");

    return 0;
}

/*+
 * Function: driver_write
 * ----------------------
 * Called when data is written to a device file.
 * 
 * Parameters:
 * - File: Pointer to the file structure representing the device file.
 * - user_buffer: Pointer to the buffer containing the data to be written.
 * - count: Number of bytes to write.
 * - offs: Pointer to the current file position.
 * 
 * Returns:
 * - Number of bytes successfully written, or a negative error code on failure.
 */
ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
    printk("Write function is entered\n");
    char (*value)[3] = NULL;
    int not_copied;
    
    /*
     * Determine the device based on the minor number associated with the file.
     */
    if (*(int *)File->private_data == 0)
    {
        printk("The minor who called write is minor 0\n");
        value = &RED_1;
        status = RED_1_e;
    }
    else if (*(int *)File->private_data == 1)
    {
        printk("The minor who called write is minor 1\n");
        value = &RED_2;
        status = RED_2_e;
    }
    else if (*(int *)File->private_data == 2)
    {
        printk("The minor who called write is minor 2\n");
        value = &GREEN;
        status = GREEN_e;
    }

    /*
     * Copy data from user space to kernel space.
     */
    not_copied = copy_from_user(value, user_buffer, sizeof(RED_1));

    /*
     * Process the data and perform corresponding actions.
     */
    switch ((*value)[0])
    {
    case '0':
        gpio_set_value(status, 0);
        printk("gpio clear is done\n");
        break;
    case '1':
        gpio_set_value(status, 1);
        printk("gpio set is done\n");
        break;
    default:
        printk("gpio Invalid input\n");
        break;
    }

    /*
     * Adjust the count based on the bytes successfully written.
     */
    count = count - not_copied;
    return count;
}


/*
 * Function: driver_read
 * ---------------------
 * Called when data is read from a device file.
 */
ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
    printk("read function is called but this pin is just output\n");
    return -ENOSYS;
}

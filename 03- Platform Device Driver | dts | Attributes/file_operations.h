/*
 * Author: Eng. Mostafa Tera
 * Date: 29/4/2024
 */

#ifndef FILE_OPERATION
#define FILE_OPERATION

#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/gpio.h>
#include <linux/string.h>
#include <linux/of.h>
#include <linux/gpio/consumer.h>
#include <linux/property.h>

/*
 * Enum: devices_name
 * -------------------
 * Enumerates the names of devices.
 */
enum devices_name
{
    LED_RED,
    LED_RED_2,
    LED_GREEN
};

/*
 * Function: driver_open
 * ---------------------
 * Called when a device file is opened.
 */
int driver_open(struct inode *device_file, struct file *instance);

/*
 * Function: driver_close
 * ----------------------
 * Called when a device file is closed.
 */
int driver_close(struct inode *device_file, struct file *instance);

/*
 * Function: driver_write
 * ----------------------
 * Called when data is written to a device file.
 */
ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs);

/*
 * Function: driver_read
 * ---------------------
 * Called when data is read from a device file.
 */
ssize_t driver_read(struct file *File,char *user_buffer, size_t count, loff_t *offs);

#endif // !FILE_OPERATION

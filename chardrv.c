/*
 * 1)    Write a Linux Char Driver for String convert into Upper Case 
 *       to Lower case or Lower Case to Upper Case. 
 *       Also Make User Application And Test It.
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/errno.h>

/* Device Initializers - Global Entity */
static struct cdev c_dev;
static struct class *cl = NULL;
static dev_t dev;

char internal_buffer[256] = {0};

#include "chardrv_ioctls.h"

/* -----------------------------------
 * Helper Functions
 * -----------------------------------*/

static int strtolower(void)
{
	int i = 0;
	int len = 0;
	for (i = 0; internal_buffer[i] != '\0'; i++);
	
	len = i;

	for (i = 0; i < len; i++)
	{
		if (internal_buffer[i] >= 'A' && internal_buffer[i] <= 'Z')
			internal_buffer[i] = internal_buffer[i] + 32;
	}
	return len;
}

static int strtoupper(void)
{
	int i = 0;
	int len = 0;
	for (i = 0; internal_buffer[i] != '\0'; i++);
	len = i;

	for (i = 0; i < len; i++)
	{
		if (internal_buffer[i] >= 'a' && internal_buffer[i] <= 'z')
			internal_buffer[i] = internal_buffer[i] - 32;
	}

	return len;
}

static int strtocamel(void)
{
	int i = 0;
	int len = 0;
	for (i = 0; internal_buffer[i] != '\0'; i++);
	len = i;
	return len;
}




static int Dev_Open(struct inode *i, struct file *f)
{
	printk(KERN_INFO"Device Opened!!!\n");
	return 0;
}
static int Dev_Close(struct inode *i, struct file *f)
{
	printk(KERN_INFO"Device Closed!!!\n");
	return 0;
}
static ssize_t Dev_Read(struct file *f, char *buffer, size_t length, loff_t *offset)
{
	ssize_t ret = 0;
	printk(KERN_INFO"Reading from Device...");
	ret = copy_to_user(buffer, internal_buffer, length);
	if (ret)
		return ret;
	ret = length;
	printk(KERN_INFO"Completed!!!\n");
	return ret;
}
static ssize_t Dev_Write(struct file *f, const char *buffer, size_t length, loff_t *offset)
{
	ssize_t ret = 0;
	printk(KERN_INFO"Writing to Device...");
	ret = copy_from_user(internal_buffer, buffer, length);
	if (ret)
		return ret;
	ret = length;

	printk(KERN_INFO"Completed!!!\n");
	return ret;
}
static long Dev_Ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	long ret = 0;
	printk(KERN_INFO"Ioctl (%d) is received...\n", cmd);
	switch(cmd)
	{
		case IOCTL_STR_TO_LOWER: ret = strtolower(); break;
		case IOCTL_STR_TO_UPPER: ret = strtoupper(); break;
		case IOCTL_STR_TO_CAMEL: ret = strtocamel(); break;
		default: printk(KERN_INFO"default case for Ioctl\n");
			 ret = -1;
			 break;
	}
	printk(KERN_INFO"Completed!!!\n");
	return ret;
}
static struct file_operations query_fops = {
	.owner = THIS_MODULE,
	.open = Dev_Open,
	.release = Dev_Close,
	.read = Dev_Read,
	.write = Dev_Write,
	.unlocked_ioctl = Dev_Ioctl
};
int Dev_Init(void)
{
	int ret = 0;
	struct device *dev_ret = NULL;

	printk(KERN_INFO"Initialising Kernel Module...");

	ret = alloc_chrdev_region(&dev, 0 /*FIRST_MINOR*/, 1 /*MINOR_CNT*/, "chardrv");
	if (ret < 0)
	{
		return ret;
	}

	cdev_init(&c_dev, &query_fops);

	ret = cdev_add(&c_dev, dev, 1 /*MINOR_CNT*/);
	if (ret < 0)
	{
		unregister_chrdev_region(dev, 1/*MINOR_CNT*/);
		return ret;
	}

	cl = class_create(THIS_MODULE, "char");
	if (IS_ERR(cl))
	{
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, 1 /*MINOR_CNT*/);
		return PTR_ERR(cl);
	}

	dev_ret = device_create(cl, NULL, dev, NULL, "chardrv");
	if (IS_ERR(dev_ret))
	{
		class_destroy(cl);
		cdev_del(&c_dev);
		unregister_chrdev_region(dev, 1 /*MINOR_CNT*/);
		return PTR_ERR(dev_ret);
	}

	printk(KERN_INFO"Completed!!!\n");

	return 0;
}
void Dev_Exit(void)
{
	printk(KERN_INFO"Deinitialising Kernel Module...");
	device_destroy(cl, dev);
	class_destroy(cl);
	cdev_del(&c_dev);
	unregister_chrdev_region(dev, 1 /*MINOR_CNT*/);
	printk(KERN_INFO"Completed!!!\n");
}

module_init(Dev_Init);
module_exit(Dev_Exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jinay Patel");
MODULE_DESCRIPTION("Linux Kernel Char Driver for String Convert (Lower to Upper or vice versa)");

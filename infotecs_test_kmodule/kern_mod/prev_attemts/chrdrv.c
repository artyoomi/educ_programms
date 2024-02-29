#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
MODULE_LICENSE("GPL");
static char *kbuf;
static dev_t dev;
static unsigned int count = 1;
static int major = 700, minor = 0;
static struct cdev *cdev;

#define CHRDEV_NAME "chrdev"
#define KBUF_SIZE (size_t) ((10) * PAGE_SIZE)

static int chrdev_open(struct inode *inode, struct file *file)
{
	pr_info("+++ opening device %s\n", CHRDEV_NAME);
	return 0;
}

static int chrdev_release(struct inode *inode, struct file *file)
{
	pr_info("--- release device %s\n", CHRDEV_NAME);
	return 0;
}

static ssize_t chrdev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = lbuf - copy_to_user(buf, kbuf + *ppos, lbuf);
	*ppos += nbytes;
	pr_info("+++ read device %s nbytes = %d ppos = %d\n", CHRDEV_NAME, nbytes, (int)*ppos);
	return nbytes;
}

static ssize_t chrdev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = lbuf - copy_from_user(kbuf + *ppos, buf, lbuf);
	*ppos += nbytes;

	pr_info("+++ write device %s nbytes = %d ppos = %d\n", CHRDEV_NAME, nbytes, (int)*ppos);
	return nbytes;
}

static const struct file_operations cdev_fops =
{
	.owner = THIS_MODULE,
	.read = chrdev_read,
	.write = chrdev_write,
	.open = chrdev_open,
	.release = chrdev_release
};

static int __init chrdev_init(void)
{
	pr_info("+++ starting chrdev\n");
	kbuf = kmalloc(KBUF_SIZE, GFP_KERNEL);

	dev = MKDEV(major, minor);
	register_chrdev_region(dev, count, CHRDEV_NAME);
	cdev = cdev_alloc();

	cdev_init(cdev, &cdev_fops);
	cdev_add(cdev, dev, count);

	return 0;
}

static void __exit chrdev_cleanup(void)
{
	pr_info("+++ cleanup chrdev\n");
}

module_init(chrdev_init);
module_exit(chrdev_cleanup);

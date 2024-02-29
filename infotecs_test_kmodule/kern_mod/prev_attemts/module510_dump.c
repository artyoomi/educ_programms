#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_NAME "module510"
#define BUF_PATH "../config"
#define PROCFS_MAX_SIZE 1024

static struct proc_dir_entry *proc_file;

static char procfs_buffer[PROCFS_MAX_SIZE];
static size_t procfs_buffer_size = 0;

/*static ssize_t procfile_read(struct file *filePointer, char __user *buffer,
			     size_t buffer_length, loff_t *offset)
{
	char msg[] = "Hello from kernel module\n";
	int len = sizeof(msg);
	ssize_t ret = len;

	if (*offset >= len || copy_to_user(buffer, msg, len)) {
		pr_info("copy_to_user failed\n");
		ret = 0;
	} else {
		pr_info("procfile read %s\n", filePointer->f_path.dentry->d_name.name);
		*offset += len;
	}
	pr_info("msg - [%s]\n", msg);

	return ret;
}*/

static ssize_t procfile_write(struct file *file, const char __user *buff, 
                              size_t len, loff_t *off) 
{ 
    procfs_buffer_size = len; 
    if (procfs_buffer_size > PROCFS_MAX_SIZE) 
        procfs_buffer_size = PROCFS_MAX_SIZE; 
 
    if (copy_from_user(procfs_buffer, buff, procfs_buffer_size)) 
        return -EFAULT; 
 
    procfs_buffer[procfs_buffer_size & (PROCFS_MAX_SIZE - 1)] = '\0'; 
    pr_info("procfile write %s\n", procfs_buffer); 
 
    return procfs_buffer_size; 
}

static int procfile_show(struct seq_file *m, void *v)
{
	pr_info("proc file show\n");
	while(1)
		seq_printf(m,"Hello from kernel module\n");
	return 0;
}

static int procfile_open(struct inode *inode, struct file *file)
{
	pr_info("open proc file\n");
	single_open(file, procfile_show, NULL);
	return 0;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
//	.proc_read = procfile_read,
	.proc_write = procfile_write,
	.proc_open = procfile_open
};
#else
static const struct file_operations proc_file_fops = {
//	.read = procfile_read,
	.write = procfile_write,
	.open = procfile_open
};
#endif

static int __init lkm_init(void)
{
	pr_info("module init\n");
	
	proc_file = proc_create(PROCFS_NAME, 0644, NULL, &proc_file_fops);
	if (NULL == proc_file) {
		proc_remove(proc_file);
		pr_alert("ERROR: Could noto initialize /proc/%s\n", PROCFS_NAME);
		return -ENOMEM;
	}

	pr_info("/proc/%s created\n", PROCFS_NAME);
	return 0;
}
static void __exit lkm_exit(void)
{
	proc_remove(proc_file);
	pr_info("module_exit\n");
}

module_init(lkm_init);
module_exit(lkm_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Artyom Ivanov");
MODULE_DESCRIPTION("Kernel module for linux version 5.10.");
MODULE_VERSION("0.01");

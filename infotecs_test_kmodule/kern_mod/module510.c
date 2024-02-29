#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

#define NAME_SIZE 	    255
#define MSG 		    "Hello from kernel module"
#define PROCFS_MAX_SIZE     1024

/* получаем путь к файлу, куда необходимо записывать строки */
static char name[NAME_SIZE + 1] = "";
module_param_string(name, name, NAME_SIZE, 0000);

/* получаем период вывода строк */
static int per = 0;
module_param(per, int, 0);

static char msg[] = MSG;

/* хранит информацию о /proc файле */
static struct proc_dir_entry *proc_file;

static int proc_show(struct seq_file *m, void *v)
{
	int i = 0;
	while(i < 6) {
		unsigned long j1 = jiffies + per * HZ;
		seq_printf(m, "Hello from kernel module\n");
		pr_info("write in %s number %d\n", msg, i);
		while (time_before(jiffies, j1))
			cpu_relax();
		i++;
	}
	return 0;
}
static ssize_t proc_write(struct file *filp, const char *buf, size_t count, loff_t *offp){
	if (!copy_from_user(msg, buf, count))
	{
		pr_info("ERROR: Failed to copy str from user\n");
	}
    	return count;
}
static int proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_show, NULL);
}

static const struct proc_ops fops = {
	.proc_open = proc_open,
	.proc_read = seq_read,
	.proc_write = proc_write,
	.proc_lseek = seq_lseek,
	.proc_release = single_release
};

static int __init mod_init(void)
{
	pr_info("module510 init\n");
	pr_info("name readed: [%s]\nperiod readed: [%d]\n", name, per);

	proc_file = proc_create(name, 0644, NULL, &fops);
	if (proc_file == NULL) {
		proc_remove(proc_file);
		pr_alert("Error: Could not initialize /proc/%s\n", name);
		return -ENOMEM;
	}
	pr_info("/proc/%s created\n", name);
	return 0;
}
static void __exit mod_exit(void)
{
	pr_info("module510 exit\n");
	proc_remove(proc_file);
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

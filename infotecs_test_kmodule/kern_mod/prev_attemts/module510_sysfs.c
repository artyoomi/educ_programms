#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/jiffies.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#define NAME_SIZE 	    255
#define MSG 		    "Hello from kernel module"
#define PROCFS_MAX_SIZE     1024

/* получаем путь к файлу, куда необходимо записывать строки */
static char name[NAME_SIZE + 1] = "";
module_param_string(name, name, NAME_SIZE, 0000);

/* получаем период вывода строк */
static int per = 0;
module_param(per, int, 0);

static struct kobject *kobj;

static ssize_t sysfs_show(struct kobject *kobj,
			  struct kobj_attribute *attr,
			  char *buffer)
{
	return sprintf(buffer, "YES ITS ME!!! %s\n", MSG);
}

static struct kobj_attribute attribute = __ATTR_RO(sysfs);

static int __init mod_init(void)
{
	int ret;
	pr_info("module510 init\n");
	pr_info("name readed: [%s]\nperiod readed: [%d]\n", name, per);

	kobj = kobject_create_and_add(name, kernel_kobj);
	if (!kobj) {
		return -ENOMEM;
	}

	ret = sysfs_create_file(kobj, &attribute.attr);
	if (ret) {
		kobject_put(kobj);
		return ret;
	}
	pr_info("Module loaded!\n");
	return 0;
}
static void __exit mod_exit(void)
{
	kobject_put(kobj);
	pr_info("module510 exit\n");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");

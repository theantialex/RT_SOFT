#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <stddef.h>

#define SUCCESS 0
#define DEV_NAME "chr_dev"

static int Major;            
static int Device_Open = 0;


static int device_open(struct inode *inode, struct file *file){
    if (Device_Open) return -EBUSY;
    Device_Open++;
    return SUCCESS;
}

static int device_close(struct inode *inode, struct file *file){
   Device_Open--;
   return 0;
}

static unsigned int val = 0;
static ssize_t device_read(struct file *filp, char* buffer, size_t length, loff_t * offset) {
	copy_to_user(buffer, &val, sizeof(unsigned int));
    val++;
    return sizeof(unsigned int);
}

static struct file_operations fops = {
  .read = device_read,
  .open = device_open,
  .release = device_close
};

int init_drv(void) {
    Major = register_chrdev(0, DEV_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEV_NAME, Major);
    return 0;
}

void cleanup_drv(void){
    unregister_chrdev(Major, DEV_NAME);
    printk(KERN_ALERT "Сleanup_module OK \n");
}

module_init(init_drv);
module_exit(cleanup_drv);
MODULE_LICENSE("GPL");

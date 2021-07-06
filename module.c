#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <stddef.h>

#define SUCCESS 0
#define DEV_NAME "chr_dev"

static int Major;            
static int Device_Open = 0;

static int device_open( struct inode *, struct file * );
static int device_release( struct inode *, struct file * );
static ssize_t device_read( struct file *, char *, size_t, loff_t * );


static int device_open(struct inode *inode, struct file *file){
    if (Device_Open) return -EBUSY;
    Device_Open++;
    //MOD_INC_USE_COUNT;

    return SUCCESS;
}

static int device_close(struct inode *inode, struct file *file){
   Device_Open--;
  // MOD_DEC_USE_COUNT;

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

static int __init init_drv() {
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major < 0) {
        printk(KERN_ALERT "Registering char device failed with %d\n", Major);
        return Major;
    }
    printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
    printk(KERN_INFO "the driver, create a dev file with\n");
    printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major)
    return 0;
}

static void __exit cleanup_drv(){
    unregister_chrdev(Major, DEVICE_NAME);
    printk(KERN_ALERT "Сleanup_module OK \n");
}

module_init(init_drv);
module_exit(cleanup_drv);
MODULE_LICENSE("GPL");

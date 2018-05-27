/* A simple character driver program to pass some data from user space and storing that on kernel circular buffer and reading back it on user space */

#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/vmalloc.h>
#include<linux/fs.h>
#include<linux/major.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#include<linux/slab.h>
#include<linux/device.h>
#include<linux/types.h>
#include<linux/kdev_t.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/circ_buf.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("C-DAC");
#define MAX_DEVICE 2
#define KERN_BUFF_SIZE 1
#define DRIVER_NAME "ass3q1" 

int my_open(struct inode *inode, struct file *filp);
ssize_t my_write(struct file *filp, const char __user *usr_buff, size_t count, loff_t *ppos);
ssize_t my_read(struct file *filp, char __user *usr_buf, size_t count, loff_t *ppos);
int my_release(struct inode *inode, struct file *filp);
long my_ioctl(struct file *filp, unsigned int cmd,unsigned long arg);


/* DEFINE A DEVICE SPECIFIC DATA */
typedef struct privatedata1
{
    int minor_num;
    struct cdev mycdev;
    struct circ_buf KERN_BUFF;
    int KERN_SIZE;
//  char KERN_BUFF[KERN_BUFF_SIZE];
}my_privatedata;

my_privatedata devices[MAX_DEVICE];

/* DECLARE THE REQUIRED VARIABLES */
int major;
int minor=1;
dev_t device_num;

struct class *my_class;
struct device *my_device;

/* DEFINE THE FILE OPERATIONS STRUCTURE OF THE CHAR DRIVER */
struct file_operations my_fops={
    .owner      = THIS_MODULE,
    .open       = my_open,
    .write      = my_write,
    .read       = my_read,
    .release    = my_release,
    .unlocked_ioctl = my_ioctl,
};


/* INITIALISATION FUNCTION */
static int __init my_init(void)
{
    int i;
    int res;
    printk("\nI am in Init Function");
    /* DYNAMICALLY DEVICE NUMBER */
    res = alloc_chrdev_region(&device_num,minor,MAX_DEVICE,DRIVER_NAME);
    if(res<0)
    {
        printk("\nRegister Device Num generation failed");
        return -1;
    }
    major = MAJOR(device_num);

    my_class = class_create(THIS_MODULE,DRIVER_NAME);
    if(my_class == NULL)
    {
        printk("\nClass creation failed");
        return -1;
    }

    for(i=0; i<MAX_DEVICE; i++)
    {
        device_num = MKDEV(major, minor + i);
        cdev_init(&devices[i].mycdev,&my_fops);     //registration of device
        cdev_add(&devices[i].mycdev,device_num,1);  //attachment of device

        /* CREATE DEVICE NODES IN /dev/ DIRECTORY */
        my_device = device_create(my_class,NULL,device_num,NULL,"sample_cdev%d",i);     
        if(my_device == NULL)
        {
            class_destroy(my_class);
            printk("\nDevice creation failed");
            return -1;
        }

        devices[i].minor_num = minor+i;
    }
    return 0;
}

static void __exit my_exit(void)
{
    int i;

    printk("\nI am in Exit Function");
    /* REMOVE DEVICES AND NODES */
    for(i=0; i<MAX_DEVICE; i++)
    {
        device_num = MKDEV(major, minor + i);
        cdev_del(&devices[i].mycdev);
        device_destroy(my_class, device_num);
    }

    /* DESTROY CLASS */
    class_destroy(my_class);

    /* UNREGISTER DEVICE WITH KERNEL */
    device_num = MKDEV(major, minor);
    unregister_chrdev_region(device_num, MAX_DEVICE);
}

/* DEVICE OPEN METHOD */
int my_open(struct inode *inode, struct file *filp)
{
    my_privatedata *dev = container_of(inode->i_cdev, my_privatedata, mycdev);
    filp->private_data = dev;
    dev->KERN_SIZE = 4096;
    printk("\nIn character driver open function device node %d", dev->minor_num);
    return 0;
}

/* DEVICE WRITE METHOD */
ssize_t my_write(struct file *filp, const char __user *usr_buff, size_t count, loff_t *ppos)
{
    my_privatedata *dev = filp->private_data;

    if(CIRC_SPACE(dev->KERN_BUFF.head, dev->KERN_BUFF.tail, dev->KERN_SIZE) >= 1)
    {
        int i;
        char ch;
        for(i=0; i<count; i++)
        {
            get_user(ch, &usr_buff[i]);
            dev->KERN_BUFF.buf[dev->KERN_BUFF.head] = ch;
            printk("\nIn character driver write function and value of KERN_BUFF is: %s", dev->KERN_BUFF.buf);
            dev->KERN_BUFF.head = (dev->KERN_BUFF.head + 1) & (dev->KERN_SIZE-1);
        }
    }   
    else
    {
        printk("\nCopy from user to kernel space failed");
        return -EFAULT;
    }
    return 0;
}

/* DEVICE READ METHOD */
ssize_t my_read(struct file *filp, char __user *usr_buf, size_t count, loff_t *ppos)
{
    my_privatedata *dev = filp->private_data;
    int res;
    printk("\nI am in character driver read function");
    dev->KERN_BUFF.buf = "Bye Bye";
//  usr_buf = dev->KERN_BUFF.buf;

    res = copy_to_user((char *)usr_buf, (char *)dev->KERN_BUFF.buf, strlen(dev->KERN_BUFF.buf)+1);
    printk("\nData '%s' from kernel buffer to user buffer copied successfully with bytes: %d",dev->KERN_BUFF.buf,strlen(dev->KERN_BUFF.buf));

/*  if(res == 0)
    {
        printk("\nData '%s' from kernel buffer to user buffer copied successfully with bytes: %d",dev->KERN_BUFF.buf,strlen(dev->KERN_BUFF.buf));
        memset(dev->KERN_BUFF.buf, 0, strlen(dev->KERN_BUFF.buf));
        return strlen(dev->KERN_BUFF.buf);
    }
    else
    {
        printk("\nCopy from kernel to user failed");
        return -EFAULT;
    }
*/
    return 0;
}

/* DEVICE RELEASE METHOD */
int my_release(struct inode *inode, struct file *filp)
{
    my_privatedata *dev = filp->private_data;
    printk("\nI am in release function and minor number is %d", dev->minor_num);
    return 0;
}

/* DRIVER IOCTL METHOD */
long my_ioctl(struct file *filp, unsigned int cmd,unsigned long arg)
{
    printk("\nIn Driver ioctl function");
    my_privatedata *dev = filp->private_data;

    switch (cmd)
    {
        case 1:
            {
                int count;
                count = CIRC_CNT(dev->KERN_BUFF.head, dev->KERN_BUFF.tail,dev->KERN_SIZE);
                printk("\nSize of buffer count is %d",count);
                return count;
                break;
            }
        case 2:
            {
                int space;
                space = CIRC_SPACE(dev->KERN_BUFF.head, dev->KERN_BUFF.tail,dev->KERN_SIZE);
                printk("\nSize of buffer count is %d",space);
                return space;
                break;
            }
        default:
            {
                printk("\nNothing to show");
                break;
            }
    }
}

module_init(my_init);
module_exit(my_exit);

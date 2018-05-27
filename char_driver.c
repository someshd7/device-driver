#include"myhead.h"
#define MINOR1 0
dev_t cd;
unsigned int count=1;

int dopen(struct inode *myinode,struct file *myfile)
{
        printk(KERN_ALERT"\nFile opened\n");
        return 0;
}

int dclose(struct inode *myinode, struct file *myfile)
{
        printk(KERN_ALERT"\nFile closed\n");
        return 0;
}
ssize_t dread (struct file *myfile,char __user *buffer,size_t cnt,loff_t *offset )
 {
        unsigned long x;
    char str[]="asim chutpadoida\n" ;
    copy_to_user(buffer , str, strlen(str));
    printk("%lu",x);
    return 0;
  }


struct file_operations fop={
.open=dopen,
.read=dread,
//.write=dwrite,
.release=dclose,
};

struct cdev mycdev;
static int __init charD_init(void)
{
        int x,y,z;
        x=alloc_chrdev_region(&cd,MINOR1,count,"mydev");
        if(x<0)
        {
                            printk(KERN_ALERT"\nDev number not allocated\n");
                return x;
        }

        y=register_chrdev_region(MINOR1,count,"mydev");
        if(y<0)
        {
                printk(KERN_ALERT"\nRegistration Failure\n");
                unregister_chrdev_region(MINOR1,count);
                return y;
        }
        printk(KERN_INFO "\nmajor no is %d minor is %d\n",MAJOR(cd),MINOR(cd));
        cdev_init(&mycdev,&fop);

        z=cdev_add(&mycdev,cd,count);
        if(z<0)
        {
                cdev_del(&mycdev);
                printk(KERN_ALERT"\nAdding cdev failed\n");
                return z;
        }
        return 0;
}
static void __exit charD_exit(void)
{
        unregister_chrdev_region(MINOR1,count);
        cdev_del(&mycdev);
        printk(KERN_ALERT"\nBye\n");
}

module_init(charD_init);
module_exit(charD_exit);
                                        



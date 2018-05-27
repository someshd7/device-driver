#include<kernel/init.h>
#include<kernel/module.h>
#include<kernel/types.h>
#include<kernel/fs.h>
#include<kernel/cdev.h>
#include<asm/uaccess.h>
#include<linux/kernel.h>
#include<linux/circ_buf.h>



dev_t dev;
char kbuff[]="this is from kernel\n";
struct cdev cdev;
circ_buff cbuff;



int open (struct inode *myinode , struct file *filp)
{
	printk(KERN_INFO "\n in open function \n");
	return 0;
}

int close (struct inode *myinode , struct file *filp)
{
	printk(KERN_INFO "\n in close function \n");
	return 0;
}

ssize read(struct inode *myinode, char __user *ubuff , size_t count , loff_t *offset)
{
	
	int count;
	```````
	count=min(count,circ_buf(cbuff.head,cbuff.tail,cbuff.count));
	temp=copy_to_user(ubuff, kbuff , strlen(kbuff) );
	if(temp<0)
	{
		printk(KERN_ALERT "failed to read\n");
		return -1;
	}
	printk("%s",ubuff);
	return 0;
}

ssize write(struct inode *myinode, char __user *ubuff, size_t count , loff_t *offset)
{
	unsigned long temp;
	temp=copy_from_user(kbuff,ubuff,count);
	if(temp<0)
	{
		printk(KERN_ALERT "failed to write\n");
		return -1;
	}
	return 0;
}
 
struct file_operations fops={
	.open=my_open,
	.release=my_close,
	.write=my_write,
	.read=my_read,
	};



static int __init my_init(void)
{
	int res,ret;
	unsigned int minor;
	res=alloc_chrdev_region(&dev,minor,count,"mydevice")
	if(res<0)
	{
		printk(KERN_ALERT "\n numbers not allocated \n");
		return res;
	}
	printk(KERN_INFO "major no: %d\tminor no:%d\n",MAJOR(dev),MINOR(dev)); 
	cdev_init(&cdev,fops);
	
	ret=cdev_add(&cdev,&dev, count);
	if(ret<0)
	{
		printk(KERN_INFO "\n device not added \n");
		unregister_chrdev_unregister(&dev, count);
		cdev_del(&dev);
		return ret;
	}
        return 0;
}

static void __exit my_exit(void)
{
	unregister_chrdev_region(&dev,count);

	cdev_del(&dev);

	printk(KERN_INFO "\n in exit function \n");
	return 0;
}



module_init(my_init);
module_exit(my_exit);












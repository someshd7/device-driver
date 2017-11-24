#include<linux/init.h>
#include<linux/module.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("somesh");

static int hello_init(void){
	printk(KERN_ALERT "\nhello world\n");
	return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT"\nbye from exit\n");
}
module_init(hello_init);
module_exit(hello_exit);



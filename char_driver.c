#include<linux/types.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>

dev_t dev;
struct cdev _cdev;
struct file_oper-ations fops={
	.open=dd_open,
	.relese=dd_close




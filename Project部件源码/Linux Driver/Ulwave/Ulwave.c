#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/string.h>

#define CLASS_NAME "ulwave"
#define DEVICE_NAME "ulwave0"
#define ULWAVE_PHY_ADDR 0x70400000


MODULE_AUTHOR("SmartCar");
MODULE_DESCRIPTION("Smartcar ulwave dirver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");

static int mydriver_major;
static struct class* mydriver_class = NULL;
static struct device* mydriver_device = NULL;

void *ulwave_addr = 0;

static struct file_operations mydriver_fops = {
    .owner = THIS_MODULE,
};


ssize_t sys_distance1_get(struct device *dev,struct device_attribute *attr, char *buf)
{
    long data;
    data = readl(ulwave_addr+4);
    return sprintf(buf, "%ld0 ns\n", data);
}


ssize_t sys_distance2_get(struct device *dev,struct device_attribute *attr, char *buf)
{
    long data;
    data = readl(ulwave_addr+8);
    return sprintf(buf, "%ld0 ns\n", data);
}

ssize_t sys_distance3_get(struct device *dev,struct device_attribute *attr, char *buf)
{
    long data;
    data = readl(ulwave_addr+12);

    return sprintf(buf, "%ld0 ns\n", data);
}
ssize_t sys_data_ready(struct device *dev,struct device_attribute *attr, char *buf)
{
    long data;
    int n;
    data = readl(ulwave_addr);

    if(data & 0x2)
        n = sprintf(buf, "no\n");
    else
        n = sprintf(buf, "yes\n");

    return n;
}
static ssize_t sys_trigger (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long data;
   
    if(buf[0] == '1'){
        data = readl(ulwave_addr);
        while(data & 0x2){
            data = readl(ulwave_addr);
        }

        writel(0x1,ulwave_addr);
    }

    return count;
}

 
static DEVICE_ATTR(trigger, S_IWUSR|S_IRUSR, sys_data_ready, sys_trigger); 
static DEVICE_ATTR(distance1, S_IRUSR, sys_distance1_get, NULL); 
static DEVICE_ATTR(distance2, S_IRUSR, sys_distance2_get, NULL); 
static DEVICE_ATTR(distance3, S_IRUSR, sys_distance3_get, NULL); 


static int __init mydriver_module_init(void)
{
    int ret;

    mydriver_major=register_chrdev(0, DEVICE_NAME, &mydriver_fops);
    if (mydriver_major < 0){
        printk("failed to register device.\n");
        return -1;
    }

    mydriver_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(mydriver_class)){
        printk("failed to create device class.\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }

    mydriver_device = device_create(mydriver_class, NULL, MKDEV(mydriver_major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mydriver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }

    ret = device_create_file(mydriver_device, &dev_attr_distance1);
    if (ret < 0)
        printk("failed to create /sys endpoint");
	
	ret = device_create_file(mydriver_device, &dev_attr_distance2);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
	ret = device_create_file(mydriver_device, &dev_attr_distance3);
    if (ret < 0)
        printk("failed to create /sys endpoint");
		
	ret = device_create_file(mydriver_device, &dev_attr_trigger);
    if (ret < 0)
        printk("failed to create /sys endpoint");

    ulwave_addr = ioremap(ULWAVE_PHY_ADDR, 16);

    printk("[ulwave] inserted ok!!");
    return 0;
}

static void __exit mydriver_module_exit(void)
{
    device_remove_file(mydriver_device, &dev_attr_distance1);
    device_remove_file(mydriver_device, &dev_attr_distance2);
	device_remove_file(mydriver_device, &dev_attr_distance3);
	device_remove_file(mydriver_device, &dev_attr_trigger);
    
	
    device_destroy(mydriver_class, MKDEV(mydriver_major, 0));
    class_unregister(mydriver_class);
    class_destroy(mydriver_class);
    unregister_chrdev(mydriver_major, DEVICE_NAME);
    printk("[motor] pwm module exit.\n");
}

module_init(mydriver_module_init);
module_exit(mydriver_module_exit);


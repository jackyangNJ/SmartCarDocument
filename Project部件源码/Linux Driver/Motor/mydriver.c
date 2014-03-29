#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/io.h>

#define DEVICE_NAME "device"
#define PWM_PHY_ADDR 0x7e400000


MODULE_AUTHOR("SmartCar");
MODULE_DESCRIPTION("Smartcar motor dirver");
MODULE_VERSION("v1.0");
MODULE_LICENSE("GPL");

static int mydriver_major;
static struct class* mydriver_class = NULL;
static struct device* mydriver_device = NULL;

unsigned long mypwm_addr = 0;

static struct file_operations mydriver_fops = {
    .owner = THIS_MODULE,
};

static ssize_t sys_direction1_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	long value;
    if (buf[0] == '1'){
		value = inl(mypwm_addr);
		value |= 0x40000000;
		outl(value,mypwm_addr);
	}else{
		value = inl(mypwm_addr);
		value &= 0xBFFFFFFF;
		outl(value,mypwm_addr);
	}
    return count;
}

static ssize_t sys_direction2_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	long value;
    if (buf[0] == '1'){
		value = inl(mypwm_addr+4);
		value |= 0x40000000;
		outl(value,mypwm_addr+4);
	}else{
		value = inl(mypwm_addr+4);
		value &= 0xBFFFFFFF;
		outl(value,mypwm_addr+4);
	}
    return count;
}
static ssize_t sys_direction3_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	long value;
    if (buf[0] == '1'){
		value = inl(mypwm_addr+8);
		value |= 0x40000000;
		outl(value,mypwm_addr+8);
	}else{
		value = inl(mypwm_addr+8);
		value &= 0xBFFFFFFF;
		outl(value,mypwm_addr+8);
	}
    return count;
}

static ssize_t sys_direction4_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
	long value;
    if (buf[0] == '1'){
		value = inl(mypwm_addr+0xC);
		value |= 0x40000000;
		outl(value,mypwm_addr+0xC);
	}else{
		value = inl(mypwm_addr+0xC);
		value &= 0xBFFFFFFF;
		outl(value,mypwm_addr+0xC);
	}
    return count;
}


static ssize_t sys_pwm1_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    printk("wrtite to addr\n");
    outl(num, mypwm_addr);
    printk("write to addr OK!!!\n");
    return count;
}

static ssize_t sys_pwm2_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+4);
    return count;
}

static ssize_t sys_pwm3_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+8);
    return count;
}

static ssize_t sys_pwm4_speed_set (struct device* dev, struct device_attribute* attr, const char* buf, size_t count)
{
    long num = 0;
    int i;
    for (i = 0; i < count-1; i++){
        num *= 10;
        num += buf[i] - '0';
    }
    if (num != 0)
        num = num | 0x80000000;
    outl(num, mypwm_addr+12);
    return count;
}

static DEVICE_ATTR(direction1, S_IWUSR, NULL, sys_direction1_set); 
static DEVICE_ATTR(direction2, S_IWUSR, NULL, sys_direction2_set); 
static DEVICE_ATTR(direction3, S_IWUSR, NULL, sys_direction3_set); 
static DEVICE_ATTR(direction4, S_IWUSR, NULL, sys_direction4_set); 
static DEVICE_ATTR(pwm1_speed, S_IWUSR, NULL, sys_pwm1_speed_set);
static DEVICE_ATTR(pwm2_speed, S_IWUSR, NULL, sys_pwm2_speed_set);
static DEVICE_ATTR(pwm3_speed, S_IWUSR, NULL, sys_pwm3_speed_set);
static DEVICE_ATTR(pwm4_speed, S_IWUSR, NULL, sys_pwm4_speed_set);

static int __init mydriver_module_init(void)
{
    int ret;

    mydriver_major=register_chrdev(0, DEVICE_NAME, &mydriver_fops);
    if (mydriver_major < 0){
        printk("failed to register device.\n");
        return -1;
    }

    mydriver_class = class_create(THIS_MODULE, "motor");
    if (IS_ERR(mydriver_class)){
        printk("failed to create device class.\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }

    mydriver_device = device_create(mydriver_class, NULL, MKDEV(mydriver_major, 0), NULL, "device");
    if (IS_ERR(mydriver_device)){
        printk("failed to create device .\n");
        unregister_chrdev(mydriver_major, DEVICE_NAME);
        return -1;
    }

    ret = device_create_file(mydriver_device, &dev_attr_direction1);
    if (ret < 0)
        printk("failed to create /sys endpoint");
	
	ret = device_create_file(mydriver_device, &dev_attr_direction2);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
	ret = device_create_file(mydriver_device, &dev_attr_direction3);
    if (ret < 0)
        printk("failed to create /sys endpoint");
		
	ret = device_create_file(mydriver_device, &dev_attr_direction4);
    if (ret < 0)
        printk("failed to create /sys endpoint");
  
    ret = device_create_file(mydriver_device, &dev_attr_pwm1_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");

    ret = device_create_file(mydriver_device, &dev_attr_pwm2_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_pwm3_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");
    
    ret = device_create_file(mydriver_device, &dev_attr_pwm4_speed);
    if (ret < 0)
        printk("failed to create /sys endpoint");

    mypwm_addr = (unsigned long)ioremap(PWM_PHY_ADDR, sizeof(u32));
   
    printk("my pwm driver initial successfully!\n");
    return 0;
}

static void __exit mydriver_module_exit(void)
{
    device_remove_file(mydriver_device, &dev_attr_direction1);
    device_remove_file(mydriver_device, &dev_attr_direction2);
	device_remove_file(mydriver_device, &dev_attr_direction3);
	device_remove_file(mydriver_device, &dev_attr_direction4);
    device_remove_file(mydriver_device, &dev_attr_pwm1_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm2_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm3_speed);
    device_remove_file(mydriver_device, &dev_attr_pwm4_speed);
	
    device_destroy(mydriver_class, MKDEV(mydriver_major, 0));
    class_unregister(mydriver_class);
    class_destroy(mydriver_class);
    unregister_chrdev(mydriver_major, DEVICE_NAME);
    printk("pwm module exit.\n");
}

module_init(mydriver_module_init);
module_exit(mydriver_module_exit);

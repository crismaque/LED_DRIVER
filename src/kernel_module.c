#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#include "kernel_module.h"

static int major;
static struct class *led_class = NULL;
static struct device *led_device = NULL;
static dev_t dev_num;

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_release,
	.write = led_write,
	.read = led_read,
};

static int led_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "LED driver abierto\n");
	return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "LED dirver cerrado\n");
	return 0;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	char kbuf[10];
	size_t copy_len;

	if (len == 0)
		return 0;

	copy_len = len < sizeof(kbuf) - 1 ? len : sizeof(kbuf) - 1;

	if(copy_from_user(kbuf,buf,copy_len))
		return -EFAULT;

	kbuf[copy_len] = '\0';

	if(copy_len > 0 && kbuf[copy_len - 1] == '\n')
		kbuf[copy_len - 1] = '\0';

	if(strncmp(kbuf,"on",2) == 0 || strncmp(kbuf,"1",1) == 0){
		gpio_set_value(LED_GPIO_PIN, 1);
		printk(KERN_INFO "LED encendido\n");
	} else if(strncmp(kbuf,"off",3) == 0 || strncmp(kbuf,"0",1) == 0){
		gpio_set_value(LED_GPIO_PIN, 0);
		printk(KERN_INFO "LED apagado\n");
	} else{
		printk(KERN_WARNING "Comando desconocido: %s\n",kbuf);
		return -EINVAL;
	}

										return len;
}

static ssize_t led_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	char status[3];
	size_t status_len;

	if (*offset > 0)
		return 0;

	if(gpio_get_value(LED_GPIO_PIN))
		strcpy(status, "1\n");
	else
		strcpy(status, "0\n");

	status_len = strlen(status);

	if(len < status_len)
		return -EINVAL;

	if(copy_to_user(buf,status,status_len))
		return -EFAULT;

	*offset += status_len;
		return status_len;
}

static int __init led_init(void)
{
	int ret;
			
	printk(KERN_INFO "Inicializando LED driver...\n");

	msleep(1000);

	if(!gpio_is_valid(LED_GPIO_PIN)){
		printk(KERN_ERR "GPIO %d no es válido\n", LED_GPIO_PIN);
		return -ENODEV;
	}

	ret = gpio_request(LED_GPIO_PIN, "led_gpio");
	if(ret<0){
		printk(KERN_ALERT "Fallo al solicitar GPIO %d (error: %d)\n", LED_GPIO_PIN, ret);
		printk(KERN_INFO "Intenta con un GPIO diferente o verifica que no esté en uso\n");
		return ret;
	}

	ret = gpio_direction_output(LED_GPIO_PIN, 0);
	if(ret){
		printk(KERN_ERR "Fallo al configurar GPIO %d como salida\n", LED_GPIO_PIN);
		goto fail_gpio_direction;
	}

	major = register_chrdev(0,DEV_NAME,&fops);
	if(major < 0){
		printk(KERN_ERR "Fallo al registrar dispositivo de caracteres\n");
		ret = major;
		goto fail_chrdev;
	}
	
	led_class = class_create(THIS_MODULE, "led_class");
	if(IS_ERR(led_class)){
		printk(KERN_ERR "Fallo al crear clase de dispositivo\n");
		ret = PTR_ERR(led_class);
		goto fail_class;
	}

	dev_num = MKDEV(major,0);
	led_device = device_create(led_class, NULL, dev_num, NULL, DEV_NAME);
	if(IS_ERR(led_device)){
		printk(KERN_ERR "Fallo al crear nodo de dispositivo\n");
		ret = PTR_ERR(led_device);
		goto fail_device;
	}

	printk(KERN_INFO "LED driver cargado existosamente\n");
	printk(KERN_INFO "Dispositivo creado: /dev/%s (Major: %d)\n", DEV_NAME, major);
	printk(KERN_INFO "GPIO %d configurado para control de LED\n", LED_GPIO_PIN);

	return 0;

fail_device:
	class_destroy(led_class);
fail_class:
	unregister_chrdev(major, DEV_NAME);
fail_chrdev:
fail_gpio_direction:
	gpio_free(LED_GPIO_PIN);
	return ret;
}

static void __exit led_exit(void)
{
	printk(KERN_INFO "Descargando LED driver...\n");
			
	gpio_set_value(LED_GPIO_PIN, 0);
				
	if(led_device)
		device_destroy(led_class,dev_num);
	if(led_class)
		class_destroy(led_class);
						
	unregister_chrdev(major, DEV_NAME);
	gpio_free(LED_GPIO_PIN);

	printk(KERN_INFO "LED driver descargado exitosamente\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cristian Quenguan - Gabriela Romo");
MODULE_DESCRIPTION("Controlador de dispositivo para manejar un LED GPIO");

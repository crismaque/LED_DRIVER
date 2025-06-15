// Librerias necesarias para el desarrollo de un módulo del kernel
#include <linux/module.h> // Necesario para todos los módulos del kernel
#include <linux/init.h> // Macros __init y __exit
#include <linux/fs.h> // Estructuras y funciones para dispositivos de carácter
#include <linux/uaccess.h> // Para copiar datos entre espacio de usuario y kernel
#include <linux/gpio.h> // API de control de GPIO basada en número
#include <linux/device.h> // Funciones para crear dispositivos en /dev
#include <linux/cdev.h> // Funciones y estructuras para dispositivos de carácter
#include <linux/delay.h> // Funciones de retardo

#include "kernel_module.h" 

// Variables para manejar la clase, dispositivo y número mayor
static int major;
static struct class *led_class = NULL;
static struct device *led_device = NULL;
static dev_t dev_num; // Número mayor y menor del dispositivo

// Estructura con las operaciones que soporta el archivo de dispositivo
static struct file_operations fops = {
	.owner = THIS_MODULE, // Define el propietario del módulo
	.open = led_open, // Función que se llama al abrir /dev/led_gpio
	.release = led_release, // Función que se llama al cerrar /dev/led_gpio
	.write = led_write, // Escribir para encender/apagar el LED
	.read = led_read, // Leer estado del LED (0/1)
};

// Función que se ejecuta al abrir el dispositivo
static int led_open(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "LED driver abierto\n");
	return 0;
}

// Función que se ejecuta al cerrar el dispositivo
static int led_release(struct inode *inode, struct file *file)
{
	printk(KERN_INFO "LED dirver cerrado\n");
	return 0;
}

// Función de escritura
static ssize_t led_write(struct file *file, const char __user *buf, size_t len, loff_t *offset)
{
	char kbuf[10]; // Buffer local donde se copiarán los datos del usuario
	size_t copy_len;

	if (len == 0)
		return 0;

	// Determinar cuántos bytes copiar (máximo 9 + '\0')
	copy_len = len < sizeof(kbuf) - 1 ? len : sizeof(kbuf) - 1;

	// Copiar datos del espacio de usuario al kernel
	if(copy_from_user(kbuf,buf,copy_len))
		return -EFAULT;

	kbuf[copy_len] = '\0'; // Asegurar que sea una cadena válida

	// Eliminar salto de línea si viene de comandos tipo 'echo'
	if(copy_len > 0 && kbuf[copy_len - 1] == '\n')
		kbuf[copy_len - 1] = '\0';

	// Interpretar el comando recibido
	if(strncmp(kbuf,"on",2) == 0 || strncmp(kbuf,"1",1) == 0){
		gpio_set_value(LED_GPIO_PIN, 1); // Encender el LED
		printk(KERN_INFO "LED encendido\n");
	} else if(strncmp(kbuf,"off",3) == 0 || strncmp(kbuf,"0",1) == 0){
		gpio_set_value(LED_GPIO_PIN, 0); // Apagar el LED
		printk(KERN_INFO "LED apagado\n");
	} else{
		printk(KERN_WARNING "Comando desconocido: %s\n",kbuf);
		return -EINVAL; // Comando inválido
	}

	return len; // Devuelve el número de bytes escritos
}

// Función de lectura
static ssize_t led_read(struct file *file, char __user *buf, size_t len, loff_t *offset)
{
	char status[3]; // Cadena de respuesta ("1\n" o "0\n")
	size_t status_len;

	if (*offset > 0)
		return 0; // Solo permite una lectura por llamada

	// Leer el estado actual del GPIO y preparar la respuesta
	if(gpio_get_value(LED_GPIO_PIN))
		strcpy(status, "1\n");
	else
		strcpy(status, "0\n");

	status_len = strlen(status);

	// Si el buffer del usuario es muy pequeño, devuelve error
	if(len < status_len)
		return -EINVAL;

	// Copiar el estado del LED al espacio de usuario
	if(copy_to_user(buf,status,status_len))
		return -EFAULT;

	*offset += status_len; // Actualiza el offset para evitar lecturas múltiples
	return status_len; // Devuelve el número de bytes leidos
}

// Función de inicialización del módulo
static int __init led_init(void)
{
	int ret;
			
	printk(KERN_INFO "Inicializando LED driver...\n");

	msleep(1000); // Retardo artificial de 1 segundo

	// Verifica si el GPIO definido es válido
	if(!gpio_is_valid(LED_GPIO_PIN)){
		printk(KERN_ERR "GPIO %d no es válido\n", LED_GPIO_PIN);
		return -ENODEV;
	}

	// Solicita el GPIO para asegurarse que no esté en uso
	ret = gpio_request(LED_GPIO_PIN, "led_gpio");
	if(ret<0){
		printk(KERN_ALERT "Fallo al solicitar GPIO %d (error: %d)\n", LED_GPIO_PIN, ret);
		printk(KERN_INFO "Intenta con un GPIO diferente o verifica que no esté en uso\n");
		return ret;
	}

	// Configura el GPIO como salida (inicialmente apagado)
	ret = gpio_direction_output(LED_GPIO_PIN, 0);
	if(ret){
		printk(KERN_ERR "Fallo al configurar GPIO %d como salida\n", LED_GPIO_PIN);
		goto fail_gpio_direction;
	}

	// Registrar el dispositivo de carácter dinámicamente
	major = register_chrdev(0,DEV_NAME,&fops);
	if(major < 0){
		printk(KERN_ERR "Fallo al registrar dispositivo de caracteres\n");
		ret = major;
		goto fail_chrdev;
	}

	// Crear clase para el dispositivo (/dev/led_gpio)
	led_class = class_create(THIS_MODULE, "led_class");
	if(IS_ERR(led_class)){
		printk(KERN_ERR "Fallo al crear clase de dispositivo\n");
		ret = PTR_ERR(led_class);
		goto fail_class;
	}

	// Crear el nodo /dev/led_gpio
	dev_num = MKDEV(major,0);
	led_device = device_create(led_class, NULL, dev_num, NULL, DEV_NAME);
	if(IS_ERR(led_device)){
		printk(KERN_ERR "Fallo al crear nodo de dispositivo\n");
		ret = PTR_ERR(led_device);
		goto fail_device;
	}

	// Mensajes de confirmación
	printk(KERN_INFO "LED driver cargado existosamente\n");
	printk(KERN_INFO "Dispositivo creado: /dev/%s (Major: %d)\n", DEV_NAME, major);
	printk(KERN_INFO "GPIO %d configurado para control de LED\n", LED_GPIO_PIN);

	return 0; // Instalación correcta del LED driver

// Manejo de errores
fail_device:
	class_destroy(led_class);
fail_class:
	unregister_chrdev(major, DEV_NAME);
fail_chrdev:
fail_gpio_direction:
	gpio_free(LED_GPIO_PIN);
	return ret;
}

// Función de salida del módulo
static void __exit led_exit(void)
{
	printk(KERN_INFO "Descargando LED driver...\n");

	// Apagar el LED antes de liberar
	gpio_set_value(LED_GPIO_PIN, 0);

	// Eliminar el nodo de dispositivo y clase
	if(led_device)
		device_destroy(led_class,dev_num);
	if(led_class)
		class_destroy(led_class);

	// Eliminar el registro del dispositivo de carácter y liberar GPIO
	unregister_chrdev(major, DEV_NAME);
	gpio_free(LED_GPIO_PIN);

	printk(KERN_INFO "LED driver descargado exitosamente\n");
}

// Declaración de funciones de inicialización y salida
module_init(led_init);
module_exit(led_exit);

// Información del módulo
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cristian Quenguan - Gabriela Romo");
MODULE_DESCRIPTION("Controlador de dispositivo para manejar un LED GPIO");

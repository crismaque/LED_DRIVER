#ifndef KERNEL_MODULE_H
#define KERNEL_MODULE_H

#include <linux/fs.h>

// Nombre del dispositivo que aparecerá en /dev
#define DEV_NAME "led_gpio"
// Número del pin GPIO que controla el LED
#define LED_GPIO_PIN 23

// Funciones para las operaciones del archivo de dispositivo
static int led_open(struct inode *, struct file *); // Función al abrir el dispositivo
static int led_release(struct inode *, struct file *); // Función al cerrar el dispositivo
static ssize_t led_write(struct file *, const char __user *, size_t, loff_t *); // Escribir datos (comandos)
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *); // Leer estado del LED

#endif 

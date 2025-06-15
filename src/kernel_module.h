#ifndef KERNEL_MODULE_H
#define KERNEL_MODULE_H

#include <linux/fs.h>

#define DEV_NAME "led_gpio"
#define LED_GPIO_PIN 23

static int led_open(struct inode *, struct file *);
static int led_release(struct inode *, struct file *);
static ssize_t led_write(struct file *, const char __user *, size_t, loff_t *);
static ssize_t led_read(struct file *, char __user *, size_t, loff_t *);

#endif 

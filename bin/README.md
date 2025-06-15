# **Carpeta bin/**

Esta carpeta contiene los **archivos ejecutables finales** generados tras compilar el proyecto `LED_DRIVER`. Es el resultado directo de la
compilación de los archivos fuente que se encuentran en `src/`.

## Contenido de la carpeta:

- `kernel_module.ko`
  Este es el **módulo del kernel** (`.ko` significa Kernel Object) que se puede cargar en el sistema mediante `insmod` o `modprobe`. Este
  módulo implemente un **dispositivo de carácter** que permite controlar un LED conectado al GPIO 23 mediante el archivo `/dev/led_gpio`.
- `user_app`
  Este es el **programa en espacio de usuario**, que permite interactuar fácilmente con el driver a través de la terminal.
  
  Admite los siguientes comandos:
  ```bash
  ./user_app on       #Enciende el LED
  ./user_app off      #Apaga el LED
  ./user_app status   #Muestra si el LED está encendido o apagado

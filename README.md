# Taller #2 de Programación de Sistemas Linux Embebidos

Desarrollo de un controlador de dispositivo de caracteres de Linux para el control de un LED GPIO.

## Autores:
- Cristian Marcelo Quenguan Malte - C.C. 1004532387
- Gabriela Romo Mendoza - C.C. 1080691539

## Descripción:
'LED_DRIVER' es un módulo de kernel de Linux que implementa un controlador de dispositivo de caracteres que controla un LED conectado a un pin GPIO. El controlador implementa operaciones de archivo estándar como inicializar, salir, abrir, liberar, leer y escribir. El LED se controla mediante un espacio de usuario que interactua con el controlador de dispositivo, este permite ejecutar los comandos de control básicos para encender y apagar el LED.

## Objetivos:
- Entender la arquitectura de un controlador de dispositivo de caracteres de Linux.
- Aprender a registrar y exponer módulos de kernel a través de archivos del dispositivo.
- Implementar el control GPIO dentro de kernel de Linux.
- Diseñar aplicaciones de espacio de usuario que interactúen con archivos del dispositivo.
- Aplicar prácticas sólidas de ingeniería de software, incluida la documentación, las pruebas y el control de revisiones.


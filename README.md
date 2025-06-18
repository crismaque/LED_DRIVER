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

El código fue desarrollado y probado en una Raspberry Pi ejecutado en Linux. Este repositorio incluye todo el código fuente, herramientas de compilación y evidencia de pruebas. 

## Requisitos previos
- Raspberry Pi con Linux (Raspberry Pi OS)
- Paquetes instalados:

```bash
sudo apt update
sudo apt install raspberrypi-kernel-headers
```
o también:

```bash
sudo apt update
sudo apt install raspberrypi-kernel-headers build-essential git
```
Esto instalará el directorio:

`/lib/modules/$(uname -r)/build`

Para verificar que el directorio existe:
```bash
ls -l /lib/modules/$(uname -r)/build
```
Se debería ver un enlace simbólico apuntando al árbol de código fuente o headers del kernel.

## Montaje del Hardware 
- LED conectado al pin GPIO 23
- Resistencia limitadora
- GND

## Clonación del repositorio

```bash
git clone
cd LED_DRIVER
```














## Pruebas
Se describe el procedimiento para validar el correcto funcionamiento del driver de dispositivo de carácter para el control de un LED GPIO.

## 1. Compilación en instalación
Se compiló utilizando el Makefile de la raíz, que automatiza la compilación de módulo de kernel, la aplicación en espacio de usuario y la creación del dispositivo `/dev/led_gpio`.
```bash
make all
make install
make link
```
`make install` muestra los mensajes de kernel de linux en la instalación, también se pueden verificar por aparte usando:
```bash
dmesg
```
Con esto se verificó que:
- El driver se haya cargado bien.
- La asignación del número mayor.
- La configuración del GPIO 23.

![](https://github.com/crismaque/LED_DRIVER/blob/129a0259208a56d30aa65d4048f98e4f96250233/test/imagen_2025-06-19_090725506.png)

## 2. Verificación del dispositivo
Luego de instalar el módulo, se verificó que el dispositivo `/dev/led_gpio` fue creado correctamente en el sistema.

El dispositivo es un archivo de carácter que permite la comunicación entre la aplicación en espacio de usuario y el driver del kernel.

```bash
ls -l /dev/led_gpio
```
- la letra `c` indica que es un dispositivo de carácter.
- `rw-rw-rw-` indica que el dispositivo tiene permisos de lectura y escritura para todos los usuarios.
- `239` es el número mayor asignado por el kernel.
- `0` es el número menor.


# Pruebas
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

Se obtuvo la siguiente estructura con los archivos necesarios para el dispositivo:
```bash
tree
```
![](https://github.com/crismaque/LED_DRIVER/blob/80121233f074ef86a9d765433edb4b2510b4146b/test/imagen_2025-06-19_160817387.png)

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

![](https://github.com/crismaque/LED_DRIVER/blob/0482c695a6fc203cf69225742f43459950802751/test/imagen_2025-06-19_094152203.png)

## 3. Control de LED
Desde la raíz se verificó el funcionamiento del driver y su interacción con la aplicación en espacio de usuario, se realizaron pruebas de control directo del LED mediante comandos enviados al dispositivo `/dev/led_gpio`. 

Desde la terminal se cambió el estado del LED usando los comandos para el espacio de usuario.

## Encendido
```bash
./user_app on
```

```bash
./user_app status
```
![](https://github.com/crismaque/LED_DRIVER/blob/d7b4c7b3b6ef324cf3e48283cc76c130488c840c/test/WhatsApp%20Image%202025-06-19%20at%202.39.34%20PM.jpeg)

## Apagado
```bash
./user_app off
```

```bash
./user_app status
```
![](https://github.com/crismaque/LED_DRIVER/blob/d7b4c7b3b6ef324cf3e48283cc76c130488c840c/test/WhatsApp%20Image%202025-06-19%20at%202.39.35%20PM.jpeg)

## 4. Desinstalación del módulo y limpieza del proyecto
Por último, se probó la correcta desisntalción del módulo y la limpieza de todos los archivos generados por la compilación.
```bash
make uninstall
```
![](https://github.com/crismaque/LED_DRIVER/blob/f73e29a7fcb3b94b89d3d483cce82648307c5113/test/imagen_2025-06-19_145524743.png)
```bash
make clean
```
![](https://github.com/crismaque/LED_DRIVER/blob/f73e29a7fcb3b94b89d3d483cce82648307c5113/test/imagen_2025-06-19_145555260.png)

Los archivos en `bin/` y `temp/` son eliminados, dejando el proyecto listo para una recompilación limpia.
```bash
tree
```
![](https://github.com/crismaque/LED_DRIVER/blob/80121233f074ef86a9d765433edb4b2510b4146b/test/imagen_2025-06-19_160851136.png)

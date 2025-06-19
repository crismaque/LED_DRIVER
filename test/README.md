## Pruebas
Se describe el procedimiento para validar el correcto funcionamiento del driver de dispositivo de carácter para el control de un LED GPIO.

## 1. Compilación en instalación
Se compiló utilizando el Makefile de la raíz, que automatiza la compilación de módulo de kernel, la aplicación en espacio de usuario y la creación del dispositivo `/dev/led_gpio`.
```bash
make all
```
[make all](test/imagen_2025-06-19_081311141.png)


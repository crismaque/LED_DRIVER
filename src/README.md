# **Carpeta Source (src)**

Esta carpeta contiene el código fuente del proyecto `LED_DRIVER`, un controlador de dispositivo de carácter en Linux
que permite controlar un LED conectado a un pin GPIO mediante una interfaz en `\dev`.

Incluye los siguientes archivos fuente y de configuración:

- `kernel_module.h` - Definición de las funciones del módulo del kernel.
- `kernel_module.c` - Implementación del módulo del núcleo de Linux para controlar el LED.
- `user_app.h` - Definición de la ruta del dispositivo y función principal.
- `user_app.c` - Aplicación en espacio de usuario para interactuar con el driver.
- `Makefile` - Compilación del módulo del kernel y del programa de usuario.

## Funcionalidad de cada bloque:

### **- Bloque kernel_module:**
Este bloque contiene la implementación del módulo del núcleo que actúa como driver de un dispositivo de carácter asociado a un pin GPIO. Controla
el encendido y apagado de un LED y expone su estado mediante lectura. 

- `kernel_module.h`
  Define constantes como el nombre del dispositivo (`led_gpio`) y el pin GPIO (`23`). Además, declara los prototipos de las funciones necesarias para
  las operaciones de archivo (`open`, `release`, `write`, `read`).
- `kernel_module.c`
  Implementa un driver de cáracter que:
  - Solicita el GPIO y lo configura como salida.
  - Crea un archivo de dispositivo en `/dev/led_gpio`.
  - Permite encender o apagar el LED desde espacio de usuario escribiendo `1`, `0`, `on` u `off`.
  - Permite consultar el estado actual del LED leyendo el archivo `dev/led_gpio` (retorna `1` o `0`).

### **- Bloque user_app:**
Este bloque corresponde al programa en espacio de usuario que se comunica con el driver a través de operaciones de lectura y escritura al archivo 
`/dev/led_gpio`.

- `user_app.h`
  Define la ruta del dispositivo (`/dev/led_gpio`) y declara la función `main()`.
- `user_app.c`
  Implementa una interfaz por línea de comandos que:
  - Abre el dispositivo `/dev/led_gpio`.
  - Si se ejecuta con el argumento `status`, lee el estado del LED y lo muestra por pantalla.
  - Si se ejecuta con `on` u `off`, escribe el comando correspondiente al driver para controlar el LED.
  - Muestra un mensaje de error si no se pasan argumentos válidos.

### **- Makefile:**
Se encarga de compilar el módulo del kernel (`kernel_module.c`) como un objeto (`.ko`) haciendo uso del comando `make all` mediante la línea:

```make
obj-m := kernel_module.o

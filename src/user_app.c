#include <stdio.h> // Funciones estándar de entrada/salida 
#include <stdlib.h> // Funciones de utilidad como exit()
#include <fcntl.h> // Funcioens para control de archivos (open, O_RDWR, etc)
#include <unistd.h> // Funciones POSIX como read,write,close
#include <string.h> // Funciones para manejo de cadenas

#include "user_app.h"

int main(int argc,char *argv[]){
	int fd; // Descriptor del archivo para /dev/led_gpio
	char buf[4]; // Buffer para leer el estado del LED

	// Verifica que se haya pasado al menos un argumento
	if(argc == 1){
		printf("Usage:\n %s on\n %s off\n %s status\n", argv[0], argv[0], argv[0]);
		return 1;
	}

	// Abre el dispositivo con permisos de lectura y escritura
	fd = open(DEVICE, O_RDWR);
		if(fd < 0){
		perror("Failed to open device"); // Mensaje de error en caso de fallo
		return 1;
	}

	// Si el argumento es "status", se realiza una lectura del estado del LED
	if(strcmp(argv[1],"status") == 0){
		int r = read(fd,buf,sizeof(buf)); // Lee desde el dispositivo
		if(r > 0){
			if(buf[0] == '1')
				printf("LED is ON\n"); // LED encendido
			else if(buf[0] == '0')
				printf("LED is OFF\n"); // LED apagado
			else
				printf("Unknown status\n"); // Valor inesperado
		}else{
			printf("Failed to read status\n"); // Error al leer
		}
	} else{
		// Si no es "status", se interpreta como comando "on" u "off"
		write(fd,argv[1],strlen(argv[1]));
	}

	close(fd); // Cierra el archivo del dispositivo
	return 0;
}

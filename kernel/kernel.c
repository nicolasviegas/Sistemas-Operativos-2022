#include "include/kernel.h"


//cambiar funciones de conexion a otro archivo
bool generar_conexiones(t_log* logger, char* ip, char* puerto, int* fd_mod2) {
    // No hardcodear, levantar de config


    *fd_mod2 = crear_conexion(logger,"CONSOLA", ip, puerto );

    return *fd_mod2 != 0;
}


int main() {
	
	char* ip;
	char* puerto_escucha;

	log_kernel = log_create("kernel.log","consola",1,LOG_LEVEL_TRACE);

	config_kernel = config_create("kernel.config");

	ip = config_get_string_value(config_kernel,"IP_MEMORIA");
	puerto_escucha = config_get_string_value(config_kernel,"PUERTO_ESCUCHA");


	int fd_mod2=0;
	if (!generar_conexiones(log_kernel, ip, puerto_escucha, &fd_mod2)) {
		//cerrar_programa(logger);
		return EXIT_FAILURE;
	}





	return 0;
}


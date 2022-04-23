#include "include/kernel.h"

int main() {
	
	char* ip;
	char* puerto_escucha;

	log_kernel = log_create("kernel.log","consola",1,LOG_LEVEL_TRACE);

	config_kernel = config_create("kernel.config");

	ip = config_get_string_value(config_kernel,"IP_MEMORIA");
	puerto_escucha = config_get_string_value(config_kernel,"PUERTO_ESCUCHA");


	int cliente;






	return 0;
}


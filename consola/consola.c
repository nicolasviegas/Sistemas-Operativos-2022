#include "include/consola.h"
//#include "../../include/consola.h"

//ASI SE CORRE POR CONSOLA
//./consola.out /home/utnso/tp-2022-1c-yaguarethreads-/consola/lista_instrucciones.txt 100


int main(int argc, char** argv){

	if(argc != 3){
		printf("Cantidad de parametros incorrecta");
		return EXIT_FAILURE;
	}

	bool success = false;

	char* path;
	int tam;
	char* ip;
	char* puerto;

	log_consola = log_create("consola.log","consola",1,LOG_LEVEL_TRACE);

	path = argv[1];

	tam = atoi(argv[2]); //IDem anterior pero ahora el numero del 2ndo argumento

	/* --------------------- CREO LISTA DE INSTRUCCIONES-------------------*/
	lista_instrucciones = list_create();

	obtener_instrucciones(path);


	/*------------ AHORA UNA VEZ RESUELTA LA LISTA DE INSTRUCCIONES CREO CONEXION---------*/
	config_consola = config_create("consola.config");

	ip = config_get_string_value(config_consola,"IP_KERNEL");

	puerto = config_get_string_value(config_consola,"PUERTO_KERNEL");

	paquete_consola_kernel = malloc(sizeof(t_paquete));

	int cant_instrucciones = list_size(lista_instrucciones);

	log_error(log_consola,"cant de instrucciones: %d",cant_instrucciones);

	int fd_mod2=0;
	if (!generar_conexiones(log_consola, ip, puerto, &fd_mod2)) {
		cerrar_programa(logger);
		return EXIT_FAILURE;
	}
	log_trace(log_consola,"El fd_mod2 despues de grar conexiones es: %d",fd_mod2);

	send_instrucciones(lista_instrucciones,fd_mod2);

/*
	success = recibir_confirmacion(fd_mod2); //Implementar, reveer esto creo q esta demas

	if(success){
	// terminar_consola(log_consola,lista_instrucciones, conexion,config_consola);
	}
*/

	return EXIT_SUCCESS;
}


void paquete_instrucciones(t_list* instrucciones, int conexion){ //MOVER A LAS SHARED, SE EMPAQUETA AHI PARA UTILIZARLO
																// EN OTROS MODULOS

	//assert("No IMPLEMENTADO");

}

bool recibir_confirmacion(int conexion){
	//assert("No implementado");

	return true;
}



#include "include/consola.h"

//ASI SE CORRE POR CONSOLA
//./consola.out /home/utnso/tp-2022-1c-yaguarethreads-/consola/BASE_2.txt 512

int main(int argc, char** argv){

	if(argc != 3){
		printf("Cantidad de parametros incorrecta");
		return EXIT_FAILURE;
	}

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

	int cant_instrucciones = list_size(lista_instrucciones);

	fd_kernel=0;
	if (!generar_conexiones(log_consola, ip, puerto, &fd_kernel)) {
		cerrar_programa(log_consola);
		return EXIT_FAILURE;
	}
	//log_trace(log_consola,"El fd_kernel despues de grar conexiones es: %d",fd_kernel);

	send_TAM(fd_kernel,tam);
//
	//log_error(log_consola,"Pase el send TAM ");

	send_instrucciones(lista_instrucciones,fd_kernel);

	//log_error(log_consola,"Pase el send instrucciones ");

	log_info(log_consola,"[INFO] La consola finalizo correctamente.");
	terminar_consola(log_consola, lista_instrucciones, fd_kernel, config_consola);

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

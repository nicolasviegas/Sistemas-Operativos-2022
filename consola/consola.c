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

	//strcpy(path,argv[1]); //copio el path del argumento1 en la variable
	//EL STRCPY NO FUNCIONA

	path = argv[1];

	log_trace(log_consola, "pase strcpy");

	/*if(!sintaxis(path)){
		printf("Error de sintaxis en el archivo de pseudocodigo");
		return EXIT_FAILURE;
	}*/

	tam = atoi(argv[2]); //IDem anterior pero ahora el numero del 2ndo argumento

	/* --------------------- CREO LISTA DE INSTRUCCIONES-------------------*/
	lista_instrucciones = list_create();
	obtener_instrucciones(path);

	log_trace(log_consola,"pase obtener instrucciones");

	/*------------ AHORA UNA VEZ RESUELTA LA LISTA DE INSTRUCCIONES CREO CONEXION---------*/
	config_consola = config_create("consola.config");

	ip = config_get_string_value(config_consola,"IP_KERNEL");

	puerto = config_get_string_value(config_consola,"PUERTO_KERNEL");


	//int conexion = crear_conexion(ip,puerto); //ver bien lo de crear conexion

	//paquete_instrucciones(lista_instrucciones, conexion); //IMPLEMENTAR

	//success = recibir_confirmacion(conexion); //Implementar, reveer esto creo q esta demas

	//if(success){
	// terminar_consola(log_consola,lista_instrucciones, conexion,config_consola);
	//}

	return EXIT_SUCCESS;
}


void paquete_instrucciones(t_list* instrucciones, int conexion){

	//assert("No IMPLEMENTADO");

}

bool recibir_confirmacion(int conexion){
	//assert("No implementado");

	return true;
}

void terminar_consola(t_log* log, t_list* lista, int conexion, t_config* config){
	log_destroy(log);
	list_destroy(lista);
	liberar_conexion(conexion);
	config_destroy(config);
}

void obtener_instrucciones(char* path){
	char buffer[100];
	char* linea_instrucciones = string_new();
	instrucciones* estructura_instrucciones;

	FILE* f;
	f = fopen(path,"r");
	if(f == NULL){
		printf("Error al abrir el archivo: %s \n",path);
		return EXIT_FAILURE;
	}

	log_info(log_consola, "Pude leer el archivo \n");

	char* token;
	while(fgets(buffer, 100, f)){

	token = strtok(buffer,"\n"); //con esta funcion separo cada instruccion por linea

	log_info(log_consola,token);

	string_append_with_format(&linea_instrucciones,"%s;",token); //con esta funcion le agrego el ; entre cada instruccion

	log_warning(log_consola,linea_instrucciones);

	}

	fclose(f);
	//free(buffer);

}




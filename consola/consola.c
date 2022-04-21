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
	lista_instrucciones2 = queue_create();

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
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	FILE* f;
	f = fopen(path,"r");
	if(f == NULL){
		printf("Error al abrir el archivo: %s \n",path);
		//return EXIT_FAILURE;
	}

	log_info(log_consola, "Pude leer el archivo \n");

	char* token;
	while(fgets(buffer, 100, f)){

	token = strtok(buffer,"\n"); //con esta funcion separo cada instruccion por linea

	//log_info(log_consola,token);

	if(strncmp(buffer,"NO_OP",5) == 0){

		log_error(log_consola,"Entre en NO_OP");

		estructura_instrucciones->id = strtok(token," ");
		estructura_instrucciones->parametro1 = strtok(NULL," ");
		estructura_instrucciones->parametro2 = strtok(NULL," ");

		list_add(lista_instrucciones,estructura_instrucciones);
		queue_push(lista_instrucciones2,estructura_instrucciones);

		log_trace(log_consola,estructura_instrucciones->id);
		log_trace(log_consola,estructura_instrucciones->parametro1);
		log_trace(log_consola,estructura_instrucciones->parametro2);

	}/*else if(strncmp(buffer,"I/O",3) == 0){

		log_error(log_consola,"Entre en I/O");

		//instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		estructura_instrucciones->id = strtok(token," ");
		estructura_instrucciones->parametro1 = strtok(NULL," ");
		estructura_instrucciones->parametro2 = strtok(NULL," ");

		list_add(lista_instrucciones,&estructura_instrucciones);

		log_trace(log_consola,estructura_instrucciones->id);
		log_trace(log_consola,estructura_instrucciones->parametro1);
		log_trace(log_consola,estructura_instrucciones->parametro2);

	}else if(strncmp(buffer,"READ",4) == 0){

		log_error(log_consola,"Entre en READ");

		//instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		estructura_instrucciones->id = strtok(token," ");
		estructura_instrucciones->parametro1 = strtok(NULL," ");
		estructura_instrucciones->parametro2 = NULL;

		list_add(lista_instrucciones,&estructura_instrucciones);

		log_trace(log_consola,estructura_instrucciones->id);
		log_trace(log_consola,estructura_instrucciones->parametro1);
		log_trace(log_consola,estructura_instrucciones->parametro2);

	}else if(strncmp(buffer,"WRITE",5) == 0){

		log_error(log_consola,"Entre en WRITE");

		//instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		estructura_instrucciones->id = strtok(token," ");
		estructura_instrucciones->parametro1 = strtok(NULL," ");

		estructura_instrucciones->parametro2 = strtok(NULL," ");
		list_add(lista_instrucciones,&estructura_instrucciones);

		log_trace(log_consola,estructura_instrucciones->id);
		log_trace(log_consola,estructura_instrucciones->parametro1);
		log_trace(log_consola,estructura_instrucciones->parametro2);

	}else if(strncmp(buffer,"COPY",4) == 0){

		log_error(log_consola,"Entre en COPY");

		//instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		estructura_instrucciones->id = strtok(token," ");
		estructura_instrucciones->parametro1 = strtok(NULL," ");
		estructura_instrucciones->parametro2 = strtok(NULL," ");

		list_add(lista_instrucciones,&estructura_instrucciones);

		log_trace(log_consola,estructura_instrucciones->id);
		log_trace(log_consola,estructura_instrucciones->parametro1);
		log_trace(log_consola,estructura_instrucciones->parametro2);

	}else if(strncmp(buffer,"EXIT",4) == 0){

		log_error(log_consola,"Entre en EXIT");
		estructura_instrucciones->id = strtok(token," ");

		list_add(lista_instrucciones,&estructura_instrucciones);


		//IMPLEMETNAR

	}*/

	//string_append_with_format(&linea_instrucciones,"%s;",token); //con esta funcion le agrego el ; entre cada instruccion

	//log_warning(log_consola,linea_instrucciones);

	}

	mostrar_lista_instrucciones(lista_instrucciones);

	fclose(f);
	//free(buffer);

}

void mostrar_lista_instrucciones2(t_queue* queue,char * nombre_cola){
	void mostrar_instrucciones(instrucciones* instruccion){
		log_info(log_consola,"ID: %s   PARAMETRO1: %d PARAMETRO2: %d ", instruccion->id,instruccion->parametro1,instruccion->parametro2);
	}
	list_iterate(queue->elements, (void*) mostrar_instrucciones);
}

void mostrar_lista_instrucciones(t_list* lista){

	void mostrar_instruccion(instrucciones lista){
		log_warning(log_consola,"ID : %s",lista.id);
		log_warning(log_consola,"PARAMETRO 1 : %d",lista.parametro1);
		log_warning(log_consola,"PARAMETRO 2 : %d",lista.parametro2);
	}
		list_iterate(lista, (void*) mostrar_instruccion);

}



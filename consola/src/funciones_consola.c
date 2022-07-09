#include "../include/consola.h"


void terminar_consola(t_log* log, t_list* lista, int conexion, t_config* config){
	log_destroy(log);
	list_destroy(lista);
	//liberar_conexion(conexion);
	config_destroy(config);
}

void obtener_instrucciones(char* path){ //1era idea: LO QUE PODRIA HACER SERIA ENVIAR LA LISTA CON EL FORMATO [NO_OP 4,I/O 1,..]
										//ASI EN UNA LISTA DIVIDIDA CON COMAS Y QUE CPU LUEGO LO DIVIDA,
										//2da idea: DENTRO DE NO_OP PONER LA FUNCION SEND_INSTRUCCION(int socket, int NO_OP)
	char buffer[100];
	//char* linea_instrucciones = string_new();

	FILE* f;
	f = fopen(path,"r");
	if(f == NULL){
		printf("Error al abrir el archivo: %s \n",path);
		//return EXIT_FAILURE;
	}

	char* token;
	while(fgets(buffer, 100, f)){

	token = strtok(buffer,"\n"); //con esta funcion separo cada instruccion por linea

	//log_info(log_consola,token);

	if(strncmp(buffer,"NO_OP",5) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en NO_OP");

		char** parametros = string_n_split(token,2," ");

		estructura_instrucciones->id = NO_OP;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = atoi(parametros[1]);
		estructura_instrucciones->parametro2 = NULL;

		list_add(lista_instrucciones,estructura_instrucciones);

	/*	log_trace(log_consola,"%d",estructura_instrucciones->id);
		log_trace(log_consola,"%s",estructura_instrucciones->nombre);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro1);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro2);*/


	}else if(strncmp(buffer,"I/O",3) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en I/O");


		char** parametros = string_n_split(token,2," ");

		estructura_instrucciones->id = IO;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = atoi(parametros[1]);
		estructura_instrucciones->parametro2 = NULL;


		list_add(lista_instrucciones,estructura_instrucciones);

		/*log_trace(log_consola,"%d",estructura_instrucciones->id);
		log_trace(log_consola,"%s",estructura_instrucciones->nombre);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro1);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro2);*/

	}else if(strncmp(buffer,"READ",4) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en READ");

		char** parametros = string_n_split(token,2," ");

		estructura_instrucciones->id = READ;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = atoi(parametros[1]);
		estructura_instrucciones->parametro2 = NULL;

		list_add(lista_instrucciones,estructura_instrucciones);

	/*	log_trace(log_consola,"%d",estructura_instrucciones->id);
		log_trace(log_consola,"%s",estructura_instrucciones->nombre);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro1);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro2);*/

	}else if(strncmp(buffer,"WRITE",5) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en WRITE");

		char** parametros = string_n_split(token,3," ");

		estructura_instrucciones->id = WRITE;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = atoi(parametros[1]);
		estructura_instrucciones->parametro2 = atoi(parametros[2]);

		list_add(lista_instrucciones,estructura_instrucciones);

		/*log_trace(log_consola,"%d",estructura_instrucciones->id);
		log_trace(log_consola,"%s",estructura_instrucciones->nombre);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro1);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro2);*/

	}else if(strncmp(buffer,"COPY",4) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en COPY");

		char** parametros = string_n_split(token,3," ");

		estructura_instrucciones->id = COPY;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = atoi(parametros[1]);
		estructura_instrucciones->parametro2 = atoi(parametros[2]);

		list_add(lista_instrucciones,estructura_instrucciones);

		/*log_trace(log_consola,"%d",estructura_instrucciones->id);
		log_trace(log_consola,"%s",estructura_instrucciones->nombre);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro1);
		log_trace(log_consola,"%d",estructura_instrucciones->parametro2);*/

	}else if(strncmp(buffer,"EXIT",4) == 0){
		instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));
		//log_error(log_consola,"Entre en EXIT");

		char** parametros = string_n_split(token,2," ");

		estructura_instrucciones->id = EXIT;
		estructura_instrucciones->nombre = parametros[0];
		estructura_instrucciones->parametro1 = NULL;
		estructura_instrucciones->parametro2 = NULL;

		list_add(lista_instrucciones,estructura_instrucciones);


	}


	}

	fclose(f);
	//free(buffer);
}

void cerrar_programa(t_log* logger) {
    log_destroy(logger);
}


void empaquetar(int socket){

}

void mostrar_lista_instrucciones(t_list* lista){

}

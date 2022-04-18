#include "include/consola.h"

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

	log_consola = log_create("consola.log","consola",1,LOG_LEVEL_INFO);

	strcpy(path,argv[1]); //copio el path del argumento1 en la variable

	if(!sintaxis(path)){
		printf("Error de sintaxis en el archivo de pseudocodigo");
		return EXIT_FAILURE;
	}

	tam = atoi(argv[2]); //IDem anterior pero ahora el numero del 2ndo argumento

	/* --------------------- CREO LISTA DE INSTRUCCIONES-------------------*/
	lista_instrucciones = list_create();
	lista_instrucciones = obtener_instrucciones(path);

	/*------------ AHORA UNA VEZ RESUELTA LA LISTA DE INSTRUCCIONES CREO CONEXION---------*/
	config_consola = config_create("consola.config");

	ip = config_get_string_value(config_consola,"IP_KERNEL");

	puerto = config_get_string_value(config_consola,"PUERTO_KERNEL");


	int conexion = crear_conexion(ip,puerto); //ver bien lo de crear conexion

	paquete_instrucciones(lista_instrucciones, conexion); //IMPLEMENTAR

	success = recibir_confirmacion(conexion); //Implementar, reveer esto creo q esta demas

	if(success){
		terminar_consola(log_consola,lista_instrucciones, conexion,config_consola);
	}

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
	char* buffer[100];
	instrucciones estructura_instrucciones;

	FILE* f;
	f = fopen(path,"r");
	if(f == NULL){
		printf("Error al abrir el archivo: %s",path);
		return EXIT_FAILURE;
	}

	/*while(fgets(buffer, 100, f)){
		//assert("no implementado");
		int cant_parametros = 0;
		//cant_parametros = nespacios;

		char* token = strtok(buffer," "); //hola 2 2 3

		strcpy(estructura_instrucciones.id,token);

		while(token != NULL){
					token = strtok(NULL," ");
					cant_parametros += 1;
		}

		cant_parametros -= 1;

		char* token2 = strtok(buffer," "); //hola 2 2

		while(token2 != NULL){
			//Hacer algo que trate o procese el string
			int a = atoi(token2);
			token2 = strtok(NULL," ");
			int b = atoi(token2);

			if(cant_parametros == 1){
				estructura_instrucciones.parametro1 = a;
				estructura_instrucciones.parametro2 = NULL;
			}

			if(cant_parametros == 2){
				estructura_instrucciones.parametro1 = a;
				estructura_instrucciones.parametro2 = b;
			}


		}
	}*/

	while(fgets(buffer, 100, f)){
	char* token = strtok(buffer," "); //copy 2 1
	int a = atoi(token); //hola
	while(token != NULL){

		token = strtok(NULL," ");
		}

	}

	fclose(f);
	free(buffer);

}


bool sintaxis(char* path){
	bool success;
	FILE* f;
	f = fopen(path,"r");
	char* buffer[100];
	if(f == NULL){
		printf("Error al abrir el archivo %s",path);
		return false;
	}

	while(fgets(buffer,100,f)){
		char* tok = strtok(buffer," "); //tok tiene que ser un int
		switch(tok){
			case (unParametro(tok)):
					int i=0;
					tok = strtok(NULL," ");
					while(isdigit(tok)){
						i++;
						tok = strtok(NULL," ");
					}
					if(i!=1){success = false;}
					break;

			case(dosParametros(tok)):
					int i = 0;
					tok = strtok(NULL," ");
					while(isdigit(tok)){
						i++;
						tok = strtok(NULL," ");
					}
					if(i !=2 ){success = false;}
					break;


			case(tok == 5): //exit tiene que ser un numero
					int i= 0;
					tok = strtok(NULL," ");
					if(tok != NULL){success = false;}
					break;

			default:
				success = false;

			}

	}

	return success;
}

bool unParametro(char* id){
	if(id == 0 || id == 1 || id == 2){ //NO_OP, IO, READY
		return true;
	}
	else return false;
}

bool dosParametros(char*id){
	if(id == 3 || id == 4){ // COPY, WRITE
		return true;
	}
	else return false;
}


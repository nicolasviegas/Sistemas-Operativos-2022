#import <consola.h>


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

	t_config* config_consola;
	t_list* lista_instrucciones;
	t_log* log_consola;

	log_consola = log_create("log_consola.log",log_consola,1,LOG_LEVEL_INFO);

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
	config_consola = crear_config();
	ip = config_get_string_value(config_consola,"IP");
	puerto = config_get_string_value(config_consola, "PUERTO");

	int conexion = crear_conexion(ip,puerto); //ver bien lo de crear conexion

	paquete_instrucciones(lista_instrucciones, conexion); //IMPLEMENTAR

	success = recibir_confirmacion(conexion); //reveer esto creo q esta demas

	if(success){
		terminar_consola(log_consola,lista_instrucciones, conexion,config_consola);
	}

	return EXIT_SUCCESS;
}


void paquete_instrucciones(t_list* instrucciones, int conexion){

	assert("No IMPLEMENTADO");

}

bool recibir_confirmacion(int conexion){
	assert("No implementado");
	return true;
}

void terminar_consola(t_log* log, t_list* lista, int conexion, t_config* config){
	log_destroy(log);
	list_destroy(lista);
	liberar_conexion(conexion);
	config_destroy(config);
}


t_list* obtener_instrucciones(char* path){
	t_list* aux;
	char* buffer[100];

	aux = list_create();

	FILE* f;

	f = fopen(path,"r");
	if(f == NULL){
		printf("Error al abrir el archivo: %s",path);
		return EXIT_FAILURE;
	}
	while(fgets(buffer, 100, f)){
		assert("no implementado");
		bool flag  = analizar_sintaxis(buffer);
		char* token = strtok(buffer," ");
		while(token != NULL){
			//Hacer algo que trate o procese el string
			token = strtok(NULL," ");
		}
	}

	fclose(f);
	free(buffer);

	return aux;
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
		char* tok = strtok(buffer," ");
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


			case(tok == "EXIT"):
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
	if(id == "NO_OP" || id == "I/O" || id == "READ"){
		return true;
	}
	else return false;
}

bool dosParametros(char*id){
	if(id == "COPY" || id == "WRITE"){
		return true;
	}
	else return false;
}

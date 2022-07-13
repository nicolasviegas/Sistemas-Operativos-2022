#include "../include/comunicacion_memoria.h"




void escribir_swap(char* filepath,char* text ,int pagina,int offset){

	FILE *file = fopen(filepath, "rb+");

	int desp = pagina * tamanio_paginas + offset;

	fseek(file, desp, SEEK_SET);

	fwrite(text, sizeof(text), 1, file);



	fclose(file);

}



void escribir_en_swap(uint32_t indice_archivo_swap,pagina* pagina_a_escribir){
	//NECESITA EL FRAME SOLO O TDA LA PAG?
	char* c = pasar_a_char(indice_archivo_swap);
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Escribiendo en swap la pagina %d, en el archivo %s ... ",pagina_a_escribir->nro_pagina,c);
	//log_debug(log_memoria,"Escribiendo en swap el frame %d, en el archivo %s ...  ",pagina_a_escribir->frame,c);

	char *asd = string_new();
	string_append_with_format(&asd,"%s/",path_swap);
	string_append(&asd,c);

	int desp = 0;
	uint32_t contenido_pagina;
	char* char_contenido;


///////////////////////////////////////////////////////////////
//ESTE ES EL BUENO DESCOMENTAR
	//log_warning(log_memoria,"[EL PATH EN ESCRIBIR SWAP ES: %s]",asd);

	for(desp = 0; desp < tamanio_paginas ;desp+=4){
		contenido_pagina = leer_de_memoria(pagina_a_escribir->frame,desp);
		//log_debug(log_memoria,"El contenido que lei de memoria antes de escribirlo en swap es: %d ",contenido_pagina);


		char_contenido = pasar_a_char_sin_terminacion(contenido_pagina);
		//log_debug(log_memoria,"El char contenido es: %s",char_contenido);

		escribir_swap(asd,char_contenido,pagina_a_escribir->nro_pagina,desp);
	}

	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
	free(asd);
	free(c);

}




////////////////////
uint32_t lectura_swap(char*filepath,uint32_t pagina,int offset,uint32_t tamanio){

	int fd = open(filepath, O_RDONLY, (mode_t)0600);
	//log_info(log_memoria,"[RUTA DEL ARCHIVO DESPUES DEL OPEN %s]",filepath);

	//int fd = open("/home/utnso/tp-2022-1c-yaguarethreads-/memoria/0.swap",O_RDONLY);

	 ftruncate(fd,0);
	 ftruncate(fd,tamanio);




	  if (fd == -1){
	        perror("Error opening file for writing");
	       // exit(EXIT_FAILURE);
	    }


	    struct stat fileInfo = {0};


	    if (fstat(fd, &fileInfo) == -1){
	        perror("Error getting the file size");
	       // exit(EXIT_FAILURE);
	    }


	    if (fileInfo.st_size == 0){
	        fprintf(stderr, "Error: File is empty, nothing to do\n");
	     //   exit(EXIT_FAILURE);
	    }

	  //  printf("File size is %ji\n", (intmax_t)fileInfo.st_size);

	    char *map = mmap(NULL, tamanio, PROT_READ, MAP_FILE | MAP_SHARED , fd, 0);
	    if (map == MAP_FAILED)
	    {
	        close(fd);
	        perror("Error mmapping the file");
	      //  exit(EXIT_FAILURE);
	    }

	    char* valor_leido = NULL;

	    valor_leido = (char *)realloc(valor_leido, sizeof(valor_leido));

	    memcpy(valor_leido,map +pagina*tamanio_paginas +offset,sizeof(tamanio_paginas));

	  //  printf("Lei el valor %s\n",valor_leido);

	    // Don't forget to free the mmapped memory
	    if (munmap(map, fileInfo.st_size) == -1){
	        close(fd);
	        perror("Error un-mmapping the file");
	      //  exit(EXIT_FAILURE);
	    }

	    // Un-mmaping doesn't close the file, so we still need to do that.
	    close(fd);

	    uint32_t valor = atoi(valor_leido);

	    if(valor != 0){
		    //log_info(log_memoria,"Entre en leer memoria de mati: el valor leido del archivo %s es: %d",filepath,valor);
	    }

	    free(valor_leido);

	    return valor;
}

//////////////////

uint32_t leer_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina, uint32_t desp,uint32_t tamanio){
	//log_debug(log_memoria,"Trayendo pagina de swap...");

		//char* valor_leido;
		uint32_t valor_leido;// = 500;

		char* path = pasar_a_char(indice_archivo_swap);

		char *ruta_archivo = string_new();
		string_append_with_format(&ruta_archivo,"%s/",path_swap);
		string_append(&ruta_archivo,path);


		//log_info(log_memoria,"[RUTA DEL ARCHIVO EN LA FUNCION DE LEER DE SWAP %s]",ruta_archivo);


	//log_info(log_memoria,"El offset %d y el nro de pagna %d con el que voy a entrar a la funcion de mati en swap es",desp,nro_pagina);

	valor_leido = lectura_swap(ruta_archivo,nro_pagina,desp,tamanio);

	free(ruta_archivo);
	free(path);
	return valor_leido;
}

t_list* traer_pagina_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina,uint32_t tamanio){

	t_list* a = list_create();
	usleep(retardo_swap * 1000);

	log_info(log_memoria,"El nro de pagina que traigo de swap es: %d",nro_pagina);
	for(int desp = 0;desp < tamanio_paginas ; desp+=4){
		uint32_t contenido_de_pagina = leer_de_swap(indice_archivo_swap,nro_pagina,desp,tamanio);

    	//log_info(log_memoria,"El valor en la lista de valores que me vinieron de swap es: %d",contenido_de_pagina);

		list_add(a,contenido_de_pagina);

		//log_debug(log_memoria,"El size de la lista con los valores que tenia la pag(deberia ser 16): %d",list_size(a));
	}


	return a;
}


void traer_proceso_de_swap(uint32_t indice_archivo_swap,uint32_t tamanio, pagina* pagina_buscada,uint32_t frame_a_utilizar){
	//log_trace(log_memoria,"El indice archivo swap es: %d",indice_archivo_swap);
	//log_trace(log_memoria,"El size de la tabla global de 1er nuevel es %d", list_size(lista_tablas_1er_nivel));
	//log_trace(log_memoria,"El size de la tabla global de 2do nuevel es %d", list_size(lista_tablas_2do_nivel));


		pagina* pagina_aux = pagina_buscada;
		uint32_t dataAux;

		//log_trace(log_memoria,"El size de la lista paginas por proceso es: %d",list_size(paginas_del_proceso));



				pagina_aux->bit_uso = 1;
		//		log_trace(log_memoria,"Antes de escribir pagina");

				poner_pagina_en_marco(frame_a_utilizar,pagina_aux,indice_archivo_swap);
			//	log_info(log_memoria,"PUSE EL PROCESO(indice archivo) %d CUANDO LO TRAIGO DE SWAP EN EL FRAME %d",indice_archivo_swap,frame_a_escribir);
				usleep(retardo_swap * 1000);

				for(int desp = 0 ; desp < tamanio_paginas ; desp+=4){
					dataAux = leer_de_swap(indice_archivo_swap,pagina_aux->nro_pagina,desp,tamanio);

					if(dataAux != 0){
						log_warning(log_memoria,"El valor que lei de swap del archivo %d es: %d",indice_archivo_swap,dataAux);
						//log_warning(log_memoria,"El desplazamiento que lei de swap es: %d",desp);
					}

					escribir_pagina(dataAux,frame_a_utilizar,desp);

					leer_de_memoria(frame_a_utilizar,desp);
				}





//	// ir a swap y hacer fread y memcpy en mem principal
}


void pasar_proceso_a_swap(uint32_t indice_tabla){
	t_list * paginas_del_proceso;
	pagina* pagina_aux;

	paginas_del_proceso = buscar_paginas_proceso(indice_tabla);

	//log_warning(log_memoria,"EL SIZE DE LA LISTA PAGINAS POR PROCESO ES %d",list_size(paginas_del_proceso));
	for(int k=0;k < list_size(paginas_del_proceso);k++){
		pagina_aux = list_get(paginas_del_proceso,k);
		if(pagina_aux->bit_presencia == 1){
			pagina_aux->bit_presencia = 0;
			pagina_aux->bit_uso = 0;
			pagina_aux->bit_modificado = 0;
			escribir_en_swap(indice_tabla,pagina_aux);
			liberar_memoria(pagina_aux->frame);
		}

	}
}

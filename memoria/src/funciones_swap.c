#include "../include/comunicacion_memoria.h"



void crear_archivo_swap(uint32_t indice_tabla){ //TODO
//FILE* fp;
//char* a;
//itoa(a);
//fp = fopen(a,"r+w");
}

void escribir_swap(char* filepath,char* text ,int pagina,int offset){

	uint32_t tam_swap = 512; //TODO ESTA RE HARDCODEADO TIENE QUE RECIBIRLO ANTES CAMBIAR  AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA

	 printf("Will write text '%s'\n", text);

	    int fd = open(filepath, O_RDWR | O_CREAT, (mode_t)0600);

	    if (fd == -1)
	    {
	        perror("Error opening file for writing");
	        exit(EXIT_FAILURE);
	    }

	    ftruncate(fd,0);
	    ftruncate(fd,tam_swap);

	    // Stretch the file size to the size of the (mmapped) array of char

	    size_t textsize = strlen(text) + 1; // + \0 null character

	    if (lseek(fd, textsize-1, SEEK_SET) == -1)
	    {
	        close(fd);
	        perror("Error calling lseek() to 'stretch' the file");
	        exit(EXIT_FAILURE);
	    }

	    if (write(fd, "", 1) == -1)
	    {
	        close(fd);
	        perror("Error writing last byte of the file");
	        exit(EXIT_FAILURE);
	    }


	    // Now the file is ready to be mmapped.

	    char *map = mmap(NULL, tam_swap, PROT_READ | PROT_WRITE, MAP_FILE | MAP_SHARED , fd, 0);
	    if (map == MAP_FAILED)
	    {
	        close(fd);
	        perror("Error mmapping the file");
	        exit(EXIT_FAILURE);
	    }

	    memcpy(map +pagina*tamanio_paginas +offset,text,textsize);

	    // Write it now to disk
	    if (msync(map, textsize, MS_SYNC) == -1)
	    {
	        perror("Could not sync the file to disk");
	    }
	   // printf("direccion pagina 1: %p\n",map);

	    // Don't forget to free the mmapped memory
	    if (munmap(map, tam_swap) == -1)
	    {
	        close(fd);
	        perror("Error un-mmapping the file");
	        exit(EXIT_FAILURE);
	    }

	    // Un-mmaping doesn't close the file, so we still need to do that.
	    close(fd);


}

void escribir_en_swap(uint32_t indice_archivo_swap,pagina* pagina_a_escribir){
	//NECESITA EL FRAME SOLO O TDA LA PAG?
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Escribiendo en swap...");

	char* path = pasar_a_char(indice_archivo_swap);
	int desp = 0;
	uint32_t contenido_pagina;
	char* char_contenido;

	/*log_warning(log_memoria,"Frame a escribir : %d",pagina_a_escribir->frame);

	contenido_pagina = leer_de_memoria(pagina_a_escribir->frame,40);
	char_contenido = pasar_a_char_sin_terminacion(contenido_pagina);

	log_debug(log_memoria,"El contenido que lei de memoria antes de escribirlo en swap es: %d ",contenido_pagina);
	escribir_swap(path,char_contenido,pagina_a_escribir->nro_pagina,40);*/

	for(int i = 0;i < tamanio_paginas/4;i++){
		contenido_pagina = leer_de_memoria(pagina_a_escribir->frame,desp);
		log_debug(log_memoria,"El contenido que lei de memoria antes de escribirlo en swap es: %d ",contenido_pagina);
		char_contenido = pasar_a_char_sin_terminacion(contenido_pagina);
		log_debug(log_memoria,"El char contenido es: %s",char_contenido);
		escribir_swap(path,char_contenido,pagina_a_escribir->nro_pagina,desp);
		desp = desp + 4;
	}



	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
}


uint32_t leer_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina){
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Trayendo pagina de swap...");
	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
	return 500; // todo ir a leer a swap y devolver lo leido en vez de un 500
}


uint32_t traer_pagina_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina){
	uint32_t contenido_de_pagina = leer_de_swap(indice_archivo_swap,nro_pagina);
	return contenido_de_pagina;
}


void traer_proceso_de_swap(uint32_t indice_archivo_swap){
	log_trace(log_memoria,"El indice archivo swap es: %d",indice_archivo_swap);
	log_trace(log_memoria,"El size de la tabla global de 1er nuevel es %d", list_size(lista_tablas_1er_nivel));
	log_trace(log_memoria,"El size de la tabla global de 2do nuevel es %d", list_size(lista_tablas_2do_nivel));


		t_list* paginas_del_proceso = buscar_paginas_proceso(indice_archivo_swap);
		pagina* pagina_aux = malloc(sizeof(pagina));
		uint32_t dataAux;

		log_trace(log_memoria,"El size de la lista paginas por proceso es: %d",list_size(paginas_del_proceso));

		for(int k=0;k < marcos_por_proceso;k++){
			//log_trace(log_memoria,"Antes del list get del demonio");

			pagina_aux = list_get(paginas_del_proceso,k);
			dataAux = leer_de_swap(indice_archivo_swap,pagina_aux->nro_pagina);
			uint32_t frame_a_escribir = buscar_frame_libre();
			if(frame_a_escribir != -1){
				//pagina_aux->bit_presencia = 1;
				pagina_aux->bit_uso = 1;
				log_trace(log_memoria,"Antes de escribir pagina");

				poner_pagina_en_marco(frame_a_escribir,pagina_aux);
				log_info(log_memoria,"PUSE EL PROCESO CUANDO LO TRAIGO DE SWAP EN EL FRAME %d",frame_a_escribir);
				escribir_pagina(dataAux,frame_a_escribir,0);
			}
			else{
				log_trace(log_memoria,"Entre en el else, por ende ejecuto reemplazo");

				ejecutar_reemplazo(dataAux,pagina_aux,indice_archivo_swap);
			}

		}
	// ir a swap y hacer fread y memcpy en mem principal
}


void pasar_proceso_a_swap(uint32_t indice_tabla){
	t_list * paginas_del_proceso = list_create();
	pagina* pagina_aux = malloc(sizeof(pagina));
//	for(int i = 0;i < list_size(tabla_primer_nivel_buscada);i++){
//		entrada_primer_nivel_aux = list_get(tabla_primer_nivel_buscada,i);
//		tabla_segundo_nivel_aux = list_get(lista_tablas_2do_nivel,entrada_primer_nivel_aux);
//		for(int j = 0;j < list_size(tabla_segundo_nivel_aux);j++){
//			pagina_aux = list_get(tabla_segundo_nivel_aux,j);
//			list_add(paginas_del_proceso,pagina_aux);
//		}
//	}

	paginas_del_proceso = buscar_paginas_proceso(indice_tabla);

	//log_warning(log_memoria,"EL SIZE DE LA LISTA PAGINAS POR PROCESO ES %d",list_size(paginas_del_proceso));
	for(int k=0;k < list_size(paginas_del_proceso);k++){
		pagina_aux = list_get(paginas_del_proceso,k);
		if(pagina_aux->bit_presencia == 1){ //todo controlar si se modifica el bit cuando liberamos memoria
			pagina_aux->bit_presencia = 0;//
			escribir_en_swap(indice_tabla,pagina_aux);
			liberar_memoria(pagina_aux->frame);
		}

	sacar_proceso_de_memoria(indice_tabla);
		//log_warning(log_memoria,"El bit de presencia al pasar el proceso a swap es: %d",pagina_aux->bit_presencia);

	}
}

#include "../include/comunicacion_memoria.h"



void crear_archivo_swap(uint32_t indice_tabla){ //TODO
//FILE* fp;
//char* a;
//itoa(a);
//fp = fopen(a,"r+w");
}

void escribir_en_swap(uint32_t indice_archivo_swap,uint32_t frame){
	//NECESITA EL FRAME SOLO O TDA LA PAG?
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Escribiendo en swap...");
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
				escribir_pagina(dataAux,frame_a_escribir,indice_archivo_swap,0);
			}
			else{
				log_trace(log_memoria,"Entre en el else, por ende ejecuto reemplazo");

				ejecutar_reemplazo(dataAux,pagina_aux,indice_archivo_swap);
			}

		}
	// ir a swap y hacer fread y memcpy en mem principal
}


void pasar_proceso_a_swap(uint32_t indice_tabla){
	t_list * tabla_primer_nivel_buscada = list_get(lista_tablas_1er_nivel,indice_tabla);
	uint32_t entrada_primer_nivel_aux;
	t_list * tabla_segundo_nivel_aux = list_create();
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
			escribir_en_swap(indice_tabla,pagina_aux->frame);
			liberar_memoria(pagina_aux->frame);
		}
		//log_warning(log_memoria,"El bit de presencia al pasar el proceso a swap es: %d",pagina_aux->bit_presencia);

	}
}

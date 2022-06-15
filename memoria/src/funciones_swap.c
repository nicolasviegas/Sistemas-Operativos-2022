#include "../include/comunicacion_memoria.h"



void crear_archivo_swap(uint32_t indice_tabla){ //TODO
//FILE* fp;
//char* a;
//itoa(a);
//fp = fopen(a,"r+w");
}

void escribir_en_swap(uint32_t indice_archivo_swap,uint32_t frame){
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Escribiendo en swap...");
	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
}


uint32_t leer_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina){
	usleep(retardo_swap * 1000);
	log_debug(log_memoria,"Trayendo pagina de swap...");
	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
	return 1; // todo ir a leer a swap y devolver lo leido en vez de un 1
}


uint32_t traer_pagina_de_swap(uint32_t indice_archivo_swap,uint32_t nro_pagina){
	uint32_t contenido_de_pagina = leer_de_swap(indice_archivo_swap,nro_pagina);
	return contenido_de_pagina;
}


void traer_proceso_de_swap(uint32_t indice_archivo_swap){
	t_list * tabla_primer_nivel_buscada = list_get(lista_tablas_1er_nivel,indice_archivo_swap);
		uint32_t entrada_primer_nivel_aux;
		t_list * tabla_segundo_nivel_aux = list_create();
		t_list * paginas_del_proceso = list_create();
		pagina* pagina_aux = malloc(sizeof(pagina));
		uint32_t dataAux;
		for(int i = 0;i < list_size(tabla_primer_nivel_buscada);i++){
			entrada_primer_nivel_aux = list_get(tabla_primer_nivel_buscada,i);
			tabla_segundo_nivel_aux = list_get(lista_tablas_2do_nivel,entrada_primer_nivel_aux);
			for(int j = 0;j < list_size(tabla_segundo_nivel_aux);j++){
				pagina_aux = list_get(tabla_segundo_nivel_aux,j);
				//pagina_aux->bit_presencia = 0;//
				list_add(paginas_del_proceso,pagina_aux);
			}
		}
		for(int k=0;k < marcos_por_proceso;k++){
			pagina_aux = list_get(paginas_del_proceso,k);
			dataAux = leer_de_swap(indice_archivo_swap,pagina_aux->nro_pagina);
			uint32_t frame_a_escribir = buscar_frame_disponible();
			if(frame_a_escribir != -1){
				escribir_pagina(dataAux,frame_a_escribir,indice_archivo_swap,0);
			}
			else{
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
	for(int i = 0;i < list_size(tabla_primer_nivel_buscada);i++){
		entrada_primer_nivel_aux = list_get(tabla_primer_nivel_buscada,i);
		tabla_segundo_nivel_aux = list_get(lista_tablas_2do_nivel,entrada_primer_nivel_aux);
		for(int j = 0;j < list_size(tabla_segundo_nivel_aux);j++){
			pagina_aux = list_get(tabla_segundo_nivel_aux,j);
			pagina_aux->bit_presencia = 0;//
			list_add(paginas_del_proceso,pagina_aux);
		}
	}
	for(int k=0;k < list_size(paginas_del_proceso);k++){
		pagina_aux = list_get(paginas_del_proceso,k);
		if(pagina_aux->bit_presencia == 1){ //todo controlar si se modifica el bit cuando liberamos memoria
		escribir_en_swap(indice_tabla,pagina_aux->frame);
		liberar_memoria(pagina_aux->frame);
		}

	}
}

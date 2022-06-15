#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/string.h>
#include "utils.h"
#include <math.h>



uint32_t asignar_tabla_1er_nivel_a_proceso(t_list* tabla_1er_nivel){ // Devuelve el numero de tabla que se le asiga al proceso, o -1 si no hay mas lugar para entradas
	//t_list* nueva_entrada = list_create();
	uint32_t a;
	a = list_add(lista_tablas_1er_nivel,tabla_1er_nivel);
	//list_clean_and_destroy_elements(nueva_entrada,free);
	//list_destroy(nueva_entrada);
	return a;
}

uint32_t obtener_nro_tabla_2do_nivel(uint32_t numero_tabla_1er_nivel,uint32_t entrada_primer_nivel){
	t_list* tabla_1er_nivel_buscada = list_create();

	tabla_1er_nivel_buscada = list_get(lista_tablas_1er_nivel,numero_tabla_1er_nivel);
	uint32_t entrada_buscada;
	//log_debug(log_memoria,"Pase el list get de la tabla de primer nivel");
	entrada_buscada = list_get(tabla_1er_nivel_buscada,entrada_primer_nivel);
	log_debug(log_memoria,"LA entrada buscada es: %d",entrada_buscada);
	//list_clean_and_destroy_elements(tabla_1er_nivel_buscada,&free);todo VER COMO HACER ESTE LIST AND DESTROY
	list_destroy(tabla_1er_nivel_buscada);
	//log_debug(log_memoria,"Pase el list destroy en obtener nro tabla 2do nivel");

	return entrada_buscada;

}


pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_entrada){ // Busca la pagina en la tabla de 2do nivel, si no esta, devuelve null
	t_list* tabla_2do_nivel_buscada = list_create();
	tabla_2do_nivel_buscada = list_get(lista_tablas_2do_nivel,nro_tabla_2do_nivel);
	log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_tabla_2do_nivel);
	log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_entrada);
	pagina* pagina_buscada = list_get(tabla_2do_nivel_buscada,nro_entrada);
	//list_clean_and_destroy_elements(tabla_2do_nivel_buscada,free);
	list_destroy(tabla_2do_nivel_buscada);
	return pagina_buscada;
}

t_list* dividir_proceso_en_paginas(uint32_t tam_proceso){
	float a = (float)tam_proceso/(float)tamanio_paginas;
	uint32_t cant_pags = ceil(a);

	log_debug(log_memoria,"El float es: %f",a);
	t_list* lista_con_todas_las_paginas = list_create();


	for(int i=0; i< cant_pags;i++){
		pagina* pagina_nueva = malloc(sizeof(pagina));
		pagina_nueva->nro_pagina = i;
		pagina_nueva->bit_modificado = 0;
		pagina_nueva->bit_presencia = 0;
		pagina_nueva->bit_uso = 0;
		pagina_nueva->frame = 0;
		//pagina_nueva->tiempo_uso = 0;

		list_add(lista_con_todas_las_paginas,pagina_nueva);
	}

	return lista_con_todas_las_paginas;
}

t_list* colocar_paginas_en_tabla(t_list* lista_paginas_del_proceso){ //esta funcion carga la tabla de primer nivel con numeritos y la de seg nivel con lista de paginas
	t_list* tabla_de_1er_nivel = list_create();
	t_list* lista_aux = list_create();
	t_list* lista_aux2 = list_create();

	while(list_size(lista_paginas_del_proceso) != 0){
		//log_warning(log_memoria,"El size de la lista dentro del for en antes del if es: %d",list_size(lista_paginas_del_proceso));

		if(list_size(lista_paginas_del_proceso) >= cant_entradas_por_tabla){

			lista_aux = list_take_and_remove(lista_paginas_del_proceso,cant_entradas_por_tabla);

		//	log_warning(log_memoria,"El size de la lista dentro del for en el if es: %d",list_size(lista_paginas_del_proceso));

			uint32_t indice = list_add(lista_tablas_2do_nivel,lista_aux);

			list_add(tabla_de_1er_nivel,indice);

		}else{
			lista_aux2 = list_take_and_remove(lista_paginas_del_proceso,list_size(lista_paginas_del_proceso));

			// log_warning(log_memoria,"El size de la lista dentro del for en el else es: %d",list_size(lista_paginas_del_proceso));

			uint32_t indice = list_add(lista_tablas_2do_nivel,lista_aux2);

			list_add(tabla_de_1er_nivel,indice);
		}

	}

	log_debug(log_memoria,"El size de la lista de tablas de 2do nivel es %d",list_size(lista_tablas_2do_nivel));


//	list_clean_and_destroy_elements(lista_aux,free);
//	list_destroy(lista_aux);
//	list_clean_and_destroy_elements(lista_aux2,free);
//	list_destroy(lista_aux2);
	log_debug(log_memoria,"El size de la lista de tablas de 2do nivel despues de liberar es %d",list_size(lista_tablas_2do_nivel));
	return tabla_de_1er_nivel;
}




t_list* buscar_paginas_proceso(uint32_t indice_tabla_1er_nivel){
	t_list * tabla_primer_nivel_buscada = list_get(lista_tablas_1er_nivel,indice_tabla_1er_nivel);
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
	return paginas_del_proceso;
}


t_list* buscar_paginas_proceso_en_mem_ppal(t_list* paginas_proceso) {
	t_list* paginas_del_proceso_en_mem_ppal = list_create();
	pagina* pagina_aux = malloc(sizeof(pagina));
	for(int i = 0;i < list_size(paginas_proceso);i++){
		pagina_aux = list_get(paginas_proceso,i);
		if(pagina_aux->bit_presencia == 1){
			list_add(paginas_del_proceso_en_mem_ppal,pagina_aux);
		}
	}
	return paginas_del_proceso_en_mem_ppal;
}



pagina* pagina_a_reemplazar(uint32_t indice_tabla_1er_nivel) {

	t_list* paginas_proceso = list_create();
	t_list* paginas_proceso_en_mem_ppal = list_create();



	paginas_proceso = buscar_paginas_proceso(indice_tabla_1er_nivel);
	paginas_proceso_en_mem_ppal = buscar_paginas_proceso_en_mem_ppal(paginas_proceso);

	if(algoritmo_config == CLOCK_M)
	{


		pagina* recorredorPaginas;
		int cantidadFrames = list_size(paginas_proceso_en_mem_ppal);

		//esta es la primera vuelta para encontrar 0|0
		for(int i = 0; i < cantidadFrames ; i++){
			if(punteroClock == cantidadFrames)
			{
				punteroClock = 0;
			}

			recorredorPaginas = list_get(paginas_proceso_en_mem_ppal, punteroClock);
			punteroClock++;

			if(recorredorPaginas->bit_uso == 0 && recorredorPaginas->bit_modificado == 0 ){
				log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
				log_warning(log_memoria,"Victima CLOCK-M: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
				return recorredorPaginas;
			}

		}

		//esta segunda vuelta es para encontrar 0|1 modificando el bit de uso
		for(int i = 0; i < cantidadFrames ; i++){
			if(punteroClock == cantidadFrames)
			{
				punteroClock = 0;
			}

			recorredorPaginas = list_get(paginas_proceso_en_mem_ppal, punteroClock);
			punteroClock++;

			if(recorredorPaginas->bit_uso == 0 && recorredorPaginas->bit_modificado == 1 ){
				log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
				log_warning(log_memoria,"Victima CLOCK-M: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
				return recorredorPaginas;
			}

			recorredorPaginas->bit_uso = 0;

		}

		//esta tercera vuelta es para encontrar 0|0
		for(int i = 0; i < cantidadFrames ; i++){
			if(punteroClock == cantidadFrames)
			{
				punteroClock = 0;
			}

			recorredorPaginas = list_get(paginas_proceso_en_mem_ppal, punteroClock);
			punteroClock++;

			if(recorredorPaginas->bit_uso == 0 && recorredorPaginas->bit_modificado == 0 ){
				log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
				log_warning(log_memoria,"Victima CLOCK-M: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
				return recorredorPaginas;
			}

		}

		//esta segunda vuelta es para encontrar 0|1
		for(int i = 0; i < cantidadFrames ; i++){
			if(punteroClock == cantidadFrames)
			{
				punteroClock = 0;
			}

			recorredorPaginas = list_get(paginas_proceso_en_mem_ppal, punteroClock);
			punteroClock++;

			if(recorredorPaginas->bit_uso == 0 && recorredorPaginas->bit_modificado == 1 ){
				log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
				log_warning(log_memoria,"Victima CLOCK-M: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
				return recorredorPaginas;
			}
		}
	}

	//list_destroy(paginas_proceso);

}

void cargar_lista_frames(){
	int cantidad_marcos = tamanio_memoria / tamanio_paginas;
	frame* marco;
	for(int i=0;i < cantidad_marcos;i++){
		marco->nro_pagina = 0;
		marco->ocupado = 0;
		list_add(lista_frames,marco);
	}
	log_trace(log_memoria,"Se genero la lista de marcos");
}


void liberar_memoria(uint32_t frame){
	//liberar desde el (frame * tam_paginas) hasta (frame + 1 * tam_paginas)
}


void escribir_en_memoria(uint32_t valor,pagina* pagina,uint32_t indice_tabla_1er_nivel){
	//ejecu
	//escribe en memoria la data y pone en 1 el bit de presencia de la pagina
}


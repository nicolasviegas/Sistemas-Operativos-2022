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
	t_list* tabla_1er_nivel_buscada = list_get(lista_tablas_1er_nivel,numero_tabla_1er_nivel);
	uint32_t entrada_buscada;
	//log_debug(log_memoria,"Pase el list get de la tabla de primer nivel");
	entrada_buscada = list_get(tabla_1er_nivel_buscada,entrada_primer_nivel);
	//log_debug(log_memoria,"LA entrada buscada es: %d",entrada_buscada);
	//list_clean_and_destroy_elements(tabla_1er_nivel_buscada,&free);todo VER COMO HACER ESTE LIST AND DESTROY
	//list_destroy(tabla_1er_nivel_buscada);///////////////////////////////////////////////////////////TODO ESTE DESTROY HACE CAGADA
	//log_debug(log_memoria,"Pase el list destroy en obtener nro tabla 2do nivel");

	return entrada_buscada;

}


pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_entrada){ // Busca la pagina en la tabla de 2do nivel, si no esta, devuelve null
	t_list* tabla_2do_nivel_buscada = list_get(lista_tablas_2do_nivel,nro_tabla_2do_nivel);
	//log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_tabla_2do_nivel);
	//log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_entrada);
	pagina* pagina_buscada = list_get(tabla_2do_nivel_buscada,nro_entrada);
	//list_clean_and_destroy_elements(tabla_2do_nivel_buscada,free);
	//list_destroy(tabla_2do_nivel_buscada);
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
	//log_debug(log_memoria,"El size de la lista de tablas de 2do nivel despues de liberar es %d",list_size(lista_tablas_2do_nivel));
	return tabla_de_1er_nivel;
}




t_list* buscar_paginas_proceso(uint32_t indice_tabla_1er_nivel){
			t_list * tabla_primer_nivel_buscada = list_create();
			tabla_primer_nivel_buscada = list_get(lista_tablas_1er_nivel,indice_tabla_1er_nivel);
			uint32_t entrada_primer_nivel_aux;
			t_list * tabla_segundo_nivel_aux = list_create();
			t_list * paginas_del_proceso = list_create();
			pagina* pagina_aux = malloc(sizeof(pagina));
			uint32_t dataAux;

			log_debug(log_memoria,"EL SIZE DE LA LISTA  TABLA PRIMER NIVEL EN  BUSCAR PAGINAS PROCESO ES: %d",list_size(tabla_primer_nivel_buscada));
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
	log_trace(log_memoria,"Cant marcos seran %d",cantidad_marcos);
	for(int i=0;i < cantidad_marcos;i++){
		frame* marco = malloc(sizeof(frame));
		marco->nro_pagina = 0;
		marco->ocupado = false;
		list_add(lista_frames,marco);
	}
	log_trace(log_memoria,"Se genero la lista de marcos, con %d marcos",list_size(lista_frames));

}




void liberar_memoria(uint32_t marco1){//liberamos la memoria posta y ponemos el marco ocmo desocupado
	//liberar desde el (frame * tam_paginas) hasta (frame + 1 * tam_paginas)
	frame* frame_aux = list_get(lista_frames,marco1);
	frame_aux->ocupado = false;
	//todo liberar memoria posta
}


void escribir_pagina(uint32_t valor,uint32_t frame, uint32_t desplazamiento){// TODO VER COMO SE ESCIBE EN MEMORIA
	log_error(log_memoria,"Entre a escribir pagina,valor: %d ",valor);
	uint32_t posicion_marco = frame * tamanio_paginas;
	//log_error(log_memoria,"El frame es es %d ",frame);
	//log_error(log_memoria,"El tam pagina %d ",tamanio_paginas);
	memcpy(memoria_principal+posicion_marco+desplazamiento,&valor,sizeof(uint32_t));
	//memcpy(memoria_principal+posicion_marco+desplazamiento,valor,tamanio_paginas);
	//escribe en memoria la data y pone en 1 el bit de presencia de la pagina
}

uint32_t leer_de_memoria(uint32_t frame,uint32_t desplazamiento){ // TODO LEER DE MEMORIA
	uint32_t valor_leido;
	uint32_t posicion_marco = frame * tamanio_paginas;
	memcpy(&valor_leido,memoria_principal+posicion_marco+desplazamiento,sizeof(uint32_t));
	log_error(log_memoria,"El valor despues de leer en memoria %d ",valor_leido);
	return valor_leido;
}

uint32_t buscar_frame_libre(){
	//log_error(log_memoria,"Entre a buscar frame libre");
	frame* frameAux;
	frame* frame_libre;
	for(int i=0;i < list_size(lista_frames);i++){
		frameAux = list_get(lista_frames,i);
		if(!frameAux->ocupado){
			return i;
		}
	}
	return -1;
}



void ejecutar_reemplazo(uint32_t valor, pagina* info_pagina,uint32_t indice_pagina_1er_nivel) {

    pagina* info_paginaAReemplazar =  pagina_a_reemplazar(indice_pagina_1er_nivel);



    uint32_t frame = info_paginaAReemplazar->frame;
    uint32_t  biteme = info_paginaAReemplazar->bit_modificado;
    info_paginaAReemplazar->bit_presencia = 0;
//
//    if(!send_pag_swamp(socketSwamp,info_paginaAReemplazar, carpincho->pid)){
//        return false;
//    }
//
//    if(biteme != 0){
//        MEM_SWAP_MESSAGE respuesta;
//        recv(socketSwamp, &respuesta, sizeof(MEM_SWAP_MESSAGE), 0);
//
//        if(respuesta != PAGE_ADDED){
//            return false;
//        }
//    }

    escribir_en_swap(indice_pagina_1er_nivel,info_paginaAReemplazar->frame);

    //GUARDAMOS EN RAM LO QUE SE QUIERE USAR
    info_pagina->frame = frame;
    info_pagina->bit_presencia = 1;
    info_pagina->bit_uso = 1;
    //info_pagina->tiempo_uso = obtener_tiempo();

    log_error(log_memoria,"Antes de escribir pagina el frame es: %d",info_pagina->frame);
    escribir_pagina(valor,info_pagina->frame,0);
    poner_pagina_en_marco(frame,info_pagina);

}



bool el_proceso_tiene_almenos_una_pag_en_mem(uint32_t indice_tabla_1er_nivel){
	t_list* paginas_del_proceso = list_create();
	paginas_del_proceso = buscar_paginas_proceso(indice_tabla_1er_nivel);
	t_list* paginas_del_proceso_en_mem = list_create();
	paginas_del_proceso_en_mem = buscar_paginas_proceso_en_mem_ppal(paginas_del_proceso);
	log_error(log_memoria,"El proceso tiene %d paginas en memoria",list_size(paginas_del_proceso_en_mem));
	if(list_size(paginas_del_proceso_en_mem) > 0 ){
		return true;
	}
	return false;
}

bool al_proceso_le_quedan_frames(uint32_t indice_tabla_1er_nivel){
	t_list* paginas_del_proceso = list_create();
	paginas_del_proceso = buscar_paginas_proceso(indice_tabla_1er_nivel);
	t_list* paginas_del_proceso_en_mem = list_create();
	paginas_del_proceso_en_mem = buscar_paginas_proceso_en_mem_ppal(paginas_del_proceso);
	if(list_size(paginas_del_proceso_en_mem) < marcos_por_proceso){
		return true;
	}
	return false;
}

void poner_pagina_en_marco(uint32_t marco,pagina* pagina){
	//log_error(log_memoria,"Entre en poner pagina en marco la re concha de tu madre");
	frame* frame_buscado = list_get(lista_frames,marco);
	frame_buscado->nro_pagina = pagina->nro_pagina;
	frame_buscado->ocupado = true;
	pagina->frame = marco;
	pagina->bit_presencia = 1;
}

void poner_proceso_en_mem_ppal(uint32_t indice_proceso){
	pagina* paginaAux = malloc(sizeof(pagina));
	//frame* marcoAux = malloc(sizeof(frame));
	uint32_t marcoAux;
	t_list* paginas_del_proceso = list_create();
	paginas_del_proceso = buscar_paginas_proceso(indice_proceso);
	t_list* paginas_del_proceso_para_mem = list_create();
	if(list_size(paginas_del_proceso) > marcos_por_proceso){
		paginas_del_proceso_para_mem = list_take(paginas_del_proceso,marcos_por_proceso);
	}
	else{
		paginas_del_proceso_para_mem = list_take(paginas_del_proceso,list_size(paginas_del_proceso));
	}

	//log_debug(log_memoria,"El size de LA LISTA EN PONER PROCESO EN MEM PRICNCIPAL ES %d", list_size(paginas_del_proceso_para_mem));

	for(int i=0;i<list_size(paginas_del_proceso_para_mem);i++){
		paginaAux = list_get(paginas_del_proceso_para_mem,i);
		marcoAux = buscar_frame_libre();
		poner_pagina_en_marco(marcoAux,paginaAux);

		log_debug(log_memoria,"El frame donde voy a poner al proceso en mem ppal:  %d",paginaAux->frame);
	}



}

void sacar_pagina_de_marco(pagina* pagina_aux){
	frame* frame_aux = malloc(sizeof(frame));

	for(int i = 0;i<list_size(lista_frames);i++){
		frame_aux = list_get(lista_frames,i);
		if(frame_aux->nro_pagina == pagina_aux->nro_pagina){
			//log_debug(log_memoria,"El frame a liberar es:  %d",i);

			frame_aux->ocupado = false;
		}
	}
}


void sacar_proceso_de_memoria(uint32_t indice_proceso){
	t_list* lista_aux1 = list_create();
	t_list* lista_aux2 = list_create();

	lista_aux1 = buscar_paginas_proceso(indice_proceso);
	lista_aux2 = buscar_paginas_proceso_en_mem_ppal(lista_aux1);

	pagina* pagina_aux = malloc(sizeof(pagina));
	for(int i = 0; i < list_size(lista_aux2);i++){
		pagina_aux = list_get(lista_aux2,i);
		sacar_pagina_de_marco(pagina_aux);
	}

}

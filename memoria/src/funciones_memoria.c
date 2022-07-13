#include "../include/memoria.h"
#include <stdio.h>
#include <stdlib.h>
#include<commons/collections/list.h>
#include<commons/collections/queue.h>
#include<commons/string.h>
#include "utils.h"
#include <math.h>



uint32_t asignar_tabla_1er_nivel_a_proceso(t_list* tabla_1er_nivel){ // Devuelve el numero de tabla que se le asiga al proceso, o -1 si no hay mas lugar para entradas
	uint32_t a;
	a = list_add(lista_tablas_1er_nivel,tabla_1er_nivel);
	return a;
}

uint32_t obtener_nro_tabla_2do_nivel(uint32_t numero_tabla_1er_nivel,uint32_t entrada_primer_nivel){
	t_list* tabla_1er_nivel_buscada = list_get(lista_tablas_1er_nivel,numero_tabla_1er_nivel);
	uint32_t entrada_buscada;
	entrada_buscada = list_get(tabla_1er_nivel_buscada,entrada_primer_nivel);

	return entrada_buscada;

}


pagina* buscar_pagina_en_tabla_2do_nivel(uint32_t nro_tabla_2do_nivel,uint32_t nro_entrada){ // Busca la pagina en la tabla de 2do nivel, si no esta, devuelve null
	t_list* tabla_2do_nivel_buscada = list_get(lista_tablas_2do_nivel,nro_tabla_2do_nivel);
	//log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_tabla_2do_nivel);
	//log_debug(log_memoria,"El indice de la tabla de paginas de segundo nivel en la global es: %d",nro_entrada);
	pagina* pagina_buscada = list_get(tabla_2do_nivel_buscada,nro_entrada);
	return pagina_buscada;
}

t_list* dividir_proceso_en_paginas(uint32_t tam_proceso){
	float a = (float)tam_proceso/(float)tamanio_paginas;
	uint32_t cant_pags = ceil(a);

	//log_debug(log_memoria,"El float es: %f",a);
	t_list* lista_con_todas_las_paginas = list_create();


	for(int i=0; i< cant_pags;i++){
		pagina* pagina_nueva = malloc(sizeof(pagina));
		pagina_nueva->nro_pagina = i;
		pagina_nueva->bit_modificado = 0;
		pagina_nueva->bit_presencia = 0;
		pagina_nueva->bit_uso = 0;
		pagina_nueva->frame = 0;

		list_add(lista_con_todas_las_paginas,pagina_nueva);
	}

	return lista_con_todas_las_paginas;
}

t_list* colocar_paginas_en_tabla(t_list* lista_paginas_del_proceso){ //esta funcion carga la tabla de primer nivel con numeritos y la de seg nivel con lista de paginas
	t_list* tabla_de_1er_nivel = list_create();
	t_list* lista_aux;
	t_list* lista_aux2;

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


	list_destroy(lista_paginas_del_proceso);

	return tabla_de_1er_nivel;
}




t_list* buscar_paginas_proceso(uint32_t indice_tabla_1er_nivel){
			t_list * tabla_primer_nivel_buscada;
			tabla_primer_nivel_buscada = list_get(lista_tablas_1er_nivel,indice_tabla_1er_nivel);
			uint32_t entrada_primer_nivel_aux;
			t_list * tabla_segundo_nivel_aux;
			t_list * paginas_del_proceso = list_create();
			pagina* pagina_aux;
			uint32_t dataAux;

		//	log_debug(log_memoria,"EL SIZE DE LA LISTA  TABLA PRIMER NIVEL EN  BUSCAR PAGINAS PROCESO ES: %d",list_size(tabla_primer_nivel_buscada));
			for(int i = 0;i < list_size(tabla_primer_nivel_buscada);i++){
				entrada_primer_nivel_aux = list_get(tabla_primer_nivel_buscada,i);
				tabla_segundo_nivel_aux = list_get(lista_tablas_2do_nivel,entrada_primer_nivel_aux);
				for(int j = 0;j < list_size(tabla_segundo_nivel_aux);j++){
					pagina_aux = list_get(tabla_segundo_nivel_aux,j);
					list_add(paginas_del_proceso,pagina_aux);
				}
			}
	return paginas_del_proceso;
}



t_list* buscar_paginas_proceso_en_mem_ppal(t_list* paginas_proceso) {
	t_list* paginas_del_proceso_en_mem_ppal = list_create();
	pagina* pagina_aux;
	for(int i = 0;i < list_size(paginas_proceso);i++){
		pagina_aux = list_get(paginas_proceso,i);
		if(pagina_aux->bit_presencia == 1){
			list_add(paginas_del_proceso_en_mem_ppal,pagina_aux);
		}
	}
	return paginas_del_proceso_en_mem_ppal;
}



pagina* pagina_a_reemplazar(uint32_t indice_tabla_1er_nivel) {

	t_list* paginas_proceso;
	t_list* paginas_proceso_en_mem_ppal;



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
				//log_warning(log_memoria,"ENTRE POR EL 0 | 0");

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
				//log_warning(log_memoria,"ENTRE POR EL 0 | 1");
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
				//log_warning(log_memoria,"ENTRE POR EL 0 | 0 EN LA TERCER VUELTA");
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
			//	log_warning(log_memoria,"ENTRE POR EL 0 | 1 EN LA CUARTA VUELTA");
				return recorredorPaginas;
			}
		}
	}
	else{// CLOCK NORMAL
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

					if(recorredorPaginas->bit_uso == 0){
						log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
						log_warning(log_memoria,"Victima CLOCK: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
						return recorredorPaginas;
					}
				}

				//esta segunda vuelta es para encontrar 0 modificando el bit de uso
				for(int i = 0; i < cantidadFrames ; i++){
					if(punteroClock == cantidadFrames)
					{
						punteroClock = 0;
					}

					recorredorPaginas = list_get(paginas_proceso_en_mem_ppal, punteroClock);
					punteroClock++;

					if(recorredorPaginas->bit_uso == 0){
						log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
						log_warning(log_memoria,"Victima CLOCK: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
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

					if(recorredorPaginas->bit_uso == 0){
						log_warning(log_memoria,"Indice de la tabla 1er nivel de la pagina a meter: %d", indice_tabla_1er_nivel);
						log_warning(log_memoria,"Victima CLOCK: pagina:%d - frame:%d \n", recorredorPaginas->nro_pagina, recorredorPaginas->frame);
						return recorredorPaginas;
					}

				}
			}

	}

	list_destroy(paginas_proceso);
	list_destroy(paginas_proceso_en_mem_ppal);

}

void cargar_lista_frames(){
	int cantidad_marcos = tamanio_memoria / tamanio_paginas;
	//log_trace(log_memoria,"Cant marcos seran %d",cantidad_marcos);
	for(int i=0;i < cantidad_marcos;i++){
		frame* marco = malloc(sizeof(frame)); // no se libera?
		marco->nro_pagina = -1;
		marco->ocupado = false;
		marco->indice_proceso = -1;
		pthread_mutex_lock(&mutexListaFrame);
		list_add(lista_frames,marco);
		pthread_mutex_unlock(&mutexListaFrame);
	}
	//log_trace(log_memoria,"Se genero la lista de marcos, con %d marcos",list_size(lista_frames));

}




void liberar_memoria(uint32_t marco1){//liberamos la memoria posta y ponemos el marco ocmo desocupado
	//liberar desde el (frame * tam_paginas) hasta (frame + 1 * tam_paginas)
	pthread_mutex_lock(&mutexListaFrame);
	frame* frame_aux = list_get(lista_frames,marco1);
	frame_aux->ocupado = false;
	frame_aux->nro_pagina = -1;
	frame_aux->indice_proceso = -1;
	//log_warning(log_memoria,"//////////////////MIRAME MIRAME SE PUSO UN FRAME (el %d) COMO LIBREEEEE//////////////////",marco1);
	pthread_mutex_unlock(&mutexListaFrame);

	//int desp = 0;
	for(int desp = 0;desp < tamanio_paginas;desp+=4){
		escribir_pagina(0,marco1,desp);
		//desp+=4;
	}
}


void escribir_pagina(uint32_t valor,uint32_t frame, uint32_t desplazamiento){
	pthread_mutex_lock(&mutexEscribirEnMemoria);

	if(valor != 0){
		log_info(log_memoria,"Escribo en el frame %d,el valor: %d ",frame,valor);

	}

	uint32_t posicion_marco = frame * tamanio_paginas;

	memcpy(memoria_principal+posicion_marco+desplazamiento,&valor,sizeof(uint32_t));
	pthread_mutex_unlock(&mutexEscribirEnMemoria);
}

uint32_t leer_de_memoria(uint32_t frame,uint32_t desplazamiento){
	uint32_t valor_leido;
	uint32_t posicion_marco = frame * tamanio_paginas+desplazamiento;

	//log_error(log_memoria,"dentro de leer de memoria %d ",valor_leido);



	memcpy(&valor_leido,memoria_principal+posicion_marco,sizeof(uint32_t));
//	if(valor_leido != 0){
//		log_error(log_memoria,"El valor que leo en memoria principal %d con posicion marco: %d",valor_leido,posicion_marco);
//	}
	return valor_leido;
}


void copiar_en_memoria(uint32_t marco_origen,uint32_t desplazamiento_origen,uint32_t marco_destino,uint32_t desplazamiento_destino){

	uint32_t valor_leido = leer_de_memoria(marco_origen,desplazamiento_origen);
	escribir_pagina(valor_leido,marco_destino,desplazamiento_destino);

}


uint32_t buscar_frame_libre(){
	//log_error(log_memoria,"Entre a buscar frame libre");
	frame* frameAux;
	pthread_mutex_lock(&mutexListaFrame);
	for(int i=0;i < list_size(lista_frames);i++){
		frameAux = list_get(lista_frames,i);
//			if(frameAux->ocupado){
//				log_info(log_memoria,"*******************************EL FRAME %d QUE ACABO DE BUSCAR ESTA OCUPADO",i);
//
//				}else{
//				log_info(log_memoria,"-------------------------------------EL FRAME %d QUE ACABO DE BUSCAR ESTA LIBRE",i);
//
//				}
		//log_warning(log_memoria,"Estoy pensando....");
		if(!frameAux->ocupado){

			if(frameAux->ocupado){
				//log_info(log_memoria,"*******************************EL FRAME  QUE ACABO DE ELEGIR %d ESTA OCUPADO", i );

			}else{
				//log_info(log_memoria,"-------------------------------------EL FRAME QUE ACABO DE ELEGIR %d ESTA LIBRE",i);

			}



			pthread_mutex_unlock(&mutexListaFrame);
			return i;

		}

	}
	pthread_mutex_unlock(&mutexListaFrame);

	return -1;
}



void ejecutar_reemplazo(t_list* lista_valores, pagina* info_pagina,uint32_t indice_pagina_1er_nivel) {

    pagina* info_paginaAReemplazar =  pagina_a_reemplazar(indice_pagina_1er_nivel);



    uint32_t frame = info_paginaAReemplazar->frame;
    uint32_t  biteme = info_paginaAReemplazar->bit_modificado;
    info_paginaAReemplazar->bit_presencia = 0;

    //log_error(log_memoria,"La pagina a reemplazar es: %d",info_paginaAReemplazar->nro_pagina);

    if(info_paginaAReemplazar->bit_modificado == 1){
    	escribir_en_swap(indice_pagina_1er_nivel,info_paginaAReemplazar);
    }


    //GUARDAMOS EN RAM LO QUE SE QUIERE USAR
    info_pagina->frame = frame;
    info_pagina->bit_presencia = 1;
    info_pagina->bit_uso = 1;
    //info_pagina->tiempo_uso = obtener_tiempo();

    //log_error(log_memoria,"Antes de escribir pagina el frame es: %d",info_pagina->frame);
    uint32_t valor;
    uint32_t desp = 0;
    for(int i = 0 ; i < list_size(lista_valores);i++){
    	valor = list_get(lista_valores,i);
    	escribir_pagina(valor,info_pagina->frame,desp);
    	desp+=4;

    }
    poner_pagina_en_marco(frame,info_pagina,indice_pagina_1er_nivel);

}



bool el_proceso_tiene_almenos_una_pag_en_mem(uint32_t indice_tabla_1er_nivel){
	t_list* paginas_del_proceso;
	paginas_del_proceso = buscar_paginas_proceso(indice_tabla_1er_nivel);
	t_list* paginas_del_proceso_en_mem;
	paginas_del_proceso_en_mem = buscar_paginas_proceso_en_mem_ppal(paginas_del_proceso);
	//log_error(log_memoria,"El proceso %d tiene %d paginas en memoria",indice_tabla_1er_nivel,list_size(paginas_del_proceso_en_mem));
	if(list_size(paginas_del_proceso_en_mem) > 0 ){
		return true;
	}
	list_destroy(paginas_del_proceso);
	list_destroy(paginas_del_proceso_en_mem);
	return false;
}

bool al_proceso_le_quedan_frames(uint32_t indice_tabla_1er_nivel){
	t_list* paginas_del_proceso;
	paginas_del_proceso = buscar_paginas_proceso(indice_tabla_1er_nivel);
	t_list* paginas_del_proceso_en_mem;
	paginas_del_proceso_en_mem = buscar_paginas_proceso_en_mem_ppal(paginas_del_proceso);
	if(list_size(paginas_del_proceso_en_mem) < marcos_por_proceso){
		return true;
	}
	list_destroy(paginas_del_proceso);
	list_destroy(paginas_del_proceso_en_mem);
	return false;
}

void poner_pagina_en_marco(uint32_t marco,pagina* pagina,uint32_t indice_proceso){
	//log_error(log_memoria,"Entre en poner pagina en marco la re concha de tu madre");
	pthread_mutex_lock(&mutexListaFrame);

	frame* frame_buscado = list_get(lista_frames,marco);
	frame_buscado->nro_pagina = pagina->nro_pagina;
	frame_buscado->indice_proceso = indice_proceso;
	frame_buscado->ocupado = true;
	pthread_mutex_unlock(&mutexListaFrame);

	pagina->frame = marco;
	pagina->bit_presencia = 1;


}

void poner_proceso_en_mem_ppal(uint32_t indice_proceso){
	pagina* paginaAux;
	uint32_t marcoAux;
	t_list* paginas_del_proceso;
	paginas_del_proceso = buscar_paginas_proceso(indice_proceso);
	t_list* paginas_del_proceso_para_mem;
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
		log_trace(log_memoria,"El marco que se eligio es: %d",marcoAux);

		poner_pagina_en_marco(marcoAux,paginaAux,indice_proceso);

		log_debug(log_memoria,"El frame donde voy a poner al proceso en mem ppal:  %d",paginaAux->frame);
	}

	list_destroy(paginas_del_proceso);
	list_destroy(paginas_del_proceso_para_mem);

}

void sacar_pagina_de_marco(pagina* pagina_aux,uint32_t indice_proceso){
	frame* frame_aux = (sizeof(frame));
	//log_debug(log_memoria,"El numero de pagina que se compara con el frame es %d",pagina_aux->nro_pagina);
	for(int i = 0;i<list_size(lista_frames);i++){
		pthread_mutex_lock(&mutexListaFrame);

		frame_aux = list_get(lista_frames,i);
		if(frame_aux->nro_pagina == pagina_aux->nro_pagina && frame_aux->indice_proceso == indice_proceso){
			//log_debug(log_memoria,"El frame a liberar es:  %d",i);

			frame_aux->ocupado = false;
			for(int desp = 0;desp<tamanio_paginas;desp+=4){
				escribir_pagina(0,i,desp);
			}
			//log_warning(log_memoria,"//////////////////MIRAME MIRAME SE PUSO UN FRAME (el %d) COMO LIBREEEEE, con pagina %d //////////////////",i,pagina_aux->nro_pagina);
		}
		pthread_mutex_unlock(&mutexListaFrame);

	}
}


void sacar_proceso_de_memoria(uint32_t indice_proceso){
	t_list* lista_aux1;
	t_list* lista_aux2;

	lista_aux1 = buscar_paginas_proceso(indice_proceso);
	lista_aux2 = buscar_paginas_proceso_en_mem_ppal(lista_aux1);

	pagina* pagina_aux;
	for(int i = 0; i < list_size(lista_aux2);i++){
		pagina_aux = list_get(lista_aux2,i);
		sacar_pagina_de_marco(pagina_aux,indice_proceso);
	}

}


void actualizar_bit_uso_tlb(uint32_t marco_aux){
	t_list* listaAux;
	t_list* listaAux2;
	pagina* paginaAux;
	for(int i=0;i<list_size(lista_tablas_2do_nivel);i++){
		listaAux = list_get(lista_tablas_2do_nivel,i);
		listaAux2 = buscar_paginas_proceso_en_mem_ppal(listaAux);
		for(int j=0;j<list_size(listaAux2);j++){
			paginaAux = list_get(listaAux2,j);
			if(paginaAux->frame == marco_aux){
				paginaAux->bit_uso = 1;
				//log_trace(log_memoria,"SE ACTUALIZO EL BIT DE USO");
			}
		}
	}
}

void actualizar_bit_modif_tlb(uint32_t marco_aux){
	t_list* listaAux;
	t_list* listaAux2;
	pagina* paginaAux;
	for(int i=0;i<list_size(lista_tablas_2do_nivel);i++){
		listaAux = list_get(lista_tablas_2do_nivel,i);
		listaAux2 = buscar_paginas_proceso_en_mem_ppal(listaAux);
		for(int j=0;j<list_size(listaAux2);j++){
			paginaAux = list_get(listaAux2,j);
			if(paginaAux->frame == marco_aux){
				paginaAux->bit_modificado = 1;
			//	log_trace(log_memoria,"SE ACTUALIZO EL BIT DE MODIF");
			}
		}
	}
}

int verificar_archivo(char *ruta)
{
	FILE *file;
	char *ruta_final = string_new();
	string_append(&ruta_final, ruta);
	int ret = 0;
	if ((file = fopen(ruta_final, "r")))
	{
		ret = 1;
		fclose(file);
	}
	free(ruta_final);
	return ret;
}


void crear_archivo(char *nuevo_archivo){


	char *ruta_archivo = string_new();
	string_append_with_format(&ruta_archivo,"%s/",path_swap);
	string_append(&ruta_archivo, nuevo_archivo);

	struct stat st = {0};

	if (stat(path_swap, &st) == -1) { //poner ruta a la carpeta donde se vana crear los archivos .swap

		 mkdir(path_swap, 0775);
		// printf("created directory swap successfully! \n");
		 log_info(log_memoria,"Se creo el directorio swap correctamente");
	}

	//log_debug(log_memoria,"el nombre del path con el archivo adentro es: %s",ruta_archivo);



	if (!verificar_archivo(ruta_archivo))
	{
		FILE *archivo = fopen(ruta_archivo, "wb+");
		//w+ es para que sea de lectura escritura.si queremos escribir y leer en binario cambiarlo a wb+


		cant_archivos_swap++;
		fclose(archivo);
	}
	else
	{
		//error_show("El directorio: %s ya existe\n", ruta_archivo);
		log_error("El directorio: %s ya existe\n", ruta_archivo);
	}


	free(ruta_archivo);
}


char *itoa(uint32_t n)
{
    int len = n==0 ? 1 : floor(log(abs(n)))+1;
    if (n<0) len++; // room for negative sign '-'

    char    *buf = calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%d", n);
    return   buf;
}


char* pasar_a_char(uint32_t num){

	char* terminacion = ".swap";


	char* num_char = itoa(num+1);
	//char* num_char = itoa(num);



	char* nuevo_path = strcat(num_char,terminacion);

	return nuevo_path;
}


char* pasar_a_char2(int num){

	char* terminacion = ".swap\0";
	//char* str;






	//char* num_char = my_itoa(num,str);
	char* num_char = itoa(num);

	char* nuevo_path = strcat(num_char,terminacion);

	if (verificar_archivo(nuevo_path))
		{
		return nuevo_path;

		}
}


char* pasar_a_char_sin_terminacion(int num){

	char* num_char = itoa(num);


	return num_char;
}


void poner_archivo_con_ceros(char* path_char,uint32_t tam_proceso){
	//log_trace(log_memoria,"[LLENO DE CEROS EL ARCHIVO DE SAWP que esta en: %s]",path_char);
	FILE *fp= fopen( path_char , "rb+" );
	char* a = "0";
	int x = tam_proceso / 4;

	fseek(fp, 0, SEEK_SET);
	for(int i = 0 ; i < x;i++){
		fwrite(a,sizeof(a),1,fp);
	}



	fclose(fp);
}

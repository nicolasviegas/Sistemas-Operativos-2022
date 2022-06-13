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
		pagina_nueva->tiempo_uso = 0;

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

void liberar_memoria(uint32_t frame){
	//liberar desde el (frame * tam_paginas) hasta (frame + 1 * tam_paginas)
}

#include "../include/comunicacion_memoria.h"



void crear_archivo_swap(uint32_t indice_tabla){ //TODO
//FILE* fp;
//char* a;
//itoa(a);
//fp = fopen(a,"r+w");
}


void escribir_en_swap(uint32_t indice_archivo_swap,uint32_t frame){
	usleep(retardo_swap * 1000);
	// ir a memoria y hacer memcpy desde la direccion y pegarlo en swap
}

void traer_de_swap(uint32_t indice_archivo_swap,uint32_t frame){
	usleep(retardo_swap * 1000);
	// ir a swap y hacer fread y memcpy en mem principal
}


void pasar_proceso_a_swap(uint32_t indice_tabla){ // chequear en entorno controlado
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
		escribir_en_swap(indice_tabla,pagina_aux->frame);
		liberar_memoria(pagina_aux->frame); // todo falta hacer
	}
}

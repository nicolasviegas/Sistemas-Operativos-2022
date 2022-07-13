#include "../include/cpu.h"
#include "../include/funciones_cpu.h"
#include <math.h>


void cargar_instruccion_cpu(int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(lista_instrucciones_cpu,estructura_instrucciones);
	//free(estructura_instrucciones);
}

instrucciones* fetch(pcb_cpu* pcb){
	instrucciones* a = malloc(sizeof(instrucciones));//TODO ELIMINAR EL MALLOC
	//log_debug(log_cpu,"El pc es en fetch %d",pcb->PC);
	a = list_get(pcb->instrucciones,pcb->PC);
	return a;
}

t_list* cargar_instruccion_local(t_list* a,int id, char* nombre, uint32_t parametro1, uint32_t parametro2){
	instrucciones* estructura_instrucciones = malloc(sizeof(instrucciones));

	estructura_instrucciones->id = id;
	estructura_instrucciones->nombre = nombre;
	estructura_instrucciones->parametro1 = parametro1;
	estructura_instrucciones->parametro2 = parametro2;

	list_add(a,estructura_instrucciones);
	//free(estructura_instrucciones);
	return a;
}

////////////////////////////////////////////MMU///////////////////////////////////////////////////////////////////////

		 uint32_t obtener_numero_pagina(uint32_t direccion_logica){
			 uint32_t numero_pag = (uint32_t) floor(direccion_logica / tam_paginas);
			 return numero_pag;
		 }

		 uint32_t obtener_entrada_1er_nivel(uint32_t numero_pagina){
			 uint32_t entrada1 = (uint32_t)floor(numero_pagina/cant_entradas_por_tabla);
			 return entrada1;
		 }

		 uint32_t obtener_entrada_2do_nivel(uint32_t numero_pagina){
			 //return mod(numero_pagina,cant_entradas_por_tabla);
			 return numero_pagina % cant_entradas_por_tabla;
		 }

		 uint32_t obtener_desplazamiento(uint32_t direccion_logica,uint32_t numero_pagina){
			 return direccion_logica - (numero_pagina * tam_paginas);
		 }


	/////////////////////////////////////////////TLB/////////////////////////////////////////////////////////////////////


		 void tlb_flush(uint32_t pid){
			 if(pid != ultimo_proceso_cpu){
				 list_clean_and_destroy_elements(lista_tlb,&free);
				 log_trace(log_cpu,"Se limpio la TLB");
			 }

		 }

		 void aumentar_tiempo(){
			 tlb* a;
			 for(int i=0; i < list_size(lista_tlb); i++){
			 		 a = list_get(lista_tlb,i);
			 		 a->tiempo_uso++;

			 }
		 }



		 void correr_algoritmo_reemplazo(uint32_t numero_pagina,uint32_t marco){
			// log_warning(log_cpu,"Entre en correr algoritmo reemplazo");
			 tlb* pagina_aux_fifo = malloc(sizeof(tlb));
			 if(algoritmo_config == FIFO){
				 if(list_size(lista_tlb) >= entradas_tlb){
					 log_warning(log_cpu,"La TLB esta llena");

					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;

					 pagina_aux_fifo =list_get(lista_tlb,0);
					 log_warning(log_cpu,"Victima TLB por FIFO: pagina %d",pagina_aux_fifo->numero_pag);
					 list_remove_and_destroy_element(lista_tlb,0,free);
					 list_add(lista_tlb,elemento);
					// log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
				 else{
					 log_warning(log_cpu,"Agrego a la TLB ");
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 list_add(lista_tlb,elemento);
					// log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
			 }
			 else{
				 if(list_size(lista_tlb) >= entradas_tlb){
					 tlb* pagina_aux_lru = malloc(sizeof(tlb));

					 log_warning(log_cpu,"La TLB esta llena");
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 elemento->tiempo_uso = 0;/////
					 tlb* elementoAux = list_get(lista_tlb,0);
					 uint32_t tiempo_mas_grande;
					 tiempo_mas_grande = elementoAux->tiempo_uso;
					 int indexARemover = 0;


					 for(int i=0;i<list_size(lista_tlb);i++){
					//	 log_warning(log_cpu,"Entre en EL FOR DE  LRU CASO EN QUE LA TLB ESTA LLENA");

					 elementoAux = list_get(lista_tlb,i);

					// log_debug(log_cpu,"El tiempo del elemento %d es %d",i,elementoAux->tiempo_uso);

					 if(tiempo_mas_grande < elementoAux->tiempo_uso){
						// log_trace(log_cpu,"Entre en EL IF DE  LRU CASO EN QUE LA TLB ESTA LLENA");

					 tiempo_mas_grande = elementoAux->tiempo_uso;
					 indexARemover = i;

				     }

				    }

					 aumentar_tiempo();

					 pagina_aux_lru =list_get(lista_tlb,indexARemover);
					 log_warning(log_cpu,"Victima TLB por LRU: pagina %d",pagina_aux_lru->numero_pag);

					// log_warning(log_cpu,"Antes del remove and destroy, el index a remover es: %d",indexARemover);

					 list_remove_and_destroy_element(lista_tlb,indexARemover,free);
					 list_add(lista_tlb,elemento);
					 //log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
				 else{
					 log_warning(log_cpu,"Agrego a la TLB");
					 aumentar_tiempo();
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 elemento->tiempo_uso = 0;/////
					 list_add(lista_tlb,elemento);
					// log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));

				 }
			 }
		 }





		 void traer_pag_de_tlb(tlb* entrada,uint32_t parametro1,uint32_t tamanio){
			 uint32_t marco = entrada->frame;

			 log_debug(log_cpu,"TLB HIT!");
					 ////////Funciones auxiliares /////



					 bool tienen_misma_pagina(void* elemento){
					 	if(entrada->numero_pag == ((tlb*) elemento)->numero_pag){
					 		return true;
					 	}
					 	else{
					 		return false;
					 	}
					 }

					 //////////////////////////////
					 if(algoritmo_config == LRU){
						 aumentar_tiempo();
						 list_remove_by_condition(lista_tlb,(void*) tienen_misma_pagina);
						 entrada->tiempo_uso = 0;
						 list_add(lista_tlb,entrada);
					//	 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
					 }


					 uint32_t desplazamiento = obtener_desplazamiento(parametro1,entrada->numero_pag);
					 send_TAM(fd_memoria,marco);
					 send_TAM(fd_memoria,desplazamiento);
					 send_TAM(fd_memoria,tamanio);

		 }

		 uint32_t traer_pag_de_tlb_cpy(tlb* entrada,uint32_t parametro1,uint32_t tamanio){
				 uint32_t marco = entrada->frame;

				 log_debug(log_cpu,"TLB HIT!");
						 ////////Funciones auxiliares /////



						 bool tienen_misma_pagina(void* elemento){
						 	if(entrada->numero_pag == ((tlb*) elemento)->numero_pag){
						 		return true;
						 	}
						 	else{
						 		return false;
						 	}
						 }

						 //////////////////////////////
						 if(algoritmo_config == LRU){
							 aumentar_tiempo();
							 list_remove_by_condition(lista_tlb,(void*) tienen_misma_pagina);
							 entrada->tiempo_uso = 0;
							 list_add(lista_tlb,entrada);
						//	 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
						 }

						 return marco;

			 }



		 void correr_tlb_read(uint32_t numero_pagina,uint32_t parametro1,uint32_t tabla_1er_nivel,uint32_t tamanio){

			 bool existe_entrada(void* elem){
				 return ((tlb*) elem)->numero_pag == numero_pagina;
			}

			 tlb* entrada = list_find(lista_tlb, &existe_entrada);
			// log_warning(log_cpu,"pase el list find");
			 if(entrada != NULL){
				 send_TAM(fd_memoria,TLB_RD);//le aviso que tlb read le va a mandar cosas
				 send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
				 traer_pag_de_tlb(entrada,parametro1,tamanio);

				 uint32_t valor_leido;
				 if(recv(fd_memoria,&valor_leido,sizeof(uint32_t),0) != sizeof(uint32_t)){
					 log_error(log_cpu,"Fallo recibiendo el valor leido");
				 }
				 log_debug(log_cpu,"El valor leido es: %d", valor_leido);
			 }
			 else{ //caso en que no este en la tlb
			log_debug(log_cpu,"El nro de pagina en el esle de la tlb es: %d",numero_pagina);
			// uint32_t marco = -1;
			 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
			 uint32_t entrada_2do_nivel = obtener_entrada_2do_nivel(numero_pagina);
			 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);

			 send_TAM(fd_memoria,TLB_RD); // le aviso a memoria que se viene un tlb read
			 log_trace(log_cpu,"TLB MISS!");

			 send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

			 send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
			 log_info(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

			 send_TAM(fd_memoria,entrada_1er_nivel);
			 log_info(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

			 send_TAM(fd_memoria,tamanio);
			 log_info(log_cpu,"El tamanio del proceso es: %d",tamanio);

			 uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
			 if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_error(log_cpu, "fallo al recibir nro tabla segundo nivel!");
				return;
			 }
			 log_info(log_cpu,"El numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

			 send_TAM(fd_memoria,entrada_2do_nivel);//enviamos a memoria la entrada de segundo nivel
			 log_info(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel);


//aca estaba el recv marco aux

			 send_TAM(fd_memoria,desplazamiento);//enviamos el desplazamiento
			 log_info(log_cpu,"El desplazamiento es: %d",desplazamiento);

			 uint32_t valor_leido;
			 if (recv(fd_memoria, &valor_leido, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
			 				log_error(log_cpu, "fallo al recibir valor_leido");
			 				return;
			 }
			 log_debug(log_cpu,"Me llego el valor leido: %d",valor_leido);


			 uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
						 if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_error(log_cpu, "fallo al recibir marcoaux");
							return;
						 }
						 log_info(log_cpu,"el marco es: %d",marcoaux);

						// log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
						 correr_algoritmo_reemplazo(numero_pagina,marcoaux);
		 }
	}


		 	 void correr_tlb_copy(uint32_t numero_pagina_origen,uint32_t numero_pagina_destino,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel,uint32_t tamanio){

		 		 	 bool existe_entrada_origen(void* elem){
		 		 		 return ((tlb*) elem)->numero_pag == numero_pagina_origen;
		 		 	 }

		 		 	bool existe_entrada_destino(void* elem){
		 				 return ((tlb*) elem)->numero_pag == numero_pagina_destino;
		 		 	}
////////////////ORIGEN
		 			 tlb* entrada_origen = list_find(lista_tlb, &existe_entrada_origen);
		 			 log_debug(log_cpu,"El numero de pagina origen es: %d",numero_pagina_origen);

		 			 uint32_t marco_origen;
		 			 uint32_t desplazamiento_origen;
		 			 uint32_t marco_destino;
		 			 uint32_t desplazamiento_destino;


		 			 if(entrada_origen != NULL){
			 			 log_debug(log_cpu,"TLB HIT!");

		 				send_TAM(fd_memoria,TLB_CPY);//le aviso que tlb read le va a mandar cosas
		 				send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
		 			   marco_origen = traer_pag_de_tlb_cpy(entrada_origen,parametro2,tamanio);
		 			  desplazamiento_origen = obtener_desplazamiento(parametro2,entrada_origen->numero_pag);
		 			   	send_TAM(fd_memoria,marco_origen);
		 			   	send_TAM(fd_memoria,desplazamiento_origen);
		 			    send_TAM(fd_memoria,tamanio);


		 			   uint32_t valor_leido;
					 if(recv(fd_memoria,&valor_leido,sizeof(uint32_t),0) != sizeof(uint32_t)){
						 log_error(log_cpu,"Fallo recibiendo el valor leido");
					 }
					 if(valor_leido == 18){
						 log_debug(log_cpu,"Esta listo para copiar:");
					 }

		 			 }
		 			 else{
		 			 log_trace(log_cpu,"TLB MISS!");
		 			 uint32_t entrada_1er_nivel_origen = obtener_entrada_1er_nivel(numero_pagina_origen);
		 			 uint32_t entrada_2do_nivel_origen = obtener_entrada_2do_nivel(numero_pagina_origen);
		 			 desplazamiento_origen = obtener_desplazamiento(parametro2,numero_pagina_origen);


		 			send_TAM(fd_memoria,TLB_CPY); // le aviso a memoria que se viene un tlb read
					//log_trace(log_cpu,"TLB MISS!");

					send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

					send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
					log_info(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

					send_TAM(fd_memoria,entrada_1er_nivel_origen);
					log_info(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel_origen);

					 send_TAM(fd_memoria,tamanio);
					 log_info(log_cpu,"El tamanio del proceso es: %d",tamanio);

					uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
					if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_error(log_cpu, "fallo al recibir nro tabla segundo nivel!");
					return;
					}
					log_info(log_cpu,"El numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

					send_TAM(fd_memoria,entrada_2do_nivel_origen);//enviamos a memoria la entrada de segundo nivel
					log_info(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel_origen);


					send_TAM(fd_memoria,desplazamiento_origen);//enviamos el desplazamiento
					log_info(log_cpu,"El desplazamiento es: %d",desplazamiento_origen);

					 uint32_t valor_leido;
					 if (recv(fd_memoria, &valor_leido, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
									log_error(log_cpu, "fallo al recibir valor_leido");
									return;
					 }
					 if(valor_leido == 18){
						 log_debug(log_cpu,"Me llego la confirmacion de que se copio el origen ");
					 }



					// uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
					 if (recv(fd_memoria, &marco_origen, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_error(log_cpu, "fallo al recibir marcoaux");
						return;
					 }
					 log_info(log_cpu,"el marco es: %d",marco_origen);

					log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
					correr_algoritmo_reemplazo(numero_pagina_origen,marco_origen);

		 			}


////////////////DESTINO

		 			 tlb* entrada_destino = list_find(lista_tlb, &existe_entrada_destino);
		 					 			 log_debug(log_cpu,"El numero de pagina origen es: %d",numero_pagina_origen);


		 					 			 if(entrada_destino != NULL){
		 						 			 log_debug(log_cpu,"TLB HIT!");

		 					 				send_TAM(fd_memoria,TLB_CPY);//le aviso que tlb read le va a mandar cosas
		 					 				send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
		 					 			    marco_destino = traer_pag_de_tlb_cpy(entrada_destino,parametro1,tamanio);
		 					 			    desplazamiento_destino = obtener_desplazamiento(parametro1,entrada_destino->numero_pag);
		 					 			    send_TAM(fd_memoria,marco_destino);
		 					 			    send_TAM(fd_memoria,desplazamiento_destino);
		 					 			    send_TAM(fd_memoria,tamanio);


		 					 			   uint32_t valor_leido;
		 								 if(recv(fd_memoria,&valor_leido,sizeof(uint32_t),0) != sizeof(uint32_t)){
		 									 log_error(log_cpu,"Fallo recibiendo el valor leido");
		 								 }
		 								 if(valor_leido == 18){
		 									 log_debug(log_cpu,"Esta listo para copiar:");
		 								 }

		 					 			 }
		 					 			 else{
		 					 			 log_trace(log_cpu,"TLB MISS!");
		 					 			 uint32_t entrada_1er_nivel_destino = obtener_entrada_1er_nivel(numero_pagina_destino);
		 					 			 uint32_t entrada_2do_nivel_destino = obtener_entrada_2do_nivel(numero_pagina_destino);
		 					 			 desplazamiento_destino = obtener_desplazamiento(parametro1,numero_pagina_destino);


		 					 			send_TAM(fd_memoria,TLB_CPY); // le aviso a memoria que se viene un tlb read
		 								//log_trace(log_cpu,"TLB MISS!");

		 								send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

		 								send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
		 								log_info(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

		 								send_TAM(fd_memoria,entrada_1er_nivel_destino);
		 								log_info(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel_destino);

										 send_TAM(fd_memoria,tamanio);
										 log_info(log_cpu,"El tamanio del proceso es: %d",tamanio);

		 								uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
		 								if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		 								log_error(log_cpu, "fallo al recibir nro tabla segundo nivel!");
		 								return;
		 								}
		 								log_info(log_cpu,"El numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

		 								send_TAM(fd_memoria,entrada_2do_nivel_destino);//enviamos a memoria la entrada de segundo nivel
		 								log_info(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel_destino);


		 								send_TAM(fd_memoria,desplazamiento_destino);//enviamos el desplazamiento
		 								log_info(log_cpu,"El desplazamiento es: %d",desplazamiento_destino);

		 								 uint32_t valor_leido;
		 								 if (recv(fd_memoria, &valor_leido, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		 												log_error(log_cpu, "fallo al recibir valor_leido");
		 												return;
		 								 }
		 								 if(valor_leido == 18){
		 									 log_debug(log_cpu,"Me llego la confirmacion de que se copio en el destino ");
		 								 }



//		 								 uint32_t marco_destino; //aca recibe de memoria el numero de la tabla de segundo nivel
		 								 if (recv(fd_memoria, &marco_destino, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		 									log_error(log_cpu, "fallo al recibir marco_destino");
		 									return;
		 								 }
		 								 log_info(log_cpu,"el marco destino es: %d",marco_destino);

		 								log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
		 								correr_algoritmo_reemplazo(numero_pagina_destino,marco_destino);

		 					 			}


		 			send_TAM(fd_memoria,COPIAR_PAGINAS);
		 			send_TAM(fd_memoria,marco_origen);
		 			send_TAM(fd_memoria,desplazamiento_origen);
		 			send_TAM(fd_memoria,marco_destino);
					send_TAM(fd_memoria,desplazamiento_destino);
					send_TAM(fd_memoria,tamanio);

}



		 		 void correr_tlb_write(uint32_t numero_pagina,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel,uint32_t tamanio){

		 			bool existe_entrada(void* elem){
							 return ((tlb*) elem)->numero_pag == numero_pagina;
						}

						 tlb* entrada = list_find(lista_tlb, &existe_entrada);
						// log_warning(log_cpu,"pase el list find");
						 if(entrada != NULL){
							 send_TAM(fd_memoria,TLB_WR);//le aviso que tlb read le va a mandar cosas
							 send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb

							 traer_pag_de_tlb(entrada,parametro1,tamanio);

							 send_TAM(fd_memoria,parametro2);//enviamos el valor a escribir

							 uint32_t rta_mem;
							 if(recv(fd_memoria,&rta_mem,sizeof(uint32_t),0) != sizeof(uint32_t)){
								 log_error(log_cpu,"Fallo escribiendo en memoria");
							 }
							 if(rta_mem == 100){
								 log_debug(log_cpu,"Se escribio en memoria exitosamente");
							 }

						 }
						 else{ //caso en que no este en la tlb

						// uint32_t marco = -1;
						 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
						 uint32_t entrada_2do_nivel = obtener_entrada_2do_nivel(numero_pagina);
						 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);

						 send_TAM(fd_memoria,TLB_WR); // le aviso a memoria que se viene un tlb read
						 log_trace(log_cpu,"TLB MISS!");

						 send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

						 send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
						 log_info(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

						 send_TAM(fd_memoria,entrada_1er_nivel);
						 log_info(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

						 send_TAM(fd_memoria,tamanio);
						 log_info(log_cpu,"El tamanio del proceso es: %d",tamanio);

						 uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
						 if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_error(log_cpu, "fallo al recibir nro tabla segundo nivel!");
							return;
						 }
						 log_info(log_cpu,"El numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

						 send_TAM(fd_memoria,entrada_2do_nivel);//enviamos a memoria la entrada de segundo nivel
						 log_info(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel);
//ESTE HAY QUE PONERLO ANTES DEL EJECUTAR ALGOREEMPLAZO
//						 uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
//						 if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
//							log_info(log_cpu, "fallo al recibir marcoaux");
//							return;
//						 }
//						 log_error(log_cpu,"el marco es: %d",marcoaux);


						 send_TAM(fd_memoria,desplazamiento);//enviamos el desplazamiento
						 log_info(log_cpu,"El desplazamiento es: %d",desplazamiento);

						 send_TAM(fd_memoria,parametro2);//enviamos el valor a escribir
						 log_info(log_cpu,"El valor a escribir es: %d",parametro2);




						 uint32_t rta_mem;
						 if(recv(fd_memoria,&rta_mem,sizeof(uint32_t),0) != sizeof(uint32_t)){
							 log_error(log_cpu,"Fallo escribiendo en memoria");
						 }
						 if(rta_mem == 100){
							 log_debug(log_cpu,"Se escribio en memoria exitosamente");
						 }








						 uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
						 if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_error(log_cpu, "fallo al recibir marcoaux");
							return;
						 }
						 log_info(log_cpu,"el marco es: %d",marcoaux);

						// log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
							correr_algoritmo_reemplazo(numero_pagina,marcoaux);
		 		 	 }
		 		 	}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////Instrucciones///////

void decode_and_execute(pcb_cpu* pcb,instrucciones* instruccion_a_decodificar){
	int co_op = instruccion_a_decodificar->id;


	switch (co_op) {
			case NO_OP:{
				tiempo_bloqueante = 0;
				log_info(log_cpu,"[EXE] ejecuto NO_OP");
				//log_warning(log_cpu,"Antes del sleep");
				//log_warning(log_cpu,"EL PARAMETRO 1 ES: %d",instruccion_a_decodificar->parametro1);
				int retardo_NO_OP = config_get_int_value(config_cpu,"RETARDO_NOOP");
				retardo_NO_OP = retardo_NO_OP / 1000;
				for(int i=0;i<instruccion_a_decodificar->parametro1;i++){
					sleep(retardo_NO_OP);
				}
				//log_warning(log_cpu,"El program counter es: %d",pcb->PC);

				//log_warning(log_cpu,"Termine NO_OP");

				pcb->PC += 1;

				// log_trace(log_cpu,"El PC es: %d",pcb->PC);


			break;
			}
			case IO:{

				tiempo_bloqueante = instruccion_a_decodificar->parametro1;
				log_info(log_cpu,"[EXE] ejecuto I/O, bloqueo por: %d",tiempo_bloqueante);

				//log_warning(log_cpu,"El tiempo bloqueante es: %d",tiempo_bloqueante);
				pcb->PC += 1;
				//interrupcion = true; // ESTO ES DE PRUEBA, VA ESTO O NO?
				break;
			 }
			 case READ:{
				 tiempo_bloqueante = 0;
				 uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				 uint32_t numero_pagina = obtener_numero_pagina(parametro1);

				 log_info(log_cpu,"[EXE] ejecuto READ en la pagina: %d",numero_pagina);

			//	log_debug(log_cpu,"El numero de paginas es %d",numero_pagina);

				 correr_tlb_read(numero_pagina,parametro1,pcb->indice_tabla_paginas,pcb->tamanio);
				 pcb->PC += 1;
				// log_trace(log_cpu,"El PC es: %d",pcb->PC);
				 break;
			}
			 case COPY:{
				tiempo_bloqueante = 0;
				// log_warning(log_cpu,"Entre en COPY");
				uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				uint32_t parametro2 = instruccion_a_decodificar->parametro2;
				uint32_t numero_pagina_origen = obtener_numero_pagina(parametro2);
				uint32_t numero_pagina_destino = obtener_numero_pagina(parametro1);
				 //log_trace(log_cpu,"Antes de correr tlb copy");
				log_info(log_cpu,"[EXE] ejecuto COPY, pagina origen: %d , pagina destino: %d",numero_pagina_origen,numero_pagina_destino);

				//log_debug(log_cpu,"El numero de paginas origen es %d",numero_pagina_origen);
				//log_debug(log_cpu,"El numero de paginas destino es %d",numero_pagina_destino);


				correr_tlb_copy(numero_pagina_origen,numero_pagina_destino,parametro1,parametro2,pcb->indice_tabla_paginas,pcb->tamanio);
				pcb->PC += 1;
				break;
			}
			 case WRITE:{
				tiempo_bloqueante = 0;
				//log_info(log_cpu,"[EXE] ejecuto WRITE");
				// log_warning(log_cpu,"Entre en WRITE");
				uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				uint32_t parametro2 = instruccion_a_decodificar->parametro2;
				uint32_t numero_pagina_write = obtener_numero_pagina(parametro1);
				//log_debug(log_cpu,"El numero de paginas es %d",numero_pagina_write);
				log_info(log_cpu,"[EXE] ejecuto WRITE, pagina a escribir: %d",numero_pagina_write);

				correr_tlb_write(numero_pagina_write,parametro1,parametro2,pcb->indice_tabla_paginas,pcb->tamanio);
				pcb->PC += 1;
				break;
			}
			 case EXIT:
			 {
				 tiempo_bloqueante = 0;
				// log_trace(log_cpu,"ENTRE EN EXIT");
					log_info(log_cpu,"[EXE] ejecuto EXIT");
				 //log_trace(log_cpu,"Finalizo el proceso");
				 pcb->PC += 1;

				 send_TAM(fd_memoria,SACAR_PROCESO_DE_MEMORIA);
				 send_TAM(fd_memoria,pcb->indice_tabla_paginas);

				 uint32_t confirmacion; //aca recibe de memoria el numero de la tabla de segundo nivel
				 if (recv(fd_memoria, &confirmacion, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_error(log_cpu, "fallo al recibir nro tabla segundo nivel!");
					return;
				 }

				 if(confirmacion == 1773){
					 log_info(log_cpu,"Se libero el proceso %d de memoria",pcb->PID);
				 }

				 //remover proceso de mem principal
				break;
			}

			 default:
				 log_error(log_cpu, "No es una instruccion valida");
				 //log_info(log_cpu, "Cop: %d", co_op);
				 //return;
				 break;
		 }

	//log_warning("Se aumento el program counter, la sig instruccion es: %d",pcb->PC);
}

bool check_interrupt(int cliente_socket){

	uint32_t cod_interrup;
	if (!recv_interrupcion(cliente_socket, &cod_interrup)) {
			//log_error(log_cpu, "no se interrumpio");

	}

	//log_debug(log_cpu,"ESTOY ADENTRO DE CHECJ INTERRUPT");

	if(cod_interrup == 777){
	//	log_debug(log_cpu,"RECIBI EL 777");

		//interrupcion = true;
		log_debug(log_cpu,"Recibi una interrupcion");
		return true;
	}

	return false;

}

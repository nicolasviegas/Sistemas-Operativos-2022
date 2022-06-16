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
	log_debug(log_cpu,"El pc es en fetch %d",pcb->PC);
	a = list_get(pcb->instrucciones,pcb->PC);
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


		 void tlb_flush(){
			 list_clean_and_destroy_elements(lista_tlb,&free);
//			 log_trace(log_cpu,"Se limpio la TLB");
		 }

		 void aumentar_tiempo(){
			 tlb* a;
			 for(int i=0; i < list_size(lista_tlb); i++){
			 		 a = list_get(lista_tlb,i);
			 		 a->tiempo_uso++;

			 }
		 }



		 void correr_algoritmo_reemplazo(uint32_t numero_pagina,uint32_t marco){
			 log_warning(log_cpu,"Entre en correr algoritmo reemplazo");
			 if(algoritmo_config == FIFO){
				 if(list_size(lista_tlb) >= entradas_tlb){
					 log_warning(log_cpu,"Entre en FIFO CASO EN QUE LA TLB ESTA LLENA");

					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 list_remove_and_destroy_element(lista_tlb,0,free);
					 list_add(lista_tlb,elemento);
					 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
				 else{
					 log_warning(log_cpu,"Entre en FIFO CASO EN QUE agrego de una ");
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 list_add(lista_tlb,elemento);
					 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
			 }
			 else{
				 if(list_size(lista_tlb) >= entradas_tlb){
					 log_warning(log_cpu,"Entre en LRU CASO EN QUE LA TLB ESTA LLENA");
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 elemento->tiempo_uso = 0;/////
					 tlb* elementoAux = list_get(lista_tlb,0);
					 uint32_t tiempo_mas_grande;
					 tiempo_mas_grande = elementoAux->tiempo_uso;
					 int indexARemover = 0;


					 for(int i=0;i<list_size(lista_tlb);i++){
						 log_warning(log_cpu,"Entre en EL FOR DE  LRU CASO EN QUE LA TLB ESTA LLENA");

					 elementoAux = list_get(lista_tlb,i);

					 log_debug(log_cpu,"El tiempo del elemento %d es %d",i,elementoAux->tiempo_uso);

					 if(tiempo_mas_grande < elementoAux->tiempo_uso){
						 log_trace(log_cpu,"Entre en EL IF DE  LRU CASO EN QUE LA TLB ESTA LLENA");

					 tiempo_mas_grande = elementoAux->tiempo_uso;
					 indexARemover = i;

				     }

				    }

					 aumentar_tiempo();
					 log_warning(log_cpu,"Antes del remove and destroy, el index a remover es: %d",indexARemover);

					 list_remove_and_destroy_element(lista_tlb,indexARemover,free);
					 list_add(lista_tlb,elemento);
					 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
				 }
				 else{
					 log_warning(log_cpu,"Entre en LRU CASO EN QUE agrego de una ");
					 aumentar_tiempo();
					 tlb* elemento = malloc(sizeof(tlb));
					 elemento->frame = marco;
					 elemento->numero_pag = numero_pagina;
					 elemento->tiempo_uso = 0;/////
					 list_add(lista_tlb,elemento);
					 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));

				 }
			 }
		 }





		 void traer_pag_de_tlb(tlb* entrada,uint32_t parametro1){
			 uint32_t marco = entrada->frame;

			 log_debug(log_cpu,"Entre en traer pag de tlb");
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
						 log_warning(log_cpu,"El size de la lista tlb es: %d",list_size(lista_tlb));
					 }


					 uint32_t desplazamiento = obtener_desplazamiento(parametro1,entrada->numero_pag);
					 send_TAM(fd_memoria,marco);
					 send_TAM(fd_memoria,desplazamiento);

		 }



		 void correr_tlb_read(uint32_t numero_pagina,uint32_t parametro1,uint32_t tabla_1er_nivel){

			 bool existe_entrada(void* elem){
				 return ((tlb*) elem)->numero_pag == numero_pagina;
			}

			 tlb* entrada = list_find(lista_tlb, &existe_entrada);
			 log_warning(log_cpu,"pase el list find");
			 if(entrada != NULL){
				 send_TAM(fd_memoria,TLB_RD);//le aviso que tlb read le va a mandar cosas
				 send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
				 traer_pag_de_tlb(entrada,parametro1);

				 uint32_t valor_leido;
				 if(recv(fd_memoria,&valor_leido,sizeof(uint32_t),0) != sizeof(uint32_t)){
					 log_error(log_cpu,"Fallo recibiendo el valor leido");
				 }
				 log_debug(log_cpu,"El valor leido es: %d", valor_leido);
			 }
			 else{ //caso en que no este en la tlb

			 uint32_t marco = -1;
			 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
			 uint32_t entrada_2do_nivel = obtener_entrada_2do_nivel(numero_pagina);
			 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);

			 send_TAM(fd_memoria,TLB_RD); // le aviso a memoria que se viene un tlb read
			 log_trace(log_cpu,"Le aviso a memoeria que le voy a mandar cosas");

			 send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

			 send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
			 log_error(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

			 send_TAM(fd_memoria,entrada_1er_nivel);
			 log_error(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

			 uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
			 if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_cpu, "fallo al recibir nro tabla segundo nivel!");
				return;
			 }
			 log_error(log_cpu,"el numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

			 send_TAM(fd_memoria,entrada_2do_nivel);//enviamos a memoria la entrada de segundo nivel
			 log_error(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

			 uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
			 if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_cpu, "fallo al recibir marcoaux");
				return;
			 }
			 log_error(log_cpu,"el marco es: %d",marcoaux);


			 send_TAM(fd_memoria,desplazamiento);//enviamos el desplazamiento
			 log_error(log_cpu,"El desplazamiento es: %d",desplazamiento);

			 uint32_t valor_leido;
			 if (recv(fd_memoria, &valor_leido, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
			 				log_info(log_cpu, "fallo al recibir valor_leido");
			 				return;
			 }
			 log_debug(log_cpu,"Me llego el valor leido: %d",valor_leido);


	//		 uint32_t rta_mem; //todo aca se podria recibir una rta de memorai para ver si es valido leer o no
	//		 recv_rta_memoria(fd_memoria,&rta_mem);
	//		 if(rta_mem == 0){
	//			 log_error(log_cpu, "La direccion donde se quiso leer no es valida");
	//		}
	//		 else{
			 log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
				correr_algoritmo_reemplazo(numero_pagina,marcoaux);
	//		}
		 }
	}


		 	 void correr_tlb_copy(uint32_t numero_pagina_origen,uint32_t numero_pagina_destino,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel){

		 		 	 bool existe_entrada_origen(void* elem){
		 		 		 return ((tlb*) elem)->numero_pag == numero_pagina_origen;
		 		 	 }

		 		 	bool existe_entrada_destino(void* elem){
		 				 return ((tlb*) elem)->numero_pag == numero_pagina_destino;
		 		 	}
////////////////ORIGEN
		 			 tlb* entrada_origen = list_find(lista_tlb, &existe_entrada_origen);
		 			 log_debug(log_cpu,"El numero de pagina origen es: %d",numero_pagina_origen);


		 			 if(entrada_origen != NULL){
			 			 log_debug(log_cpu,"Entre en el caso feliz origen de la tlb en cpy");

		 				send_TAM(fd_memoria,TLB_CPY);//le aviso que tlb read le va a mandar cosas
		 				send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
		 			    traer_pag_de_tlb(entrada_origen,parametro2);
		 			 }
		 			 else{
		 			 log_debug(log_cpu,"Entre en el caso triste origen de la tlb en cpy");
		 			 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
		 			 uint32_t marco_origen = -1;
		 			 uint32_t entrada_1er_nivel_origen = obtener_entrada_1er_nivel(numero_pagina_origen);
		 			 uint32_t entrada_2do_nivel_origen = obtener_entrada_2do_nivel(numero_pagina_origen);
		 			 uint32_t desplazamiento_origen = obtener_desplazamiento(parametro2,numero_pagina_origen);


		 			send_TAM(fd_memoria,TLB_CPY); // le aviso a memoria que se viene un tlb read
					log_trace(log_cpu,"Le aviso a memoria que le voy a mandar cosas");

					send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

					send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
					log_error(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

					send_TAM(fd_memoria,entrada_1er_nivel_origen);
					log_error(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel_origen);

					uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
					if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_cpu, "fallo al recibir nro tabla segundo nivel!");
					return;
					}
					log_error(log_cpu,"el numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

					send_TAM(fd_memoria,entrada_2do_nivel_origen);//enviamos a memoria la entrada de segundo nivel
					log_error(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel_origen);

					uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
					if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_cpu, "fallo al recibir marcoaux");
					return;
					}
					log_error(log_cpu,"el marco es: %d",marcoaux);


					send_TAM(fd_memoria,desplazamiento_origen);//enviamos el desplazamiento
					log_error(log_cpu,"El desplazamiento es: %d",desplazamiento_origen);

					//		 uint32_t rta_mem; //todo aca se podria recibir una rta de memorai para ver si es valido leer o no
					//		 recv_rta_memoria(fd_memoria,&rta_mem);
					//		 if(rta_mem == 0){
					//			 log_error(log_cpu, "La direccion donde se quiso leer no es valida");
					//		}
					//		 else{
					log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
					correr_algoritmo_reemplazo(numero_pagina_origen,marcoaux);
					//		}
		 			}


////////////////ORIGEN

		 			tlb* entrada_destino = list_find(lista_tlb, &existe_entrada_destino);

		 			//log_debug(log_cpu,"El nro de pagina de entrada destino es: %d",entrada_destino->numero_pag);
                    if(entrada_destino != NULL){
                    	log_debug(log_cpu,"Entre en el caso feliz origen de la tlb en cpy");
						send_TAM(fd_memoria,TLB_CPY);//le aviso que tlb read le va a mandar cosas
						send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb
						traer_pag_de_tlb(entrada_destino,parametro1);
                    }
		 			else{
			 			 log_debug(log_cpu,"Entre en el caso triste destino de la tlb en cpy");

		 				 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
		 				 uint32_t marco_destino = -1;
		 //				 uint32_t numero_pagina_destino = obtener_numero_pagina(parametro1);
		 				 uint32_t entrada_1er_nivel_destino = obtener_entrada_1er_nivel(numero_pagina_destino);
		 				 uint32_t entrada_2do_nivel_destino = obtener_entrada_2do_nivel(numero_pagina_destino);
		 				 uint32_t desplazamiento_destino = obtener_desplazamiento(parametro1,numero_pagina_destino);
//
		 				    send_TAM(fd_memoria,TLB_CPY); // le aviso a memoria que se viene un tlb read
							log_trace(log_cpu,"Le aviso a memoria que le voy a mandar cosas");

							send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

							send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
							log_error(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

							send_TAM(fd_memoria,entrada_1er_nivel_destino);
							log_error(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel_destino);

							uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
							if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_cpu, "fallo al recibir nro tabla segundo nivel!");
							return;
							}
							log_error(log_cpu,"el numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

							send_TAM(fd_memoria,entrada_2do_nivel_destino);//enviamos a memoria la entrada de segundo nivel
							log_error(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel_destino);

							uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
							if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_cpu, "fallo al recibir marcoaux");
							return;
							}
							log_error(log_cpu,"el marco es: %d",marcoaux);


							send_TAM(fd_memoria,desplazamiento_destino);//enviamos el desplazamiento
							log_error(log_cpu,"El desplazamiento es: %d",desplazamiento_destino);

							//		 uint32_t rta_mem; //todo aca se podria recibir una rta de memorai para ver si es valido leer o no
							//		 recv_rta_memoria(fd_memoria,&rta_mem);
							//		 if(rta_mem == 0){
							//			 log_error(log_cpu, "La direccion donde se quiso leer no es valida");
							//		}
							//		 else{
							log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
							correr_algoritmo_reemplazo(numero_pagina_destino,marcoaux);
//		 			}
		 		}
		 	}



		 		 void correr_tlb_write(uint32_t numero_pagina,uint32_t parametro1,uint32_t parametro2,uint32_t tabla_1er_nivel){

		 			bool existe_entrada(void* elem){
							 return ((tlb*) elem)->numero_pag == numero_pagina;
						}

						 tlb* entrada = list_find(lista_tlb, &existe_entrada);
						 log_warning(log_cpu,"pase el list find");
						 if(entrada != NULL){
							 send_TAM(fd_memoria,TLB_WR);//le aviso que tlb read le va a mandar cosas
							 send_TAM(fd_memoria,1234);//el 1234 es para avisarle a memoria que esta en la tlb

							 traer_pag_de_tlb(entrada,parametro1);

							 send_TAM(fd_memoria,parametro2);//enviamos el valor a escribir

						 }
						 else{ //caso en que no este en la tlb

						 uint32_t marco = -1;
						 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
						 uint32_t entrada_2do_nivel = obtener_entrada_2do_nivel(numero_pagina);
						 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);

						 send_TAM(fd_memoria,TLB_WR); // le aviso a memoria que se viene un tlb read
						 log_trace(log_cpu,"Le aviso a memoeria que le voy a mandar cosas");

						 send_TAM(fd_memoria,4321);//le aviso que no estaba en la tlb

						 send_TAM(fd_memoria,tabla_1er_nivel); //send indice tabla pagina
						 log_error(log_cpu,"El indice tabla pagina %d",tabla_1er_nivel);

						 send_TAM(fd_memoria,entrada_1er_nivel);
						 log_error(log_cpu,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

						 uint32_t numero_tabla_segundo_nivel; //aca recibe de memoria el numero de la tabla de segundo nivel
						 if (recv(fd_memoria, &numero_tabla_segundo_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_cpu, "fallo al recibir nro tabla segundo nivel!");
							return;
						 }
						 log_error(log_cpu,"el numero_tabla_segundo_nivel es: %d",numero_tabla_segundo_nivel);

						 send_TAM(fd_memoria,entrada_2do_nivel);//enviamos a memoria la entrada de segundo nivel
						 log_error(log_cpu,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

						 uint32_t marcoaux; //aca recibe de memoria el numero de la tabla de segundo nivel
						 if (recv(fd_memoria, &marcoaux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_cpu, "fallo al recibir marcoaux");
							return;
						 }
						 log_error(log_cpu,"el marco es: %d",marcoaux);


						 send_TAM(fd_memoria,desplazamiento);//enviamos el desplazamiento
						 log_error(log_cpu,"El desplazamiento es: %d",desplazamiento);

						 send_TAM(fd_memoria,parametro2);//enviamos el valor a escribir
						 log_error(log_cpu,"El valor a escribir es: %d",parametro2);

				//		 uint32_t rta_mem; //todo aca se podria recibir una rta de memorai para ver si es valido leer o no
				//		 recv_rta_memoria(fd_memoria,&rta_mem);
				//		 if(rta_mem == 0){
				//			 log_error(log_cpu, "La direccion donde se quiso leer no es valida");
				//		}
				//		 else{
						 log_warning(log_cpu,"Amtes de correr algoritmo reemplzo");
							correr_algoritmo_reemplazo(numero_pagina,marcoaux);
//		 			  }
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

				log_warning(log_cpu,"Termine NO_OP");

				pcb->PC += 1;

				 log_trace(log_cpu,"El PC es: %d",pcb->PC);


			break;
			}
			case IO:{

				log_info(log_cpu,"[EXE] ejecuto IO");
				tiempo_bloqueante = instruccion_a_decodificar->parametro1;
				log_warning(log_cpu,"El tiempo bloqueante es: %d",tiempo_bloqueante);
				pcb->PC += 1;
				//interrupcion = true; // ESTO ES DE PRUEBA, VA ESTO O NO?
				break;
			 }
			 case READ:{
				 tiempo_bloqueante = 0;
				log_info(log_cpu,"[EXE] ejecuto READ");
				 uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				 uint32_t numero_pagina = obtener_numero_pagina(parametro1);

				log_debug(log_cpu,"El numero de paginas es %d",numero_pagina);

				 correr_tlb_read(numero_pagina,parametro1,pcb->indice_tabla_paginas);
				 pcb->PC += 1;
				 log_trace(log_cpu,"El PC es: %d",pcb->PC);
				 break;
			}
			 case COPY:{
				tiempo_bloqueante = 0;
				log_info(log_cpu,"[EXE] ejecuto COPY");
				// log_warning(log_cpu,"Entre en COPY");
				uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				uint32_t parametro2 = instruccion_a_decodificar->parametro2;
				uint32_t numero_pagina_origen = obtener_numero_pagina(parametro2);
				uint32_t numero_pagina_destino = obtener_numero_pagina(parametro1);
				 //log_trace(log_cpu,"Antes de correr tlb copy");
				log_debug(log_cpu,"El numero de paginas origen es %d",numero_pagina_origen);
				log_debug(log_cpu,"El numero de paginas destino es %d",numero_pagina_destino);


				correr_tlb_copy(numero_pagina_origen,numero_pagina_destino,parametro1,parametro2,pcb->indice_tabla_paginas);
				pcb->PC += 1;
				break;
			}
			 case WRITE:{
				tiempo_bloqueante = 0;
				log_info(log_cpu,"[EXE] ejecuto WRITE");
				// log_warning(log_cpu,"Entre en WRITE");
				uint32_t parametro1 = instruccion_a_decodificar->parametro1;
				uint32_t parametro2 = instruccion_a_decodificar->parametro2;
				uint32_t numero_pagina_write = obtener_numero_pagina(parametro1);
				log_debug(log_cpu,"El numero de paginas es %d",numero_pagina_write);
				correr_tlb_write(numero_pagina_write,parametro1,parametro2,pcb->indice_tabla_paginas);
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
			log_error(log_cpu, "fallo recibiendo la interrupcion");

	}
	if(cod_interrup == 777){
		interrupcion = true;
		log_debug(log_cpu,"Recibi una interrupcion");
		return interrupcion;
	}else{
		//log_error(log_cpu,"No recibi ninguna interrupcion: %d",cod_interrup);
		return false;
	}
}

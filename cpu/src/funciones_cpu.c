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
	instrucciones* a = malloc(sizeof(instrucciones));
	a = list_get(pcb->instrucciones,pcb->PC);
	return a;
}

////////////////////////////////////////////MMU///////////////////////////////////////////////////////////////////////

	 uint32_t obtener_numero_pagina(uint32_t direccion_logica){
		 uint32_t dir_logica = floor(direccion_logica / tam_paginas);
		 return dir_logica;
	 }

	 uint32_t obtener_entrada_1er_nivel(uint32_t numero_pagina){
		 uint32_t entrada1 = floor(numero_pagina/cant_entradas_por_tabla);
		 return entrada1;
	 }

	 uint32_t obtener_entrada_2do_nivel(uint32_t numero_pagina){
		 //return mod(numero_pagina,cant_entradas_por_tabla);
		 return numero_pagina % cant_entradas_por_tabla;
	 }

	 uint32_t obtener_desplazamiento(uint32_t direccion_logica,uint32_t numero_pagina){
		 return direccion_logica - numero_pagina * tam_paginas;
	 }


/////////////////////////////////////////////TLB/////////////////////////////////////////////////////////////////////


	 void tlb_flush(){
		 list_clean_and_destroy_elements(lista_tlb,&free);
	 }



	 void correr_algoritmo_reemplazo(uint32_t numero_pagina,uint32_t marco){
		 if(reemplazo_tlb == FIFO){
			 if(list_size(lista_tlb) >= entradas_tlb){
				 tlb* elemento;
				 elemento->frame = marco;
				 elemento->numero_pag = numero_pagina;
				 list_remove_and_destroy_element(lista_tlb,0,free);
				 list_add(lista_tlb,elemento);
			 }
			 else{
				 tlb* elemento;
				 elemento->frame = marco;
				 elemento->numero_pag = numero_pagina;
				 list_add(lista_tlb,elemento);
			 }
		 }
		 else{
			 if(list_size(lista_tlb) >= entradas_tlb){
				 tlb* elemento;
				 elemento->frame = marco;
				 elemento->numero_pag = numero_pagina;
				 elemento->tiempo_uso = 0;/////
				 tlb* elementoAux = list_get(lista_tlb,0);
				 uint32_t tiempo_mas_grande;
				 tiempo_mas_grande = elementoAux->tiempo_uso;
				 int indexARemover;


				 for(int i=0;i<list_size(lista_tlb);i++){
				 elementoAux = list_get(lista_tlb,i);

				 if(tiempo_mas_grande < elementoAux->tiempo_uso){
				 tiempo_mas_grande = elementoAux->tiempo_uso;
				 indexARemover = i;

			     }

			    }
				 list_remove_and_destroy_element(lista_tlb,indexARemover,free);
				 list_add(lista_tlb,elemento);
			 }
			 else{
				 tlb* elemento;
				 elemento->frame = marco;
				 elemento->numero_pag = numero_pagina;
				 elemento->tiempo_uso = 0;/////
				 list_add(lista_tlb,elemento);

			 }
		 }
	 }





	 void traer_pag_de_tlb(tlb* entrada,uint32_t parametro1){
		 uint32_t marco = entrada->frame;

				 ////////Funciones auxiliares /////


				 void actualizar_tiempo_tlb(tlb* entrada){
				 	entrada->tiempo_uso = entrada->tiempo_uso++;
				 }

				 bool tienen_misma_pagina(void* elemento){
				 	if(entrada->numero_pag == ((tlb*) elemento)->numero_pag){
				 		return true;
				 	}
				 	else{
				 		return false;
				 	}
				 }

				 //////////////////////////////

				 list_iterate(lista_tlb, (void*) actualizar_tiempo_tlb);
				 list_remove_by_condition(lista_tlb,(void*) tienen_misma_pagina);
				 entrada->tiempo_uso = 0;
				 list_add(lista_tlb,entrada);

				 //send condicion a memoria para que sepa que le voy a pasar el marco TODO
				 //send marco
				 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);
				 send_desplazamiento(fd_memoria,desplazamiento);

	 }



	 void correr_tlb_read(uint32_t numero_pagina,uint32_t parametro1){

		 bool existe_entrada(void* elem){
		 		 return ((tlb*) elem)->numero_pag == numero_pagina;
		 }


		 tlb* entrada = list_find(lista_tlb, &existe_entrada);
		 if(entrada != NULL){
			 traer_pag_de_tlb(entrada,parametro1);
		 }
		 else{
		 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
		 uint32_t marco = -1;
		 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
		 uint32_t entrada_2do_nivel = obtener_entrada_1er_nivel(numero_pagina);
		 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);
		 send_numero_pagina(fd_memoria,numero_pagina);
		 send_tabla_primer_nivel_pcb();
		 send_entrada_1er_nivel(fd_memoria,entrada_1er_nivel);

		 recv_tabla_2do_nivel(fd_memoria,&entrada_1er_nivel);

		 send_entrada_2do_nivel(fd_memoria,entrada_2do_nivel);

		 recv_marco(fd_memoria,&marco);
		 send_desplazamiento(fd_memoria,desplazamiento);

		 uint32_t rta_mem;
		 recv_rta_memoria(fd_memoria,&rta_mem);
		 if(rta_mem == 0){
			 log_error(log_cpu, "La direccion donde se quiso leer no es valida");
		}
		 else{
			 correr_algoritmo_reemplazo(numero_pagina,marco);
		}
	 }

	 void correr_tlb_copy(uint32_t numero_pagina_origen,uint32_t numero_pagina_destino,uint32_t parametro1,uint32_t parametro2){

		 	 bool existe_entrada_origen(void* elem){
		 		 return ((tlb*) elem)->numero_pag == numero_pagina_origen;
		 	 }

		 	bool existe_entrada_destino(void* elem){
				 return ((tlb*) elem)->numero_pag == numero_pagina_destino;
		 	}

			 tlb* entrada_origen = list_find(lista_tlb, &existe_entrada_origen);
			 tlb* entrada_destino = list_find(lista_tlb, &existe_entrada_destino);
			 if(entrada_origen != NULL){
			 traer_pag_de_tlb(entrada_origen,parametro2);
			 }
			 else{
			 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
			 uint32_t marco_origen = -1;
			 uint32_t entrada_1er_nivel_origen = obtener_entrada_1er_nivel(numero_pagina_origen);
			 uint32_t entrada_2do_nivel_origen = obtener_entrada_1er_nivel(numero_pagina_origen);
			 uint32_t desplazamiento_origen = obtener_desplazamiento(parametro2,numero_pagina_origen);




			 send_numero_pagina(fd_memoria,numero_pagina);
			 send_tabla_primer_nivel_pcb();
			 send_entrada_1er_nivel(fd_memoria,entrada_1er_nivel_origen);

			 recv_tabla_2do_nivel(fd_memoria,&entrada_1er_nivel_origen);

			 send_entrada_2do_nivel(fd_memoria,entrada_2do_nivel_origen);

			 recv_marco(fd_memoria,&marco_origen);
			 send_desplazamiento(fd_memoria,desplazamiento_origen);
			 uint32_t rta_mem;
			 recv_rta_memoria(fd_memoria,&rta_mem);
			 if(rta_mem == 0){
				 log_error(log_cpu, "La direccion donde se quiso copiar no es valida");
			 }
			 else{
				 correr_algoritmo_reemplazo(numero_pagina,marco_origen);
			 }
			}
			if(entrada_destino != NULL){
				 traer_pag_de_tlb(entrada_destino,parametro1);
			}
			else{
				 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
				 uint32_t marco_destino = -1;
//				 uint32_t numero_pagina_destino = obtener_numero_pagina(parametro1);
				 uint32_t entrada_1er_nivel_destino = obtener_entrada_1er_nivel(numero_pagina_destino);
				 uint32_t entrada_2do_nivel_destino = obtener_entrada_1er_nivel(numero_pagina_destino);
				 uint32_t desplazamiento_destino = obtener_desplazamiento(parametro1,numero_pagina_destino);

				 send_numero_pagina(fd_memoria,numero_pagina);
				 send_tabla_primer_nivel_pcb();
				 send_entrada_1er_nivel(fd_memoria,entrada_1er_nivel_destino);

				 recv_tabla_2do_nivel(fd_memoria,&entrada_1er_nivel_destino);

				 send_entrada_2do_nivel(fd_memoria,entrada_2do_nivel_destino);

				 recv_marco(fd_memoria,&marco_destino);
				 send_desplazamiento(fd_memoria,desplazamiento_destino);
				 uint32_t rta_mem;
				 recv_rta_memoria(fd_memoria,&rta_mem);
				 if(rta_mem == 0){
					 log_error(log_cpu, "La direccion donde se quiso copiar no es valida");
				}
				 else{
					 correr_algoritmo_reemplazo(numero_pagina,marco_destino);
			}
		}
	}

	 void correr_tlb_write(uint32_t numero_pagina,uint32_t parametro1,uint32_t parametro2){

		 	 bool existe_entrada(void* elem){
		 		 return ((tlb*) elem)->numero_pag == numero_pagina;
		 	 }


	 		 tlb* entrada = list_find(lista_tlb, &existe_entrada);
	 		 if(entrada != NULL){
	 		 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
	 		 traer_pag_de_tlb(entrada,parametro1);
	 		 send_valor(fd_memoria,parametro2);
	 		 }
	 		 else{
	 		 //send condicion a memoria para que sepa que le voy a pasar todo lo de abajo
	 		 uint32_t marco = -1;
	 		 uint32_t entrada_1er_nivel = obtener_entrada_1er_nivel(numero_pagina);
	 		 uint32_t entrada_2do_nivel = obtener_entrada_1er_nivel(numero_pagina);
	 		 uint32_t desplazamiento = obtener_desplazamiento(parametro1,numero_pagina);

	 		send_numero_pagina(fd_memoria,numero_pagina);
			send_tabla_primer_nivel_pcb();
			send_entrada_1er_nivel(fd_memoria,entrada_1er_nivel);

			recv_tabla_2do_nivel(fd_memoria,&entrada_1er_nivel);

			send_entrada_2do_nivel(fd_memoria,entrada_2do_nivel);

			recv_marco(fd_memoria,&marco);
			send_desplazamiento(fd_memoria,desplazamiento);
			send_valor(fd_memoria,parametro2);

			uint32_t rta_mem;
			 recv_rta_memoria(fd_memoria,&rta_mem);
			 if(rta_mem == 0){
				 log_error(log_cpu, "La direccion donde se quiso escribir no es valida");
			}
			 else{
				 correr_algoritmo_reemplazo(numero_pagina,marco);
		  }
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
				// log_warning(log_cpu,"Entre en READ");
				 pcb->PC += 1;
				 break;
			}
			 case COPY:{
				 tiempo_bloqueante = 0;
					log_info(log_cpu,"[EXE] ejecuto COPY");
				// log_warning(log_cpu,"Entre en COPY");
				 pcb->PC += 1;
				 break;
			}
			 case WRITE:{
				 tiempo_bloqueante = 0;
					log_info(log_cpu,"[EXE] ejecuto WRITE");
				// log_warning(log_cpu,"Entre en WRITE");
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

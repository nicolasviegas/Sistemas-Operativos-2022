#include "../include/comunicacion_memoria.h"
//#include "../include/cpu.h"
//#include "../include/funciones_cpu.h"


static void procesar_conexion_memoria_kernel(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

    fd_kernel = cliente_socket;

    send_TAM(cliente_socket,tamanio_paginas);

    send_TAM(cliente_socket,cant_entradas_por_tabla);


    while (cliente_socket != -1) {
    	//log_trace(log_memoria,"Estoy dentro del while cliente socket");

    		uint32_t condicion;
    		//log_trace(log_memoria,"la condicion antes del recv es: %d",condicion);
    	    if (recv(cliente_socket, &condicion, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
    		log_info(log_memoria, "DISCONNECT!");
    		return;
    	    }

    	    if(condicion == 666){
    	    	uint32_t tam_proceso;
				//log_trace(log_memoria,"la condicion antes del recv es: %d",tam_proceso);
				if (recv(cliente_socket, &tam_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "DISCONNECT!");
				return;
				}

				log_debug(log_memoria,"El tamanio del proceso recibido por kernel es: %d",tam_proceso);

			 //lo vamos a  hacer con tam = 512 para que tenga que ocupar 2 tablas de 2do nivel

			 t_list* lista_paginas_del_proceso; //todo liberar, ESTA LISTA tiene la cantidad de paginas totales del proceso
			 lista_paginas_del_proceso = dividir_proceso_en_paginas(tam_proceso);

			 //log_debug(log_memoria,"El size de la lista de paginas del proceso es: %d",list_size(lista_paginas_del_proceso));

			 t_list* tabla_1er_nivel_del_proceso;
			 tabla_1er_nivel_del_proceso = colocar_paginas_en_tabla(lista_paginas_del_proceso);


			 uint32_t indice_tabla = asignar_tabla_1er_nivel_a_proceso(tabla_1er_nivel_del_proceso);

			// log_debug(log_memoria,"El size de la tabla de primer nivel es: %d",list_size(tabla_1er_nivel_del_proceso));
			 log_info(log_memoria,"Asigno la tabla de primer nivel al proceso");

			 if(indice_tabla == -1){
				 log_error(log_memoria,"No hay mas entradas para tablas de primer nivel");
			 }else{
				 send_indice_a_kernel(fd_kernel,indice_tabla);
				 log_info(log_memoria,"envio el inice de la tabla a kernel %d", indice_tabla);
			 }

			char* path_char = pasar_a_char(indice_tabla);

			//log_debug(log_memoria,"El path  char es: %s",path_char);

			crear_archivo(path_char);

			char *ruta_archivo = string_new();
				string_append_with_format(&ruta_archivo,"%s/",path_swap);
				string_append(&ruta_archivo, path_char);

			poner_archivo_con_ceros(ruta_archivo,tam_proceso);

			free(ruta_archivo);

    	    }

    		if(condicion == TLB_RD){
    			//log_warning(log_memoria, "Voy a recibir de TLB READ ");

    			//send_TAM(cliente_socket,TLB_CPY);
    ////
    			uint32_t encontro_en_tabla;
				if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir si lo encontro o no!");
					return;
				}

				if(encontro_en_tabla == 1234){


					uint32_t marco_aux;
					if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir el marco!");
						return;
					}
					log_trace(log_memoria,"El marco que recibi de CPU para leer es: %d",marco_aux);


					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir el desplazamiento!");
						return;
					}
					//log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);


					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir el tamanio!");
						return;
					}
					//log_trace(log_memoria,"El desplazamienmto es: %d",tamanio);



					uint32_t valor_leido = leer_de_memoria(marco_aux,desplazamiento);
					actualizar_bit_uso_tlb(marco_aux);

					send_TAM(cliente_socket,valor_leido);
					log_info(log_memoria,"Le mando a CPU el valor leido");
					//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");




				}else{
							log_info(log_memoria,"Tengo que ir a buscar la pagina que me pidio CPU");


							uint32_t indice_tabla;
							if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}


							log_trace(log_memoria,"El indice de la tabla 1er nivel del proceso %d es: %d",indice_tabla+1,indice_tabla);

							uint32_t entrada_1er_nivel;
							if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

							uint32_t tamanio;
							if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
						//	log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);


							usleep(retardo_memoria*1000);


							log_info(log_memoria,"Obtengo el indice de la tabla de segundo nivel");
							uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
							send_TAM(cliente_socket,indice_tabla_segundo_nivel);
							log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

							uint32_t entrada_2do_nivel;
							if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

							pagina* pagina_buscada;
							pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);


							uint32_t desplazamiento;
							if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}

							uint32_t valor_leido;
							if(pagina_buscada->bit_presencia == 1){
								log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);

								valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
								pagina_buscada->bit_uso = 1;
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,valor_leido);
							//	log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								//log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
								log_info(log_memoria,"Le mando a CPU el valor leido");
							}
							else{
								uint32_t frame_a_utilizar = buscar_frame_libre();

								if(frame_a_utilizar != -1){
									if(al_proceso_le_quedan_frames(indice_tabla)){
									//	log_error(log_memoria,"Entre ya que al proceso le quedan frames");

										//poner_pagina_en_marco(frame_a_utilizar,pagina_buscada,indice_tabla);
										log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);
										traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
										valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
										pagina_buscada->bit_uso = 1;
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,valor_leido);
										//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
										//log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
										log_info(log_memoria,"Le mando a CPU el valor leido");
									}
									else{
										if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
										//	log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
										//	log_info(log_memoria,"El indice tabla que voy a pasar por parametro es : %d",indice_tabla);
											//t_list* contenido_de_pagina = list_create();
											log_info(log_memoria,"La pagina buscada  es: %d",pagina_buscada->nro_pagina);
											t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
											ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
											list_destroy(contenido_de_pagina);
											valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
											pagina_buscada->bit_uso = 1;
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,valor_leido);
										//	log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
											//log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);
											log_info(log_memoria,"Le mando a CPU el valor leido");

										}
							   }
						   }

					}
				}

/////

    		}
    		if(condicion == TLB_CPY){
    			//log_warning(log_memoria, "Voy a recibir de TLB CPY ");

    			uint32_t encontro_en_tabla;
				if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir si lo encontro o no!");
					return;
				}

				if(encontro_en_tabla == 1234){

					uint32_t marco_aux;
					if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"El marco recibido de CPU es: %d",marco_aux);


					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir el desplazamiento!");
						return;
					}
					//log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir el tamanio!");
						return;
					}
				//	log_trace(log_memoria,"El desplazamienmto es: %d",tamanio);


					actualizar_bit_uso_tlb(marco_aux);
					actualizar_bit_modif_tlb(marco_aux);

					send_TAM(cliente_socket,18);
					//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");





					}else{
					uint32_t indice_tabla;
					if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}

					log_trace(log_memoria,"El indice de la tabla 1er nivel del proceso %d es: %d",indice_tabla+1,indice_tabla);

					uint32_t entrada_1er_nivel;
					if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);

					uint32_t tamanio;
					if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					//log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);



					uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
					usleep(retardo_memoria*1000);
					send_TAM(cliente_socket,indice_tabla_segundo_nivel);
					log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

					uint32_t entrada_2do_nivel;
					if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

					pagina* pagina_buscada;
					pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);

					uint32_t desplazamiento;
					if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir nro de pagina!");
						return;
					}
					//log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);



					if(pagina_buscada->bit_presencia == 1){

						//valor_leido = leer_de_memoria(pagina_buscada->frame,desplazamiento);
						pagina_buscada->bit_uso = 1;
						pagina_buscada->bit_modificado = 1;
						send_TAM(cliente_socket,18);
						//log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");

						send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
						log_trace(log_memoria,"le envio a CPU el frame %d para que actualice la TLB ",pagina_buscada->frame);
					}
					else{
						uint32_t frame_a_utilizar = buscar_frame_libre();

						if(frame_a_utilizar != -1){
							if(al_proceso_le_quedan_frames(indice_tabla)){
								//log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
								traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
								pagina_buscada->bit_uso = 1;
								pagina_buscada->bit_modificado = 1;
								send_TAM(cliente_socket,18);
								//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								log_trace(log_memoria,"le envio a CPU el frame %d para que actualice la TLB ",pagina_buscada->frame);
							}
							else{
								if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
								//	log_error(log_memoria,"Entre ya que el proceso tiene al menos un pag en memoria");
									t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
									ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
									list_destroy(contenido_de_pagina);
									pagina_buscada->bit_uso = 1;
									pagina_buscada->bit_modificado = 1;
									send_TAM(cliente_socket,18);
								//	log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
									usleep(retardo_memoria*1000);
									send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
									log_trace(log_memoria,"le envio a CPU el frame %d para que actualice la TLB ",pagina_buscada->frame);
							}
					   }
				   }

			}


	}




    		}
    		if(condicion == TLB_WR){
					//log_warning(log_memoria, "Voy a recibir de TLB WRITE ");

					uint32_t encontro_en_tabla;
					if (recv(cliente_socket, &encontro_en_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
						log_info(log_memoria, "fallo al recibir si lo encontro o no!");
						return;
					}

					if(encontro_en_tabla == 1234){

						uint32_t marco_aux;
						if (recv(cliente_socket, &marco_aux, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						//log_trace(log_memoria,"El marco es 1: %d",marco_aux);


						uint32_t desplazamiento;
						if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						//log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

						uint32_t tamanio;
						if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						//log_trace(log_memoria,"El tamanio: %d",tamanio);

						uint32_t valor;
							if (recv(cliente_socket, &valor, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
						}
						log_trace(log_memoria,"El valor a escribir es: %d",valor);

//						//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");

						escribir_pagina(valor,marco_aux,desplazamiento);

						actualizar_bit_uso_tlb(marco_aux);
						actualizar_bit_modif_tlb(marco_aux);

						send_TAM(cliente_socket,100); // el 100 es por que se escribio correctamente
						//log_warning(log_memoria,"Le mande un 18 a cpu (vino por TLB)");


					}else{
							uint32_t indice_tabla;
							if (recv(cliente_socket, &indice_tabla, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}

							log_trace(log_memoria,"El indice de la tabla 1er nivel del proceso %d es: %d",indice_tabla+1,indice_tabla);

							uint32_t entrada_1er_nivel;
							if (recv(cliente_socket, &entrada_1er_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 1er nivel es: %d",entrada_1er_nivel);


							uint32_t tamanio;
							if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"El tamanio del proceso: %d",tamanio);




							uint32_t indice_tabla_segundo_nivel = obtener_nro_tabla_2do_nivel(indice_tabla,entrada_1er_nivel);
							usleep(retardo_memoria*1000);
							send_TAM(cliente_socket,indice_tabla_segundo_nivel);
							log_trace(log_memoria,"El indice de la tabla 2do nivel es: %d",indice_tabla_segundo_nivel);

							uint32_t entrada_2do_nivel;
							if (recv(cliente_socket, &entrada_2do_nivel, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							log_trace(log_memoria,"La entrada de 2do nivel es: %d",entrada_2do_nivel);

							pagina* pagina_buscada;
							pagina_buscada = buscar_pagina_en_tabla_2do_nivel(indice_tabla_segundo_nivel,entrada_2do_nivel);
//							log_trace(log_memoria,"el marco es: %d",pagina_buscada->frame);

							uint32_t desplazamiento;
							if (recv(cliente_socket, &desplazamiento, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
								log_info(log_memoria, "fallo al recibir nro de pagina!");
								return;
							}
							//log_trace(log_memoria,"El desplazamienmto es: %d",desplazamiento);

							uint32_t valor;
							if (recv(cliente_socket, &valor, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
							log_info(log_memoria, "fallo al recibir nro de pagina!");
							return;
							}
							log_trace(log_memoria,"El valor a escribir es: %d",valor);


							if(pagina_buscada->bit_presencia == 1){

								//log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);

								escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
								pagina_buscada->bit_uso = 1;
								pagina_buscada->bit_modificado = 1;
								send_TAM(cliente_socket,100);
								//log_warning(log_memoria,"Le mande un 18 a cpu(La pagina ya estaba en memoria)");
								usleep(retardo_memoria*1000);
								send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
								log_trace(log_memoria,"el marco es 2: %d",pagina_buscada->frame);
							}
							else{
								uint32_t frame_a_utilizar = buscar_frame_libre();

								if(frame_a_utilizar != -1){
									if(al_proceso_le_quedan_frames(indice_tabla)){
								//		log_error(log_memoria,"Entre ya que al proceso le quedan frames");
									//	log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);
										traer_proceso_de_swap(indice_tabla,tamanio,pagina_buscada,frame_a_utilizar);
										escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
										pagina_buscada->bit_uso = 1;
										pagina_buscada->bit_modificado = 1;
										send_TAM(cliente_socket,100);
										//log_warning(log_memoria,"Le mande un 18 a cpu (al proceso le quedaban frames)");
										usleep(retardo_memoria*1000);
										send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
										log_trace(log_memoria,"el marco es 3: %d",pagina_buscada->frame);
									}
									else{
										if(el_proceso_tiene_almenos_una_pag_en_mem(indice_tabla)){
											t_list* contenido_de_pagina = traer_pagina_de_swap(indice_tabla,pagina_buscada->nro_pagina,tamanio);
											ejecutar_reemplazo(contenido_de_pagina,pagina_buscada,indice_tabla);
											list_destroy(contenido_de_pagina);
											//log_info(log_memoria,"La pagina en la que voy a escribir con la instruccion que me dieron es: %d",pagina_buscada->nro_pagina);

											escribir_pagina(valor,pagina_buscada->frame,desplazamiento);
											pagina_buscada->bit_uso = 1;
											pagina_buscada->bit_modificado = 1;
											send_TAM(cliente_socket,100);
											//log_warning(log_memoria,"Le mande un 18 a cpu (tuvo que reemplazar)");
											usleep(retardo_memoria*1000);
											send_TAM(cliente_socket,pagina_buscada->frame);//aca hay que pasar el marco en vez del 7
											log_trace(log_memoria,"el marco es 2: %d",pagina_buscada->frame);
									}
							   }
						   }

					}

				}
		}

    		if(condicion == METER_A_SWAP){
    			uint32_t indice_proceso;
				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir nro de pagina!");
				return;
				}
				log_trace(log_memoria,"Meto a swap al proceso es: %d",indice_proceso+1);

				pasar_proceso_a_swap(indice_proceso);

				send_TAM(cliente_socket,1773);//esto lo mando a kernel para avisarle que ya lo meti a swap
    		}

    		if(condicion == METER_EN_MEM_PRINCIPAL){
			uint32_t indice_proceso;
			if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
			log_info(log_memoria, "fallo al recibir nro de pagina!");
			return;
			}

		//	log_trace(log_memoria,"Meto a mem principal el proceso: %d",indice_proceso + 1);


			if(!el_proceso_tiene_almenos_una_pag_en_mem(indice_proceso)){
				//log_error(log_memoria,"El proceso es nuevo asi que lo meto de una a mem ppal");
			}else{
				//log_error(log_memoria,"El proceso no es nuevo asi que ya lo trajo swap");
			}

			send_TAM(cliente_socket,5555); //le aviso a kernel que ya cargue el proceso

			}

      		if(condicion == SACAR_DE_SWAP){
        			uint32_t indice_proceso;
    				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
    				log_info(log_memoria, "fallo al recibir nro de pagina!");
    				return;
    				}
    				log_trace(log_memoria,"Saco de swap el proceso: %d",indice_proceso + 1);

    				send_TAM(cliente_socket,1773);//esto lo mando a kernel para avisarle que ya lo meti a swap
        		}
      		if(condicion == SACAR_PROCESO_DE_MEMORIA){
				uint32_t indice_proceso;
				if (recv(cliente_socket, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir nro de pagina!");
				return;
				}
				log_trace(log_memoria,"Saco de memoria el proceso: %d",indice_proceso+1);


				sacar_proceso_de_memoria(indice_proceso);
				send_TAM(cliente_socket,1773);//esto lo mando a cpu para avisarle que ya lo meti a swap
			}

      		if(condicion == COPIAR_PAGINAS){
				uint32_t marco_origen;
				if (recv(cliente_socket, &marco_origen, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir marco_origen!");
				return;
				}
				log_trace(log_memoria,"El marco origen a cambiar es: %d",marco_origen );


				uint32_t desplazamiento_origen;
				if (recv(cliente_socket, &desplazamiento_origen, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir desplazamiento_origen!");
				return;
				}
				//log_trace(log_memoria,"El desplazamiento origen a cambiar es: %d",desplazamiento_origen );

				uint32_t marco_destino;
				if (recv(cliente_socket, &marco_destino, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir marco_destino!");
				return;
				}
				log_trace(log_memoria,"El marco destino a cambiar es: %d",marco_destino );


				uint32_t desplazamiento_destino;
				if (recv(cliente_socket, &desplazamiento_destino, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
				log_info(log_memoria, "fallo al recibir desplazamiento_destino!");
				return;
				}
				//log_trace(log_memoria,"El desplazamiento destino a cambiar es: %d",desplazamiento_destino );

				uint32_t tamanio;
				if (recv(cliente_socket, &tamanio, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
					log_info(log_memoria, "fallo al recibir nro de pagina!");
					return;
				}


				copiar_en_memoria(marco_origen,desplazamiento_origen,marco_destino,desplazamiento_destino);

      		}







    }

    log_warning(log_memoria, "El cliente se desconecto de %s server", server_name);


    return;
}



static void procesar_conexion_memoria_cpu(void* void_args) {

    t_procesar_conexion_args* args = (t_procesar_conexion_args*) void_args;
    t_log* log_kernel = args->log;
    int cliente_socket = args->fd;
    char* server_name = args->server_name;
    free(args);

   // fd_kernel = 7;
    fd_cpu = 5;
   // send_TAM(fd_cpu,tamanio_paginas);
    bool ya_lo_envie = false;

   // log_error(log_memoria,"El cliente socket en procesar cpu es: %d",cliente_socket);

    log_warning(log_memoria, "El cliente se desconecto de %s server", server_name);


    return;
}



int server_escuchar_memoria(t_log* logger, char* server_name, int server_socket) {
    int cliente_socket = esperar_cliente(logger, server_name, server_socket);

    if (cliente_socket != -1) {
        pthread_t hilo;
        pthread_t hilo2;

        t_procesar_conexion_args* args = malloc(sizeof(t_procesar_conexion_args));
        args->log = logger;
        args->fd = cliente_socket;
        args->server_name = server_name;


        t_procesar_conexion_args* args2 = malloc(sizeof(t_procesar_conexion_args));
                args2->log = logger;
                args2->fd = cliente_socket;
                args2->server_name = server_name;


        pthread_create(&hilo, NULL, (void*) procesar_conexion_memoria_kernel, (void*) args);
        pthread_detach(hilo);


        return 1;
    }
    return 0;
}

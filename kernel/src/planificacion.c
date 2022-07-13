#include "../include/planificacion.h"


int obtener_algoritmo(char* algoritmo_char){

	 t_algoritmo_planificacion switcher;


	    //HRRN
	    if (strcmp(algoritmo_char,"FIFO") == 0)
	    {
	        switcher = FIFO;
	        //log_info(log_kernel, "El algoritmo de planificacion elegido es FIFO.");
	    }

	    //SFJ SIN DESALOJO
	    if (strcmp(algoritmo_char,"SRT") == 0)
	    {
	        switcher = SRT;
	     //   log_info(log_kernel, "El algoritmo de planificacion elegido es SRT.");
	    }
	    return switcher;
}

float diferencia_de_tiempo(float tiempoInicial, float tiempoFinal){

	return tiempoFinal - tiempoInicial;
}


//////////////////////////////////////////////// COLAS ////////////////////////////////////////////////

void agregarANew(pcb_t* proceso) {

	pthread_mutex_lock(&mutexNew);

	queue_push(colaNew, proceso);
	log_info(log_kernel, "[NEW] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexNew);

	//log_trace(log_kernel,"Antes de analizar suspension en agregar a NEW ");

	sem_post(&analizarSuspension); // Despierta al planificador de mediano plazo
	//log_trace(log_kernel,"Despues del post de analizar suspension en agregar a NEW ");

	sem_wait(&suspensionFinalizada); // Espera a que ya se haya hecho, o no, la suspension

	//log_trace(log_kernel,"despues de suspension finalizada en agregar a NEW ");

	sem_post(&contadorNew); // Despierta al planificador de largo plazo
	sem_post(&largoPlazo);

	//log_error(log_kernel,"Salgo de agregar a NEW tranqui");
}

pcb_t* sacarDeNew(){

	sem_wait(&contadorNew);
	pthread_mutex_lock(&mutexNew);

	pcb_t* proceso = queue_pop(colaNew);
	log_info(log_kernel, "[NEW] Se saca el proceso de PID: %d de la cola", proceso->PID);

	pthread_mutex_unlock(&mutexNew);

	return proceso;
}

void agregarAReady(pcb_t* proceso){
	//log_trace(log_kernel,"Entre en agregar a ready");

	time_t a = time(NULL);

	pthread_mutex_lock(&mutexReady);

	proceso->suspendido = false;
	list_add(colaReady, proceso);

	log_info(log_kernel, "[READY] Entra el proceso de PID: %d a la cola.", proceso->PID);
	send_TAM(fd_memoria,METER_EN_MEM_PRINCIPAL);
	send_TAM(fd_memoria,proceso->indice_tabla_paginas);

	uint32_t indice_proceso; //ESTE RECV ES PARA SABER SI MEMORIA YA TERMINO DE PASAR A SWAP AL PROCESO SUSPENDIDO
	if (recv(fd_memoria, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
	log_info(log_kernel, "fallo al recibir nro de pagina!");
	return;
	}if(indice_proceso == 5555){
		log_trace(log_kernel,"Memoria termino de meter al proceso");
	}


	//printf("PROCESOS EN READY: %d \n", list_size(colaReady));
	log_debug(log_kernel,"[----------------PROCESOS EN READY: %d --------------------]\n", list_size(colaReady));

	pthread_mutex_unlock(&mutexReady);
	sem_post(&contadorReady);
	//sem_post(&contadorProcesosEnMemoria); Lo sacamos de aca para usarlo en el contexto en el que se llama a la funcion, porque no siempre que se agrega a ready, se toca la multiprogramacion
}


void agregarABlock(pcb_t* proceso){		//ver semaforos
	//log_trace(log_kernel,"Entre en agregar a block ");
	//log_info(log_kernel, "ADSSSS el proceso %d tiene el PC en %d",proceso->PID,proceso->PC);

	bool tienenMismoPID(void* elemento){

		if(proceso->PID == ((pcb_t *) elemento)->PID)
			return true;
		else
			return false;
	}


	pthread_mutex_lock(&mutexBlock);

	list_add(listaBlock, proceso);
	log_info(log_kernel, "[BLOCK] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);
	sem_post(&contadorBlock);

	sem_post(&analizarSuspension);

	sem_wait(&suspensionFinalizada);
}

void sacarDeBlock(pcb_t* proceso){

	sem_wait(&contadorBlock);

	bool tienenMismoPID(void* elemento){

		if(proceso->PID == ((pcb_t*) elemento)->PID)
			return true;
		else
			return false;
	}

	pthread_mutex_lock(&mutexBlock);

	list_remove_by_condition(listaBlock, tienenMismoPID);
	log_info(log_kernel, "[BLOCK] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);

}


void agregarABlockSuspended(pcb_t* pcb){
//	log_trace(log_kernel,"Entre en agregar a block suspended");
	pthread_mutex_lock(&mutexBlockSuspended);

	pcb->suspendido = true;
	list_add(listaBlockSuspended, pcb);

	log_info(log_kernel, "[BLOCK-SUSPENDED] Ingresa el pcb de PID: %d a la cola.", pcb->PID);

	pthread_mutex_unlock(&mutexBlockSuspended);

	send_TAM(fd_memoria,METER_A_SWAP); //HAY QUE DESCOMENTAR Y HACER EL RECV
	send_TAM(fd_memoria,pcb->indice_tabla_paginas);

	uint32_t indice_proceso; //ESTE RECV ES PARA SABER SI MEMORIA YA TERMINO DE PASAR A SWAP AL PROCESO SUSPENDIDO
	if (recv(fd_memoria, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
	log_info(log_kernel, "fallo al recibir nro de pagina!");
	return;
	}if(indice_proceso == 1773){
		log_trace(log_kernel,"Memoria termino de meter a swap al proceso");
	}


	agregarAReadySuspended(pcb); //ESTO LO COMENTAMOS NOSOTROS

}


void sacarDeBlockSuspended(pcb_t* proceso){

	bool tienenMismoPID(void* elemento){

	if(proceso->PID == ((pcb_t *) elemento)->PID)
		return true;
	else
		return false;
	}

	pthread_mutex_lock(&mutexBlockSuspended);

	list_remove_by_condition(listaBlockSuspended, tienenMismoPID);
	log_info(log_kernel, "[BLOCK-SUSPENDED] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlockSuspended);
}

void agregarAReadySuspended(pcb_t* proceso){
	//log_trace(log_kernel,"Entre en agregar a ready suspended");

	pthread_mutex_lock(&mutexReadySuspended);

	queue_push(colaReadySuspended, proceso);
	log_info(log_kernel, "[READY-SUSPENDED] Ingresa el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	sem_post(&contadorReadySuspended);
	sem_post(&medianoPlazo);
}

pcb_t* sacarDeReadySuspended(){
	//log_trace(log_kernel,"Entre en sacar de ready suspended");
	sem_wait(&contadorReadySuspended);

	pthread_mutex_lock(&mutexReadySuspended);

	pcb_t* proceso = queue_pop(colaReadySuspended);
	proceso->suspendido = false;
	log_info(log_kernel, "[READY-SUSPENDED] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	//log_error(log_kernel,"[===========] agregue a ready desde sacar de ready suspended");
	agregarAReady(proceso);//todo ESTE AGREGAR A READY ES EL QUE CAMBIAMOS PARA QUE FINALICE ACORDE A LAS PRUEBAS, CUALQUIER COSA COMENTAR ESTE Y DESCOMENTAR EL QUE QUEDO MARCADO

	return proceso;
}

// ----------------------------------HILOS---------------------------------------------------//

//------------CASO FIFO-----------//
// Hilo que maneja pasar los procesos de new a ready	-	CASO FIFO

void hiloNew_Ready(){

	while(1){
		//log_error(log_kernel,"Entre en hilo New Ready");
		sem_wait(&largoPlazo);

		if(queue_size(colaReadySuspended) != 0){

			sem_post(&medianoPlazo);
		}else{

			pcb_t* proceso = sacarDeNew();

			proceso->estimacionAnterior = estimacion_inicial;
			proceso->estimacionActual = estimacion_inicial;	//"estimacio_inicial" va a ser una variable que vamos a obtener del cfg

			sem_wait(&multiprogramacion); //HAY QUE VER DONDE PONER EL POST DE ESTE SEM, PORQUE SE QUEDA TRABADO EN EL LVL MAX DE MULTIPROGRAMACION
			//log_error(log_kernel,"[===========] agregue a ready desde hilo new ready");
			agregarAReady(proceso);


			if(hay_alguien_exe ){
			if(algoritmo_config == SRT){

						//log_debug(log_kernel,"Despues de agregar a ready tendria que mandar la interrupcion");
						//  log_debug(log_kernel,"Entre en send interrupcion en hilo new ready, Proceso nuevo 777");
				if(list_size(colaReady)>1){
					log_debug(log_kernel,"Envio una interrupcion");

					send_interrupcion(fd_cpu_interrupt,777); ///777 es que hay una interrupcion
					}
			}
			}

			sem_post(&contadorProcesosEnMemoria);
		}
	}
}

// Hilo que maneja los procesos de Ready a Execute     -    CASO SRT
void hiloReady_Exe(){

	//while(list_size(colaReady)>0){
	while(1){
		//log_trace(log_kernel,"Entre en hilo ready exe");

		pthread_mutex_lock(&multiprocesamiento);

		//log_debug(log_kernel,"[[[[LA COLA READY TIENE %d ELEMENTOS antes de obtener sig ready",list_size(colaReady));
		pcb_t* procesoAEjecutar = obtenerSiguienteDeReady();
		//log_debug(log_kernel,"[[[[LA COLA READY TIENE %d ELEMENTOS despues de obtener sig ready",list_size(colaReady));


		// Aca se crea un hilo de cpu y se le pasa ese pcb, cuando el proceso hace mate_close se pasa el pcb a EXIT y se mata el hilo

		if(procesoAEjecutar != NULL) {


			if(algoritmo_config == SRT){
				log_info(log_kernel, "[EXEC] Ingresa el proceso de PID: %d con una rafaga de ejecucion estimada de %f milisegundos.", procesoAEjecutar->PID, procesoAEjecutar->estimacionActual);
				time_t a = time(NULL);
				procesoAEjecutar->horaDeIngresoAExe = ((float) a)*1000;
			}else{
				log_info(log_kernel, "[EXEC] Ingresa el proceso de PID: %d , que era el primero que llego", procesoAEjecutar->PID);

			}

			hay_alguien_exe = true;


			enviar_pcb_a_cpu(procesoAEjecutar);


	//		log_trace(log_kernel,"ANTES DE RECIBIR EL PC DE CPU");

			uint32_t pc;
			if (!recv_PC(fd_cpu, &pc)) {
				log_error(log_kernel, "Fallo recibiendo pc");
			}

			//log_trace(log_kernel,"DESPUS DE RECIBIR EL PC DE CPU");

			procesoAEjecutar->PC = pc;


			//EL CPU ME DEVUELVE EL TIEMPO QUE SE VA A BLOQUEAR EL PROCESO POR E/S
			if (!recv_tiempo_bloqueante(fd_cpu, &tiempo_bloq_kernel)) {
				log_error(log_kernel, "Fallo recibiendo el tiempo bloqueante");
			}
			//log_trace(log_kernel,"El tiempo bloqueante despues del recv es: %d",tiempo_bloq_kernel);

			procesoAEjecutar->tiempo_bloqueo = tiempo_bloq_kernel;

			time_t b = time(NULL);
			float tiempoDeFin = ((float) b)*1000;
			procesoAEjecutar->rafagaAnterior = diferencia_de_tiempo(procesoAEjecutar->horaDeIngresoAExe, tiempoDeFin);

			procesoAEjecutar->estimacionActual = alfa*(procesoAEjecutar->rafagaAnterior) + (1-alfa)*(procesoAEjecutar->estimacionAnterior);
			procesoAEjecutar->estimacionAnterior = procesoAEjecutar->estimacionActual;


			if(procesoAEjecutar->tiempo_bloqueo > 0){
				agregarABlock(procesoAEjecutar);
				sem_post(&analizarSuspension); // Despues de que un proceso se va de Ready y hace su transicion, se analiza la suspension
				sem_wait(&suspensionFinalizada);
			}else{
				if(procesoAEjecutar->PC < list_size(procesoAEjecutar->instrucciones)){
					//log_error(log_kernel,"[===========] agregue a ready desde hilo ready exe");
					agregarAReady(procesoAEjecutar);
				}else{
					terminarEjecucion(procesoAEjecutar);
					sem_post(&multiprogramacion);//esto lo agg nosotros
				}

			}

			pthread_mutex_unlock(&multiprocesamiento);

			hay_alguien_exe = false;


		}
	}
}


// Hilo que maneja la suspension de procesos
void hiloBlockASuspension(){

	while(true){

		sem_wait(&analizarSuspension);

		//log_trace(log_kernel,"Entre en hilo bloq a suspension");



		if(list_size(listaBlock) != 0){

			pcb_t* pcb = list_get(listaBlock,0);
			sacarDeBlock(pcb);

			if(pcb->tiempo_bloqueo <= tiempo_max_bloqueado){

						log_info(log_kernel,"[BLOCK] Se bloquea el proceso %d, %d milisegs, ",pcb->PID,pcb->tiempo_bloqueo);

						usleep(pcb->tiempo_bloqueo*1000);

						//sem_post(&medianoPlazo); //esto para desbloquear el hilo suspension a ready
						//log_error(log_kernel,"[===========] agregue a ready desde sacar de hilo block a suspension");
						agregarAReady(pcb);
						//log_debug(log_kernel,"Despues de agregar a ready tendria que mandar la interrupcion, en block a suspension");
////////////////////////////////////////////////////////////////////////////-----------------------/////////////////////////
						if(hay_alguien_exe){
							if(algoritmo_config == SRT){

								//log_debug(log_kernel,"Despues de agregar a ready tendria que mandar la interrupcion");
								//  log_debug(log_kernel,"Entre en send interrupcion en hilo new ready, Proceso nuevo 777");
								if(list_size(colaReady)>1){
									log_debug(log_kernel,"Envio una interrupcion");

									send_interrupcion(fd_cpu_interrupt,777); ///777 es que hay una interrupcion

								}

								}else{

								}
							}

////////////////////////////////////////////////////////////////////////////-----------------------/////////////////////////

					}else{//sino solo lo bloqueo y lo devuelvo a ready
						log_info(log_kernel,"[SUSPENDED] Se suspende el proceso %d, %d milisegs, ",pcb->PID,pcb->tiempo_bloqueo);

						usleep(tiempo_max_bloqueado*1000);

						sem_wait(&contadorProcesosEnMemoria);

						agregarABlockSuspended(pcb);

						sem_post(&multiprogramacion);
					}

		}


		sem_post(&suspensionFinalizada);
	}
}

void hiloSuspensionAReady(){

	while(1){
		//log_trace(log_kernel,"Entre en hilo suspencion a ready");
		sem_wait(&medianoPlazo);

		if(queue_size(colaReadySuspended) == 0){

			sem_post(&largoPlazo);
		}else{

		pcb_t* proceso = sacarDeReadySuspended();

		sem_wait(&multiprogramacion);

		send_TAM(fd_memoria,SACAR_DE_SWAP);
		send_TAM(fd_memoria,proceso->indice_tabla_paginas);

		uint32_t indice_proceso; //ESTE RECV ES PARA SABER SI MEMORIA YA TERMINO DE PASAR A SWAP AL PROCESO SUSPENDIDO
		if (recv(fd_memoria, &indice_proceso, sizeof(uint32_t), 0) != sizeof(uint32_t)) {
		log_info(log_kernel, "fallo al recibir nro de pagina!");
		return;
		}if(indice_proceso == 1773){
			log_trace(log_kernel,"Memoria termino de sacar de swap al proceso");
		}



	//	agregarAReady(proceso); //TODO DATO IMPORTANTE, SI ESTE AGREGAR LO DESCOMENTAMOS EL I/O FUNCIONA COMO ERA AL PRINCIPIO, NO TERMINAN EN FIFO

		if(hay_alguien_exe){
					if(algoritmo_config == SRT){

								//log_debug(log_kernel,"Despues de agregar a ready tendria que mandar la interrupcion");
								  log_debug(log_kernel,"Envio una interrupcion");

									send_interrupcion(fd_cpu_interrupt,777); ///777 es que hay una interrupcion
								}else{
									//log_debug(log_kernel,"Entre en send interrupcion");

								}
					}else{

					}

		sem_post(&contadorProcesosEnMemoria);
		}
	}
}


bool condiciones_de_suspension(){

	int cantidadDeProcesosEnMemoria;

	sem_getvalue(&contadorProcesosEnMemoria, &cantidadDeProcesosEnMemoria);

	pthread_mutex_lock(&mutexBlock);
	pthread_mutex_lock(&mutexReady);
	pthread_mutex_lock(&mutexNew);

	bool respuesta = false; //si en memoria estan

	pthread_mutex_unlock(&mutexNew);
	pthread_mutex_unlock(&mutexReady);
	pthread_mutex_unlock(&mutexBlock);

	return respuesta;
}

pcb_t* obtenerSiguienteDeReady(){

//	log_trace(log_kernel,"Entre en obtener sig de ready");
	sem_wait(&contadorReady);

	pcb_t* procesoPlanificado = NULL;

	int tamanioDeColaReady(){

		int tamanio;

		pthread_mutex_lock(&mutexReady);
		tamanio = list_size(colaReady);
		pthread_mutex_unlock(&mutexReady);

		return tamanio;
	}

	if (tamanioDeColaReady() > 0){

		// Aca dentro un SWITCH para los distintos algoritmos q llama a una funcion para cada uno
		switch(algoritmo_config){


			//	log_trace(log_kernel,"EL ALGORITMO DE PLANIF ES: %d",algoritmo_config);
				//CASO FIFO
				case FIFO:
					procesoPlanificado = obtenerSiguienteFIFO();
				break;

				//CASO SJF sin desalojo
				case SRT:
					procesoPlanificado = obtenerSiguienteSJF();
				break;

			  }
	}

	// Devuelve NULL si no hay nada en ready
	// Caso contrario devuelve el que tiene mas prioridad segun el algoritmo que se este empleando
	return procesoPlanificado;
}

pcb_t* obtenerSiguienteFIFO(){

	pcb_t* procesoPlanificado = NULL;

	pthread_mutex_lock(&mutexReady);
	procesoPlanificado = list_remove(colaReady, 0);
    pthread_mutex_unlock(&mutexReady);

	return procesoPlanificado;
}


pcb_t* obtenerSiguienteSJF(){

	pcb_t* procesoPlanificado = NULL;
	pcb_t* procesoAux = NULL;
    int i;
	int indexARemover;
	float shortestJob;

	pthread_mutex_lock(&mutexReady);
	procesoAux = list_get(colaReady,0);
	pthread_mutex_unlock(&mutexReady);

	indexARemover = 0;
	shortestJob = procesoAux->estimacionActual;

	//itero por la lista de Ready
	//sem_wait(&contadorReady);
	pthread_mutex_lock(&mutexReady);

//	printf("[----------------PROCESOS EN READY: %d --------------------]\n", list_size(colaReady));

	//log_debug(log_kernel,"[----------------PROCESOS EN READY: %d --------------------]\n", list_size(colaReady));

    for(i=0;i<list_size(colaReady);i++){
    	procesoAux = list_get(colaReady,i);
    	log_info(log_kernel,"El proceso %d, tiene una estimacion actual de: %f",procesoAux->PID,procesoAux->estimacionActual);
    	if(shortestJob > procesoAux->estimacionActual){
    		shortestJob = procesoAux->estimacionActual;
    		indexARemover = i;
    	}

    }

    procesoPlanificado = list_remove(colaReady, indexARemover);


    pthread_mutex_unlock(&mutexReady);

	log_warning(log_kernel,"El proceso %d fue elegido mediante el algoritmo SRT ",procesoPlanificado->PID);

	return procesoPlanificado;
}

void terminarEjecucion(pcb_t* pcb){




	pthread_mutex_lock(&mutexExit);

	list_add(listaExit, pcb);
	log_info(log_kernel, "[EXIT] Finaliza el proceso de PID: %d", pcb->PID);

	pthread_mutex_unlock(&mutexExit);

	size_t size = sizeof(op_estados)+sizeof(uint32_t);

	void* stream = malloc(size);

	op_estados opCode = FINISH;


	sem_post(&contadorProcesosEnMemoria);
	free(stream);
}

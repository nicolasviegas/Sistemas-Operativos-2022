#include "../include/planificacion.h"


int obtener_algoritmo(char* algoritmo_char){

	 t_algoritmo_planificacion switcher;


	    //HRRN
	    if (strcmp(algoritmo_char,"FIFO") == 0)
	    {
	        switcher = FIFO;
	        log_info(log_kernel, "El algoritmo de planificacion elegido es FIFO.");
	    }

	    //SFJ SIN DESALOJO
	    if (strcmp(algoritmo_char,"SRT") == 0)
	    {
	        switcher = SRT;
	        log_info(log_kernel, "El algoritmo de planificacion elegido es SRT.");
	    }
	    return switcher;
}



//////////////////////////////////////////////// COLAS ////////////////////////////////////////////////

void agregarANew(pcb_t* proceso) {

	pthread_mutex_lock(&mutexNew);

	queue_push(colaNew, proceso);
	log_info(log_kernel, "[NEW] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexNew);

	log_error(log_kernel,"Entre en agregar a NEW ");

	sem_post(&analizarSuspension); // Despierta al planificador de mediano plazo
	sem_wait(&suspensionFinalizada); // Espera a que ya se haya hecho, o no, la suspension

	sem_post(&contadorNew); // Despierta al planificador de largo plazo
	sem_post(&largoPlazo);

	log_error(log_kernel,"Salgo de agregar a NEW tranqui");
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
	log_trace(log_kernel,"Entre en agregar a ready");

	time_t a = time(NULL);
	//proceso->horaDeIngresoAReady = ((float) a)*1000;
	proceso->tiempoEspera = 0;

	pthread_mutex_lock(&mutexReady);

	proceso->suspendido = false;
	list_add(colaReady, proceso);

	log_info(log_kernel, "[READY] Entra el proceso de PID: %d a la cola.", proceso->PID);

	printf("PROCESOS EN READY: %d \n", list_size(colaReady));

	pthread_mutex_unlock(&mutexReady);
	sem_post(&contadorReady);
	//sem_post(&contadorProcesosEnMemoria); Lo sacamos de aca para usarlo en el contexto en el que se llama a la funcion, porque no siempre que se agrega a ready, se toca la multiprogramacion
}


void agregarABlock(pcb_t* proceso){		//ver semaforos
	log_trace(log_kernel,"Entre en agregar a block ");

	sem_wait(&contadorExe);

	bool tienenMismoPID(void* elemento){

		if(proceso->PID == ((pcb_t *) elemento)->PID)
			return true;
		else
			return false;
	}

	list_remove_by_condition(listaExe, tienenMismoPID);

	pthread_mutex_lock(&mutexBlock);

	list_add(listaBlock, proceso);
	log_info(log_kernel, "[BLOCK] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);
	//sem_post(&multiprocesamiento);
	sem_post(&contadorBlock);

	sem_post(&analizarSuspension);
	// como funciona el analizar suspension?
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
	log_trace(log_kernel,"Entre en agregar a block suspended");
	pthread_mutex_lock(&mutexBlockSuspended);

	pcb->suspendido = true;
	list_add(listaBlockSuspended, pcb);

	log_info(log_kernel, "[BLOCK-SUSPENDED] Ingresa el pcb de PID: %d a la cola.", pcb->PID);

	pthread_mutex_unlock(&mutexBlockSuspended);

	size_t size = sizeof(op_estados)+sizeof(uint32_t);

	void* stream = malloc(size);

	op_estados opCode = SUSPENDED_BLOCKED;

	memcpy(stream, &opCode, sizeof(op_estados));
	memcpy(stream + sizeof(op_estados), &(pcb->PID), sizeof(uint32_t));

	//send(fd_memoria, stream, size, 0); HAY QUE DESCOMENTAR Y HACER EL RECV

	free(stream);
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
	log_trace(log_kernel,"Entre en agregar a ready suspended");

	pthread_mutex_lock(&mutexReadySuspended);

	queue_push(colaReadySuspended, proceso);
	log_info(log_kernel, "[READY-SUSPENDED] Ingresa el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	sem_post(&contadorReadySuspended);
	sem_post(&medianoPlazo);
}

pcb_t* sacarDeReadySuspended(){
	log_trace(log_kernel,"Entre en sacar de ready suspended");
	sem_wait(&contadorReadySuspended);

	pthread_mutex_lock(&mutexReadySuspended);

	pcb_t* proceso = queue_pop(colaReadySuspended);
	proceso->suspendido = false;
	log_info(log_kernel, "[READY-SUSPENDED] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	return proceso;
}

// ----------------------------------HILOS---------------------------------------------------//

//------------CASO FIFO-----------//
// Hilo que maneja pasar los procesos de new a ready	-	CASO FIFO

void hiloNew_Ready(){

	while(1){
		log_error(log_kernel,"Entre en hilo New Ready");
		sem_wait(&largoPlazo);

		if(queue_size(colaReadySuspended) != 0){

			sem_post(&medianoPlazo);
		}else{

			pcb_t* proceso = sacarDeNew();

			proceso->estimacionAnterior = estimacion_inicial;
			proceso->estimacionActual = estimacion_inicial;	//"estimacio_inicial" va a ser una variable que vamos a obtener del cfg
			proceso->tiempoEspera = 0;

			sem_wait(&multiprogramacion); //HAY QUE VER DONDE PONER EL POST DE ESTE SEM, PORQUE SE QUEDA TRABADO EN EL LVL MAX DE MULTIPROGRAMACION
			agregarAReady(proceso);
			sem_post(&contadorProcesosEnMemoria);
		}

		log_error(log_kernel,"Sali hilo new ready ");
	}
}

// Hilo que maneja los procesos de Ready a Execute     -    CASO SRT
void hiloReady_Exe(){

	while(1){
		log_trace(log_kernel,"Entre en hilo ready exe");
		//sem_wait(&multiprocesamiento);

		pcb_t* carpinchoAEjecutar = obtenerSiguienteDeReady();

	/*	int a = list_size(carpinchoAEjecutar->instrucciones);
		//log_trace(log_kernel,"El tam de la lista de instruc del proceso es: %d",a);
		printf("El tam de la lista de instruc del proceso a ejecutar es: %d",a);*/

		// Aca se crea un hilo de cpu y se le pasa ese pcb, cuando el carpincho hace mate_close se pasa el pcb a EXIT y se mata el hilo

		if(carpinchoAEjecutar != NULL) {

			pthread_mutex_lock(&mutexExe);
			list_add(listaExe, carpinchoAEjecutar);
			pthread_mutex_unlock(&mutexExe);

			////////////////////////////// VER COMO HACER PARA QUE LO HAGA CPU Y NO KERNEL (HAY QUE ENVIAR A CPU Y QUE EL EJECUTE)
			pthread_t hiloCPU;
			pthread_create(&hiloCPU, NULL, (void*) enviar_pcb_a_cpu, (void*) carpinchoAEjecutar); //Esta funcion ejecutar seria la que se hace en cpu
			pthread_detach(hiloCPU);
			//enviar_pcb_a_cpu(carpinchoAEjecutar);

			if(algoritmo_config == SRT){
				log_info(log_kernel, "[EXEC] Ingresa el proceso de PID: %d con una rafaga de ejecucion estimada de %f milisegundos.", carpinchoAEjecutar->PID, carpinchoAEjecutar->estimacionActual);
			}else{
				log_info(log_kernel, "[EXEC] Ingresa el proceso de PID: %d , que era el primero que llego", carpinchoAEjecutar->PID);

			}

			sem_post(&contadorExe);

			sem_post(&analizarSuspension); // Despues de que un carpincho se va de Ready y hace su transicion, se analiza la suspension
			sem_wait(&suspensionFinalizada);

		}else{
			//sem_post(&multiprocesamiento);

		}
	}
}

//Hilo que maneja la finalizacion de procesos
void hiloExecAExit(){
	log_trace(log_kernel,"Entre en hilo exec a exit");
	sem_wait(&contadorExe);
	sem_wait(&contadorProcesosEnMemoria);
	//sem_wait(&hilo_sincro_cpu_kernel);

	pcb_t* proceso = list_get(listaExe,0);



	uint32_t pc;
	if (!recv_PC(fd_cpu, &pc)) {
		log_error(log_kernel, "Fallo recibiendo pc");
	}
	log_error(log_kernel,"El PC despues del recv es: %d",pc);

	proceso->PC = pc;


	log_trace(log_kernel,"El pc del exit proceso de list_get es: %d",proceso->PC);


	pthread_mutex_lock(&mutexExe);
	list_remove(listaExe,0);
	pthread_mutex_unlock(&mutexExe);

	pthread_mutex_lock(&mutexExe);

	terminarEjecucion(proceso);

	pthread_mutex_unlock(&mutexExe);

	sem_post(&multiprogramacion);
}

// Hilo que maneja la suspension de procesos
void hiloBlockASuspension(){

	while(true){
		log_trace(log_kernel,"Entre en hilo bloq a suspension");
		sem_wait(&analizarSuspension);

		if(condiciones_de_suspension()){  //ESTO EVALUA SI HAY QUE BLOQUEARLO O  NO DEPENDIENDO DEL GRADO DE MULTIPROGRAMACION

			sem_wait(&contadorProcesosEnMemoria);

			pcb_t* pcb = list_get(listaBlock, list_size(listaBlock) - 1);
			sacarDeBlock(pcb);

			agregarABlockSuspended(pcb);

			sem_post(&multiprogramacion);
		}

		sem_post(&suspensionFinalizada);
	}
}

void hiloSuspensionAReady(){

	while(1){
		log_trace(log_kernel,"Entre en hilo suspencion a ready");
		sem_wait(&medianoPlazo);

		if(queue_size(colaReadySuspended) == 0){

			sem_post(&largoPlazo);
		}else{

		pcb_t* proceso = sacarDeReadySuspended();

		sem_wait(&multiprogramacion);

		agregarAReady(proceso);

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

	bool respuesta = (list_size(listaBlock) != 0 &&
			list_size(colaReady) == 0 &&
			queue_size(colaNew) != 0 &&
			cantidadDeProcesosEnMemoria == grado_multiprogramacion);

	pthread_mutex_unlock(&mutexNew);
	pthread_mutex_unlock(&mutexReady);
	pthread_mutex_unlock(&mutexBlock);

	return respuesta;
}

pcb_t* obtenerSiguienteDeReady(){

	log_trace(log_kernel,"Entre en obtener sig de ready");
	sem_wait(&contadorReady);

	pcb_t* carpinchoPlanificado = NULL;

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
					carpinchoPlanificado = obtenerSiguienteFIFO();
				break;

				//CASO SJF sin desalojo
				case SRT:
					carpinchoPlanificado = obtenerSiguienteSJF();
				break;

			  }
	}

	// Devuelve NULL si no hay nada en ready
	// Caso contrario devuelve el que tiene mas prioridad segun el algoritmo que se este empleando
	return carpinchoPlanificado;
}

pcb_t* obtenerSiguienteFIFO(){

	log_warning(log_kernel,"Esto en obtenre siguiente fifo");
	pcb_t* carpinchoPlanificado = NULL;

	pthread_mutex_lock(&mutexReady);
	carpinchoPlanificado = list_remove(colaReady, 0);
    pthread_mutex_unlock(&mutexReady);

	return carpinchoPlanificado;

	//FREE??
}


pcb_t* obtenerSiguienteSJF(){

	pcb_t* carpinchoPlanificado = NULL;
	pcb_t* carpinchoAux = NULL;
    int i;
	int indexARemover;
	float shortestJob;

	pthread_mutex_lock(&mutexReady);
	carpinchoAux = list_get(colaReady,0);
	pthread_mutex_unlock(&mutexReady);

	indexARemover = 0;
	shortestJob = carpinchoAux->estimacionActual;

	//itero por la lista de Ready
	//sem_wait(&contadorReady);
	pthread_mutex_lock(&mutexReady);

	printf("PROCESOS EN READY: %d \n", list_size(colaReady));

    for(i=1;i<list_size(colaReady);i++){
    	carpinchoAux = list_get(colaReady,i);

    	if(shortestJob > carpinchoAux->estimacionActual){
    		shortestJob = carpinchoAux->estimacionActual;
    		indexARemover = i;
    	}

    }

    carpinchoPlanificado = list_remove(colaReady, indexARemover);

    pthread_mutex_unlock(&mutexReady);

	return carpinchoPlanificado;
}

void terminarEjecucion(pcb_t* pcb){

	pthread_mutex_lock(&mutexExit);

	list_add(listaExit, pcb);
	log_info(log_kernel, "[EXIT] Finaliza el carpincho de PID: %d", pcb->PID);

	pthread_mutex_unlock(&mutexExit);

	size_t size = sizeof(op_estados)+sizeof(uint32_t);

	void* stream = malloc(size);

	op_estados opCode = FINISH;

	memcpy(stream, &opCode, sizeof(op_estados));
	memcpy(stream + sizeof(op_estados), &(pcb->PID), sizeof(uint32_t));

	send(fd_memoria, stream, size, 0);


	free(stream);
}

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
	    if (strcmp(algoritmo_char,"SJF") == 0)
	    {
	        switcher = SRT;
	        log_info(log_kernel, "El algoritmo de planificacion elegido es SJF.");
	    }
	    return switcher;
}



//////////////////////////////////////////////// COLAS ////////////////////////////////////////////////

void agregarANew(pcb_t* proceso) {

	pthread_mutex_lock(&mutexNew);

	queue_push(colaNew, proceso);
	log_info(log_kernel, "[NEW] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexNew);

	sem_post(&analizarSuspension); // Despierta al planificador de mediano plazo
	sem_wait(&suspensionFinalizada); // Espera a que ya se haya hecho, o no, la suspension

	sem_post(&contadorNew); // Despierta al planificador de largo plazo
	sem_post(&largoPlazo);
}

//------------CASO FIFO-----------//
void hiloNew_Ready(){

	while(1){

		sem_wait(&largoPlazo);

		if(queue_size(colaReadySuspended) != 0){

			sem_post(&medianoPlazo);
		}else{

			pcb_t* proceso = sacarDeNew();

			proceso->estimacionAnterior = estimacion_inicial;
			proceso->estimacionActual = estimacion_inicial;	//"estimacio_inicial" va a ser una variable que vamos a obtener del cfg
			proceso->tiempoEspera = 0;

			sem_wait(&multiprogramacion);
			agregarAReady(proceso);
			sem_post(&contadorProcesosEnMemoria);
		}
	}
}

pcb_t* sacarDeNew(){

	sem_wait(&contadorNew);
	pthread_mutex_lock(&mutexNew);

	pcb_t* proceso = queue_pop(colaNew);
	log_info(logger, "[NEW] Se saca el proceso de PID: %d de la cola", proceso->PID);

	pthread_mutex_unlock(&mutexNew);

	return proceso;
}

void agregarAReady(pcb_t* proceso){

	time_t a = time(NULL);
	//proceso->horaDeIngresoAReady = ((float) a)*1000;
	proceso->tiempoEspera = 0;

	pthread_mutex_lock(&mutexReady);

	proceso->suspendido = false;
	list_add(colaReady, proceso);
	log_info(logger, "[READY] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReady);
	sem_post(&contadorReady);
	//sem_post(&contadorProcesosEnMemoria); Lo sacamos de aca para usarlo en el contexto en el que se llama a la funcion, porque no siempre que se agrega a ready, se toca la multiprogramacion
}


void agregarABlock(pcb_t* proceso){		//ver semaforos

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
	log_info(logger, "[BLOCK] Entra el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);
	sem_post(&multiprocesamiento);
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
	log_info(logger, "[BLOCK] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);

}


void agregarASuspended(pcb_t* proceso){

	pthread_mutex_lock(&mutexBlockSuspended);

	proceso->suspendido = true;
	list_add(listaBlockSuspended, proceso);

	log_info(logger, "[BLOCK-SUSPENDED] Ingresa el proceso de PID: %d a la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlockSuspended);

	/*size_t size = sizeof(sem_code)+sizeof(uint32_t);

	void* stream = malloc(size);

	sem_code opCode = SUSPEND;

	memcpy(stream, &opCode, sizeof(sem_code));
	memcpy(stream + sizeof(sem_code), &(proceso->PID), sizeof(uint32_t)); // FALTA AGREGAR SEM_CODE EN LAS COMUNICACIONES

	send(proceso->socketMemoria, stream, size, 0);

	free(stream);*/
}


void agregarAReadySuspended(pcb_t* proceso){

	pthread_mutex_lock(&mutexReadySuspended);

	queue_push(colaReadySuspended, proceso);
	log_info(logger, "[READY-SUSPENDED] Ingresa el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	sem_post(&contadorReadySuspended);
	sem_post(&medianoPlazo);
}

pcb_t* sacarDeReadySuspended(){

	sem_wait(&contadorReadySuspended);

	pthread_mutex_lock(&mutexReadySuspended);

	pcb_t* proceso = queue_pop(colaReadySuspended);
	proceso->suspendido = false;
	log_info(logger, "[READY-SUSPENDED] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	return proceso;
}

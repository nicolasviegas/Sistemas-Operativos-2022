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

		log_error(log_kernel,"Sali hilo new ready ");
	}
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

	time_t a = time(NULL);
	//proceso->horaDeIngresoAReady = ((float) a)*1000;
	proceso->tiempoEspera = 0;

	pthread_mutex_lock(&mutexReady);

	proceso->suspendido = false;
	list_add(colaReady, proceso);
	log_info(log_kernel, "[READY] Entra el proceso de PID: %d a la cola.", proceso->PID);

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
	log_info(log_kernel, "[BLOCK] Entra el proceso de PID: %d a la cola.", proceso->PID);

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
	log_info(log_kernel, "[BLOCK] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexBlock);

}


void agregarASuspended(pcb_t* proceso){

	pthread_mutex_lock(&mutexBlockSuspended);

	proceso->suspendido = true;
	list_add(listaBlockSuspended, proceso);

	log_info(log_kernel, "[BLOCK-SUSPENDED] Ingresa el proceso de PID: %d a la cola.", proceso->PID);

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
	log_info(log_kernel, "[READY-SUSPENDED] Ingresa el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	sem_post(&contadorReadySuspended);
	sem_post(&medianoPlazo);
}

pcb_t* sacarDeReadySuspended(){

	sem_wait(&contadorReadySuspended);

	pthread_mutex_lock(&mutexReadySuspended);

	pcb_t* proceso = queue_pop(colaReadySuspended);
	proceso->suspendido = false;
	log_info(log_kernel, "[READY-SUSPENDED] Sale el proceso de PID: %d de la cola.", proceso->PID);

	pthread_mutex_unlock(&mutexReadySuspended);

	return proceso;
}


void agregarABlockSuspended(pcb_t* pcb){

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


// Hilo que maneja la suspension de procesos
void hiloBlockASuspension(){

	while(true){

		sem_wait(&analizarSuspension);

	/*	if(condiciones_de_suspension()){  ESTO EVALUA SI HAY QUE BLOQUEARLO O  NO DEPENDIENDO DEL GRADO DE MULTIPROGRAMACION

			sem_wait(&contadorProcesosEnMemoria);

			pcb_t* pcb = list_get(listaBlock, list_size(listaBlock) - 1);
			sacarDeBlock(pcb);

			agregarABlockSuspended(pcb);

			sem_post(&multiprogramacion);
		}*/

		sem_post(&suspensionFinalizada);
	}
}

void hiloSuspensionAReady(){

	while(1){

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

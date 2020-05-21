/*
 * Flight Management Computer
 * Will emulate the speed sensor data based on the thrust setting.
*/
/* LIBRARIES */
#define _GNU_SOURCE
#include "aircraft.h"			// Custom library
#include <stdio.h>
#include <stdint.h>
#include <time.h>			// used for periodic speed sensor reading
#include <linux/sched.h>		// used to fetch SCHED_DEADLINE
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sched.h>
#include <semaphore.h>

/* CONSTANTS */
const long SAMPLE_PERIOD = 5992704;	// time interval between two consecutive samples, in nanoseconds!
const long SAMPLE_RUNTIME = 55540;

// speed emulation
// minimum weight
//const double MASS =	49000;

// maximum weight
const double MASS = 	79000;		// aircraft mass, in Kg

// program
const size_t SHARED_MEMORY_SIZE = 128;
/* GLOBAL VARIABLES */

// speed emulation
double drag;				// air resistance against aircraft
double speed;				// most recent speed result

// aircraft values
double thrust;
double altitude;
double finalSpeed;

// program
pthread_t ctrl_t;
pthread_attr_t ctrl_attr;
void *shared_memory;
double *sh_mem_values;

pthread_t fdr_t;
pthread_mutex_t sh_mem_mutex;

/* FUNCTIONS */
// CALCULATIONS

/**
 * Calculates speed based on equation 3.
*/
double f_get_speed() {
	return speed + (thrust + drag)/(MASS/(10000*10000))*to_seconds(SAMPLE_PERIOD);
}

/**
 * Calculates drag based on equation 2.
*/

double calculate_drag(double h) {
	return -100000+1.87*h;
}

// FMC RELATED
/*
 * If real sensors were being used, this method would be responsible 
 * for communicating with them, however, it now functions as an
 * emulator.
*/
void read_sensors() {
	speed = f_get_speed();
	//printf("[FMC] SPEED\t\tTHRUST\t\tDRAG\t\tMASS\t\tINTERVAL\n");
	//printf("[FMC] %f\t%f\t%f\t%f\t%d\n\n", speed, thrust, drag, MASS, SAMPLE_PERIOD);
}

void send_info_to_fdr() {
	// start thread
	pthread_create(&fdr_t, NULL, send_to_fdr_speed, (void *)&speed);
	pthread_create(&fdr_t, NULL, send_to_fdr_thrust, (void *)&thrust);
}

void loop() {
	while(1) {
		//printf("\e[1;1H\e[2J");

		// get new thrust value
		memcpy(sh_mem_values, shared_memory, sizeof(*shared_memory));
		thrust = sh_mem_values[1];
		
		//  read sensors
		read_sensors();

		// send speed to control algorithm
		sh_mem_values[0] = speed;
		memcpy(shared_memory, sh_mem_values, sizeof(*sh_mem_values));
		

		// send data to the flight data recorder
		send_info_to_fdr();
		

	}
}

void *create_shared_memory(size_t size) {
	int protection = PROT_READ | PROT_WRITE;
	int visibility = MAP_SHARED | MAP_ANONYMOUS;
	return mmap(NULL, size, protection, visibility, -1, 0);
}

void start_ctrl_thread() {
	// initialize ctrl thread attributes
	pthread_attr_init(&ctrl_attr);

	// setup ctrl thread with real-time requirements
	pthread_attr_setschedpolicy(&ctrl_attr, SCHED_DEADLINE);

	// start thread
	pthread_create(&ctrl_t, &ctrl_attr, start_ctrl, &finalSpeed);
}


/* MAIN FUNCTION */
int main(int argc, char** argv) {
	// prevent that memory is paged to the swap area
	mlockall(MCL_CURRENT);

	// initialize and configure
	struct sched_attr attr;
	attr.size = sizeof(attr);
	attr.sched_policy = SCHED_DEADLINE;
	attr.sched_runtime = SAMPLE_RUNTIME;
	attr.sched_period = SAMPLE_PERIOD;	// in nanoseconds
	attr.sched_deadline = attr.sched_period;

	// set current thread as real-time
	sched_setattr(getpid(), &attr, 0);

	if(argc<3) {
		perror("Not enough arguments. Altitude, initial speed and final speed must be given to execute\n");
		return 0;
	}
	// get arguments and assign them to global variables
	speed = 	km_h_to_m_s(strtod(argv[2], NULL));
	finalSpeed = 	km_h_to_m_s(strtod(argv[3], NULL));

	// calculate drag based on altitude
	drag = calculate_drag(strtod(argv[1], NULL));
	printf("[FMC] initial altitude is %f\n", strtod(argv[1], NULL));	
	printf("[FMC] initial drag is %f\n", drag);
	printf("[FMC] final speed = %f\n", finalSpeed);
	
	

	// create shared memory between this program and it's children
	printf("[FMC] creating shared memory between FMC and CTRL...\n");
	shared_memory = create_shared_memory(SHARED_MEMORY_SIZE);
	printf("[FMC] done!\n");

	// send initial speed to shared memory
	double temp[2];
	temp[0] = speed;
	sh_mem_values = temp;

	printf("[FMC] initial speed = %f\n",speed);
	pthread_mutex_lock(&sh_mem_mutex);
	memcpy(shared_memory, sh_mem_values, sizeof(*sh_mem_values));
	pthread_mutex_unlock(&sh_mem_mutex);
	
	// creating control algorithm thread and sharing memory 
	// between the flight management computer and the control 
	// algorithm.
	printf("[FMC] starting CTRL thread...\n");
	start_ctrl_thread();
	printf("[FMC] done!\n");

	

	// iniitalize main loop
	printf("[FMC] starting main loop.\n");
	loop();

	// join threads
	printf("[FMC] joining threads...\n");
	pthread_join(ctrl_t, NULL);
	pthread_mutex_destroy(&sh_mem_mutex);

	return 0;
}

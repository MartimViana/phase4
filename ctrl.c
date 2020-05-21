#include "aircraft.h"
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/sched.h>

// constants
const long CTRL_PERIOD = 5992704;
const long CTRL_RUNTIME = 55540;
const double SUCCESS_MARGIN = 0.05;

// PID controller
double P;
double I;
double D;

// global variables
//program
void *shared_memory;
pthread_mutex_t sh_mem_mutex;
double *sh_mem_values;

// emulation
double thrust;
double speed;
double finalSpeed;
const double MAX_THRUST = 242000;
const double MIN_THRUST = 0;
const double MAX_VELOCITY = 2962800000;
const double MIN_VELOCITY = 1080000000;
double SENSITIVITY = 100000;

// PID controller
double previousError = 0;
double integral = 0;
const double maintenanceError = 0.05;

void pid_iteration() {
	double error = finalSpeed - speed;
	integral = integral + error;
	double derivative = error - previousError;
	double output = P*error+I*integral+D*derivative;
	thrust = output / SENSITIVITY;
	
	previousError = error;
}

void f_set_thrust() {
	// change thrust
	pid_iteration();
	//printf("[CTRL] Setting thrust to %f\n", thrust);
	if (thrust < MIN_THRUST) {thrust = MIN_THRUST;}
	if (thrust > MAX_THRUST) {thrust = MAX_THRUST;}
	//if(speed > MAX_VELOCITY) {printf("[CTRL] Maximum Exceeded!\n");}
	//if(speed < MIN_VELOCITY) {printf("[CTRL] Minimum Exceeded!\n");}
	printf("\e[1;1H\e[2J");
	//printf("P = %f\nI = %f\nD = %f\n", P, I, D);
	printf("[CTRL] Error = %f\n", (1-(double) speed/finalSpeed)*100);
	//printf("[CTRL] Mainenance error = %f\n", maintenanceError*100);
	// send thrust to flight management control
	sh_mem_values[1] = thrust;
	memcpy(shared_memory, &sh_mem_values, sizeof(*sh_mem_values));
}

void setup_thread() {
	struct sched_attr attr;
	attr.size = sizeof(attr);
	attr.sched_policy = SCHED_DEADLINE;
	attr.sched_runtime = CTRL_RUNTIME;
	attr.sched_period = CTRL_PERIOD;
	attr.sched_deadline = attr.sched_period;

	sched_setattr(getpid(), &attr, 0);
}



void *start_ctrl(double *arg) {
	setup_thread();

	finalSpeed = *arg;
	P = 0.9;
	I = 0;
	D = 1;
	SENSITIVITY = 1000;

	while(1) {
		
		// receive current speed
		//printf("[CTRL] reading shared memory and assigning values\n");
		memcpy(sh_mem_values, shared_memory, sizeof(*shared_memory));
		speed = sh_mem_values[0];
		//printf("[CTRL] speed = %f\n", speed);

		// set thrust to shared memory
		f_set_thrust();
		
	}
	return NULL;
}

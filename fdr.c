#define _GNU_SOURCE
#include "aircraft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int SIZE = 2048;

void* send_to_fdr_speed(void *arg) {
	double y = *(double*) arg;
	double x = (double) clock() / 1000;
	// open file and set it ready to append data
	FILE* file = fopen("speed.temp", "a");

	// append speed and thrust to file
	fprintf(file, "%f %f\n", x, y);

	// close file
	fclose(file);
	return arg;
}

void* send_to_fdr_thrust(void *arg) {
	double y = *(double*) arg;
	double x = (double) clock() / 1000;
	// open file and set it ready to append data
	FILE* file = fopen("thrust.temp", "a");


	// append speed and thrust to file
	fprintf(file, "%f %f\n", x, y);

	// close file
	fclose(file);
	return arg;
}

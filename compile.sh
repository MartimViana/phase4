#!/bin/bash
#gcc fmc.c -o fmc.out -lpthread
#gcc ctrl.c -o ctrl.out -lpthreads
gcc -Wall ctrl.c fmc.c fdr.c -o aircraft.out -lpthread

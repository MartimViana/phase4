//
// Library partially based on http://disi.unitn.it/~abeni/RTOS/periodic_tasks.c
//
#ifndef AIRPLANE_DOT_H
#define AIRPLANE_DOT_H
#define _GNU_SOURCE
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <linux/sched.h>
#include <sys/types.h>
#define NSEC_PER_SEC 1000000000ULL
static const int DEBUG = 1;

#define SCHED_DEADLINE  6

/* __NR_sched_setattr number */
#ifndef __NR_sched_setattr
#ifdef __x86_64__
#define __NR_sched_setattr      314
#endif

#ifdef __i386__
#define __NR_sched_setattr      351
#endif

#ifdef __arm__
#define __NR_sched_setattr      380
#endif

#ifdef __aarch64__
#define __NR_sched_setattr      274
#endif
#endif

/* __NR_sched_getattr number */
#ifndef __NR_sched_getattr
#ifdef __x86_64__
#define __NR_sched_getattr      315
#endif

#ifdef __i386__
#define __NR_sched_getattr      352
#endif

#ifdef __arm__
#define __NR_sched_getattr      381
#endif

#ifdef __aarch64__
#define __NR_sched_getattr      275
#endif
#endif

struct sched_attr {
    __u32 size;

    __u32 sched_policy;
    __u64 sched_flags;

    /* SCHED_NORMAL, SCHED_BATCH */
    __s32 sched_nice;

    /* SCHED_FIFO, SCHED_RR */
    __u32 sched_priority;

    /* SCHED_DEADLINE */
    __u64 sched_runtime;
    __u64 sched_deadline;
    __u64 sched_period;
};

static inline int sched_setattr(pid_t pid,
              const struct sched_attr *attr,
              unsigned int flags)
{
    return syscall(__NR_sched_setattr, pid, attr, flags);
}

/*static int sched_getattr(pid_t pid,
              struct sched_attr *attr,
              unsigned int size,
              unsigned int flags)
{
    return syscall(__NR_sched_getattr, pid, attr, size, flags);
}*/


/**
 * Translates time from nanoseconds to seconds.
*/
static inline long double to_seconds(int nanoseconds) {
	return ((double) nanoseconds)/1000000000;
}

/*
	Command that initializes control algorithm in ctrl.c
*/
void* start_ctrl();

/**
	Command that initializes control algorithm in fdr.c
*/
void* send_to_fdr_speed(void *arg);
void* send_to_fdr_thrust(void *arg);

/**
 * Add an amount of time to a timespec.
*/
static inline void timespec_add_us(struct timespec *t, uint64_t d) {
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
	t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

static inline double km_h_to_m_s(double kmH) {
	return kmH * 1000 * 3600;
}

static inline double m_s_to_km_h(double mS) {
	return mS / (double) (1000 * 3600);
}

#endif /* AIRPLANE_DOT_H */

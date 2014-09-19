#ifndef THREAD_H
#define THREAD_H

#include "config.h"

#include <pthread.h>

#define THREAD_FLAG_SHUTDOWN 1

typedef struct thread {
	pthread_t pthread;
	void *param;
	void (*thread_func)(struct thread *);
	uint8_t flags;
} thread_t;

thread_t *thread_begin(void (*thread_func)(thread_t *),void *);
void thread_end(thread_t *);
void threads_maintain();

#endif


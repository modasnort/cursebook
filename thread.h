#ifndef THREAD_H
#define THREAD_H

#include "config.h"

#include <pthread.h>

#define THREAD_FLAG_SHUTDOWN 1

typedef pthread_mutex_t mutex_t;

typedef struct thread {
	pthread_t pthread;
	void *param;
	void (*thread_func)(struct thread *);
	uint8_t flags;
	mutex_t mutex;
} thread_t;

void threads_init();
void threads_maintain();
void threads_shutdown();

thread_t *thread_begin(void (*thread_func)(thread_t *),void *);
void thread_end(thread_t *);

void mutex_init(mutex_t *);
void mutex_lock(mutex_t *);
void mutex_unlock(mutex_t *);
void mutex_destroy(mutex_t *);

#endif


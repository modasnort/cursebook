#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

typedef struct thread {
	pthread_t pthread;
	void *param;
	void (*thread_func)(struct thread *);
} thread_t;

thread_t *thread_begin(void (*thread_func)(thread_t *),void *);
void thread_end(thread_t *);
void threads_wait();

#endif


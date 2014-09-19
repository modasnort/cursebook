#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "thread.h"
#include "ui.h"

mutex_t master_mutex;
thread_t **thread_pool=NULL;
uint8_t threads_count=0;

void threads_lock() {
	mutex_lock(&master_mutex);
}

void threads_unlock() {
	mutex_unlock(&master_mutex);
}

void mutex_init(mutex_t *mutex) {
	pthread_mutex_init(mutex,NULL);
}

void mutex_lock(mutex_t *mutex) {
	pthread_mutex_lock(mutex);
}

void mutex_unlock(mutex_t *mutex) {
	pthread_mutex_unlock(mutex);
}

void mutex_destroy(mutex_t *mutex) {
	pthread_mutex_destroy(mutex);
}

void *thread_wrapper_func(void *void_ptr) {
	thread_t *thread=void_ptr;
	thread->thread_func(thread);
	thread_end(thread);
}

thread_t *thread_begin(void (*thread_func)(thread_t *),void *param) {
	threads_lock();
	if (threads_count==0xff)
		ui_warn("thread pool overflow, denying thread");
	else {
		if (thread_pool==NULL)
			thread_pool=malloc(sizeof(thread_t *));
		else thread_pool=realloc(thread_pool,sizeof(thread_t *)*(threads_count+1));
		thread_pool[threads_count]=malloc(sizeof(thread_t));
		
		thread_t *ret=thread_pool[threads_count];
		memset(ret,0,sizeof(thread_t));
		
		mutex_init(&ret->mutex);
		
		ret->param=param;
		ret->thread_func=thread_func;

		if (!pthread_create(&ret->pthread, NULL, thread_wrapper_func, ret)) {
			threads_count++;
		}
		else {
			ui_warn("pthread_create failed");
			free(ret);
			if (threads_count>0)
				thread_pool=realloc(thread_pool,sizeof(thread_t *)*(threads_count));
			else {
				free(thread_pool);
				thread_pool=NULL;
			}
		}

	}
	threads_unlock();
}

void thread_end(thread_t *thread) {
	mutex_lock(&thread->mutex);
	thread->flags|=THREAD_FLAG_SHUTDOWN;
	mutex_unlock(&thread->mutex);
}

void threads_init() {
	mutex_init(&master_mutex);
}

void threads_maintain() {
	uint8_t ok;
	do {
		threads_lock();
		ok=threads_count>0;
		if (ok) {
			for (int i=0;i<threads_count;i++) {
				mutex_lock(&thread_pool[i]->mutex);
				if (hasflag(thread_pool[i]->flags,THREAD_FLAG_SHUTDOWN)) {
					pthread_join(thread_pool[i]->pthread,NULL);
					mutex_unlock(&thread_pool[i]->mutex);
					mutex_destroy(&thread_pool[i]->mutex);
					free(thread_pool[i]);
					int ii=i;
					while (ii<threads_count-1) {
						thread_pool[ii]=thread_pool[ii+1];
						ii++;
					}
					threads_count--;
					if (threads_count>0)
						thread_pool=realloc(thread_pool,sizeof(thread_t *)*threads_count);
					else {
						free(thread_pool);
						thread_pool=NULL;
					}
				}
				else
					mutex_unlock(&thread_pool[i]->mutex);
			}
		}
		threads_unlock();
		
		pi_usleep(100000);
	} while (ok);
}

void threads_shutdown() {
	mutex_destroy(&master_mutex);
}

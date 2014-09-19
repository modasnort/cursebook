#include <stdint.h>
#include <malloc.h>
#include <string.h>

#include "thread.h"
#include "ui.h"

thread_t **thread_pool=NULL;
uint8_t threads_count=0;

void *thread_wrapper_func(void *void_ptr) {
	thread_t *thread=void_ptr;
	thread->thread_func(thread);
	thread_end(thread);
}

thread_t *thread_begin(void (*thread_func)(thread_t *),void *param) {
	if (threads_count==0xff)
		ui_warn("thread pool overflow, denying thread");
	else {
		if (thread_pool==NULL)
			thread_pool=malloc(sizeof(thread_t *));
		else thread_pool=realloc(thread_pool,sizeof(thread_t *)*(threads_count+1));
		thread_pool[threads_count]=malloc(sizeof(thread_t));
		
		thread_t *ret=thread_pool[threads_count];
		memset(ret,0,sizeof(thread_t));
		
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
}

void thread_end(thread_t *thread) {
	thread->flags|=THREAD_FLAG_SHUTDOWN;
}

void threads_maintain() {
	while (threads_count>0) {
		
		for (int i=0;i<threads_count;i++) {
			if (hasflag(thread_pool[i]->flags,THREAD_FLAG_SHUTDOWN)) {
				pthread_join(thread_pool[i]->pthread,NULL);
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
		}
		
		pi_usleep(100000);
	}
}


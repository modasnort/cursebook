#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "thread.h"

thread_t *ui_thread;

mutex_t ui_mutex;
uiblock_t **uiblocks=NULL;
uint16_t uiblocks_count=0;

void ui_lock() {
	mutex_lock(&ui_mutex);
}

void ui_unlock() {
	mutex_unlock(&ui_mutex);
}

void ui_thread_func(thread_t *thread)
{

	int i=0;
	while (i++<10) {
		
		//uint8_t need_refresh=0;
			
		ui_lock();
		
		for (int ii=0;ii<uiblocks_count;ii++) {
			uiblock_t *uiblock=uiblocks[ii];
			mutex_lock(&uiblock->mutex);
			
			if (hasflag(uiblock->flags,UIBLOCK_FLAG_REDRAW)) {
				//printf("X %i\n",uiblocks_count);
				unsetflag(uiblock->flags,UIBLOCK_FLAG_REDRAW);
				
				if (uiblock->window==NULL) {
					uiblock->window=newwin(15,20,9,5);
					box(uiblock->window,0,0);
					//wborder(uiblock->window, '|', '|', '-', '-', '+', '+', '+', '+');
					wrefresh(uiblock->window);
//					refresh();
					//need_refresh=1;
				}
				else {
					// ...
				}
			}
			if (hasflag(uiblock->flags,UIBLOCK_FLAG_DESTROY)) {
				wborder(uiblock->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
				wrefresh(uiblock->window);
				delwin(uiblock->window);
				mutex_destroy(&uiblock->mutex);
				free(uiblock);
				int iii=ii;
				while (iii<uiblocks_count-1) {
					uiblocks[iii]=uiblocks[iii+1];
					iii++;
				}
				uiblocks_count--;
				ii--;
				//need_refresh=1;
			}
			
			//wrefresh(uiblock->window);
			
			mutex_unlock(&uiblock->mutex);
		}
		
		ui_unlock();
		
		//if (need_refresh) {
		//	printf("REFRESH\n");
			//refresh();
		//}

		pi_usleep(100000);
	}

}

void ui_init() {
	
	mutex_init(&ui_mutex);
	
	initscr();
#if ALLOW_CONTROL_C
	cbreak();
#else
	raw();
#endif
	noecho();
	keypad(stdscr,TRUE);
	
	curs_set(0);

	ui_thread=thread_begin(ui_thread_func,NULL);
}

void ui_shutdown() {
	endwin();
	
	mutex_destroy(&ui_mutex);
}

void ui_warn(char *text) {
	printf("warning: %s\n",text);
}

void uiblock_resize(uiblock_t *uiblock,box_t constraints) {
	mutex_lock(&uiblock->mutex);
	uiblock->box=constraints;
	setflag(uiblock->flags,UIBLOCK_FLAG_REDRAW);
	mutex_unlock(&uiblock->mutex);
}

uiblock_t *uiblock_create(box_t constraints) {
	uiblock_t *uiblock=malloc(sizeof(uiblock_t));
	memset(uiblock,0,sizeof(uiblock_t));

	mutex_init(&uiblock->mutex);
	
	uiblock_resize(uiblock,constraints);
	
	ui_lock();
	if (uiblocks==NULL)
		uiblocks=malloc(sizeof(uiblock_t *));
	else
		uiblocks=realloc(uiblocks,sizeof(uiblock_t *)*(uiblocks_count+1));
	uiblocks[uiblocks_count]=uiblock;
	uiblocks_count++;
	ui_unlock();
	
	return uiblock;
}

void uiblock_destroy(uiblock_t *uiblock) {
	mutex_lock(&uiblock->mutex);
	setflag(uiblock->flags,UIBLOCK_FLAG_DESTROY);
	mutex_unlock(&uiblock->mutex);
}

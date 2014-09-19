#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "thread.h"

struct thread *ui_thread;

uint8_t need_refresh=0;

void ui_thread_func(thread_t *thread)
{

	int i=0;
	while (i++<10) {

		if (need_refresh) {
			need_refresh=0;
			refresh();
		}

		pi_usleep(100000);
	}

}

void ui_init() {
	
	initscr();
#if ALLOW_CONTROL_C
	cbreak();
#else
	raw();
#endif
	noecho();
	keypad(stdscr,TRUE);
	
	need_refresh=1;
	ui_thread=thread_begin(ui_thread_func,NULL);
}

void ui_shutdown() {
	endwin();
}

void ui_warn(char *text) {
	printf("warning: %s\n",text);
}

void uiblock_resize(uiblock_t *uiblock,box_t constraints) {
	if (uiblock->window==NULL) {
		uiblock->window=newwin(5,3,1,5);
		box(uiblock->window,0,0);
		wborder(uiblock->window, '|', '|', '-', '-', '+', '+', '+', '+');
		wrefresh(uiblock->window);
		need_refresh=1;
	}
	else {
		// ...
	}
}

uiblock_t *uiblock_create(box_t constraints) {
	uiblock_t *uiblock=malloc(sizeof(uiblock_t));
	memset(uiblock,0,sizeof(uiblock_t));
	
	uiblock->box=constraints;
	
	uiblock_resize(uiblock,constraints);
	
	return uiblock;
}

void uiblock_destroy(uiblock_t *uiblock) {
	wborder(uiblock->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
	wrefresh(uiblock->window);
	delwin(uiblock->window);
	free(uiblock);
}

void uiblock_anchor(uiblock_t *uiblock,uint8_t anchor) {
	
}

void uiblock_unanchor(uiblock_t *uiblock) {
	
}


#include <ncurses.h>
#include <malloc.h>

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

		usleep(100000);
	}

}

void ui_init() {
	initscr();
	need_refresh=1;
	ui_thread=thread_begin(ui_thread_func,NULL);
}

void ui_shutdown() {
	thread_end(ui_thread);
	endwin();
}

void ui_warn(char *text) {
	printf("warning: %s\n",text);
}

uiblock_t *uiblock_create() {
	uiblock_t *uiblock=malloc(sizeof(uiblock_t));
	return uiblock;
}

void uiblock_destroy(uiblock_t *uiblock) {
	free(uiblock);
}

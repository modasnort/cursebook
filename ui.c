#include <ncurses.h>

#include "ui.h"
#include "thread.h"

struct thread *ui_thread;

void ui_thread_func(thread_t *thread)
{

	int i=0;
	while (i++<10) {

		

		usleep(100000);
	}

}

void ui_init() {
	initscr();
	ui_thread=thread_begin(ui_thread_func,NULL);
}

void ui_shutdown() {
	thread_end(ui_thread);
	endwin();
}

void ui_warn(char *text) {
	printf("warning: %s\n",text);
}

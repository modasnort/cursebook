#include <malloc.h>

#include "console.h"

#include "ui.h"

uiblock_t *consoleblock=NULL;

void *console_callback(uint8_t event,uiblock_t *block) {
	if (event==UI_EVENT_REALIGN) {
		block->box=t_box(screen_width*0.1,screen_height*0.5,screen_width*0.9,screen_height*0.9);
	}
	else if (event==UI_EVENT_REDRAW) {
		box(block->window,0,0);
	}
}

void console_init() {
	if (consoleblock==NULL) {
		consoleblock=uiblock_create(console_callback);
	}
	
}

void console_shutdown() {
	if (consoleblock!=NULL) {
		uiblock_destroy(consoleblock);
		consoleblock=NULL;
	}
}

void console_log(char *text) {
	
}

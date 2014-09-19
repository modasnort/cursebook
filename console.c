#include <malloc.h>

#include "console.h"

#include "ui.h"

uiblock_t *consoleblock=NULL;

void console_init() {
	if (consoleblock==NULL) {
		consoleblock=uiblock_create(t_box(1,2,3,4));
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

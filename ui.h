#ifndef UI_H
#define UI_H

#include "config.h"
#include "types.h"

#include <curses.h>
#include <stdint.h>

#include "thread.h"

#define UIBLOCK_FLAG_REDRAW 1
#define UIBLOCK_FLAG_DESTROY 2

typedef struct uiblock {

	box_t box;
	
	struct curseblock **children;
	uint16_t children_count;
	struct curseblock *parent;
	
	WINDOW *window;
	
	uint8_t flags;
	
	mutex_t mutex;
	
} uiblock_t;

void ui_init();
void ui_shutdown();
void ui_warn(char *);

uiblock_t *uiblock_create(box_t);
void uiblock_destroy(uiblock_t *);

void uiblock_resize(uiblock_t *,box_t);


#endif

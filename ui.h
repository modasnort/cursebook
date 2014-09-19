#ifndef UI_H
#define UI_H

#include "config.h"

#include "types.h"

#include <curses.h>

#include <stdint.h>

typedef struct uiblock {

	box_t box;
	
	struct curseblock **children;
	uint16_t children_count;
	struct curseblock *parent;
	
	WINDOW *window;

} uiblock_t;

void ui_init();
void ui_shutdown();
void ui_warn(char *);

uiblock_t *uiblock_create(box_t);
void uiblock_destroy(uiblock_t *);

void uiblock_resize(uiblock_t *,box_t);


#endif

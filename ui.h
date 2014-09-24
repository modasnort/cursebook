#ifndef UI_H
#define UI_H

#include "config.h"
#include "types.h"

#include <curses.h>
#include <stdint.h>

#include "thread.h"

#define UI_EVENT_REALIGN 1
#define UI_EVENT_REDRAW 2

#define UIBLOCK_FLAG_REALIGN 1
#define UIBLOCK_FLAG_REDRAW 2
#define UIBLOCK_FLAG_DESTROY 4

typedef struct uiblock {

	box_t box;
	
	struct uiblock **children;
	uint16_t children_count;
	struct uiblock *parent;
	
	WINDOW *window;
	
	uint8_t flags;
	
	mutex_t mutex;
	
	void *(*callback)(uint8_t,struct uiblock *);
} uiblock_t;

void ui_init();
void ui_shutdown();
void ui_warn(char *);

void uiblock_event(uint8_t,uiblock_t *);
void uiblock_event_recursive(uint8_t,uiblock_t *);

uiblock_t *uiblock_create(void *(*callback)(uint8_t,uiblock_t *));
void uiblock_destroy(uiblock_t *);

void uiblock_setparent(uiblock_t *,uiblock_t *);

void uiblock_lock(uiblock_t *);
void uiblock_unlock(uiblock_t *);
void uiblock_redraw(uiblock_t *);
void uiblock_resize(uiblock_t *);

extern int16_t screen_width,screen_height;

#endif

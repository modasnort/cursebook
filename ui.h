#ifndef UI_H
#define UI_H

#define UI_ANCHORS_NONE 0
#define UI_ANCHOR_LEFT 1
#define UI_ANCHOR_RIGHT 2
#define UI_ANCHOR_TOP 4
#define UI_ANCHOR_BOTTOM 8

#include <stdint.h>

typedef struct uiblock {

	uint8_t anchors;

	struct curseblock **children;
	uint16_t children_count;
	struct curseblock *parent;

} uiblock_t;

void ui_init();
void ui_shutdown();
void ui_warn(char *);

uiblock_t *uiblock_create();
void uiblock_destroy(uiblock_t *);

#endif

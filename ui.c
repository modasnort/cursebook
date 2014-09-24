#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "ui.h"
#include "console.h"
#include "thread.h"

thread_t *ui_thread;

mutex_t ui_mutex;
uiblock_t **uiblocks=NULL;
uint16_t uiblocks_count=0;

#define UI_FLAG_SHUTDOWN 1

uint8_t ui_flags=0;

int16_t screen_width;
int16_t screen_height;

void uiblock_setflag_children(uiblock_t *block,uint8_t flag) {
	for (uint16_t i=0;i<block->children_count;i++) {
		uiblock_t *child=block->children[i];
		if (child!=NULL) {
			uiblock_lock(child);
			setflag(child->flags,flag);
			uiblock_unlock(child);
		}
	}
}

void ui_lock() {
	mutex_lock(&ui_mutex);
}

void ui_unlock() {
	mutex_unlock(&ui_mutex);
}

void handle_winch(int sig)
{
	ui_lock();
	endwin();
	refresh();
	clear();
	refresh();

	getmaxyx(stdscr, screen_height, screen_width);

	for (int ii=0;ii<uiblocks_count;ii++) {
		uiblock_t *uiblock=uiblocks[ii];
		mutex_lock(&uiblock->mutex);
//		if (uiblock->parent==NULL) {
			setflag(uiblock->flags,UIBLOCK_FLAG_REALIGN);
			setflag(uiblock->flags,UIBLOCK_FLAG_REDRAW);
//		}
		mutex_unlock(&uiblock->mutex);
	}

	ui_unlock();
}

void uiblock_event(uint8_t event,uiblock_t *block) {
	block->callback(event,block);
}

/* WRONG!
void uiblock_event_recursive(uint8_t event,uiblock_t *block) {
	uiblock_event(event,block);
	for (uint16_t i=0;i<block->children_count;i++) {
		uiblock_t *child=block->children[i];
		if (child!=NULL) {
			uiblock_lock(child);
			uiblock_event_recursive(event,child);
			uiblock_unlock(child);
		}
	}
}
*/

void ui_thread_func(thread_t *thread)
{
	
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

	getmaxyx(stdscr, screen_height, screen_width);
	
	uint8_t shutdown=false;
	do {
		
		ui_lock();
		
		shutdown=hasflag(ui_flags,UI_FLAG_SHUTDOWN);
		
		for (int ii=0;ii<uiblocks_count;ii++) {
			uiblock_t *uiblock=uiblocks[ii];
			mutex_lock(&uiblock->mutex);
			
			if (shutdown||hasflag(uiblock->flags,UIBLOCK_FLAG_DESTROY)) {
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
				continue;
			}
			
			if (hasflag(uiblock->flags,UIBLOCK_FLAG_REALIGN)) {
				unsetflag(uiblock->flags,UIBLOCK_FLAG_REALIGN);
				
				box_t oldbox=uiblock->box;
				uiblock_event(UI_EVENT_REALIGN,uiblock);
				
				if (!box_equal(oldbox,uiblock->box)) {
					
					int16_t width=uiblock->box.bottomright.x-uiblock->box.topleft.x;
					int16_t height=uiblock->box.bottomright.y-uiblock->box.topleft.y;
					
					if (width<0)
						width=0;
					if (height<0)
						height=0;

					if (uiblock->window!=NULL) {
						//wborder(uiblock->window, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
						//wrefresh(uiblock->window);
						delwin(uiblock->window);
					}
					uiblock->window=newwin(height,width,uiblock->box.topleft.y,uiblock->box.topleft.x);

					scrollok(uiblock->window,TRUE);
					
					uiblock_redraw(uiblock);
				}

				uiblock_setflag_children(uiblock,UIBLOCK_FLAG_REALIGN);
			}
			
			if (hasflag(uiblock->flags,UIBLOCK_FLAG_REDRAW)) {
				unsetflag(uiblock->flags,UIBLOCK_FLAG_REDRAW);
				
				uiblock_event(UI_EVENT_REDRAW,uiblock);

				//wborder(uiblock->window, '|', '|', '-', '-', '+', '+', '+', '+');

				wrefresh(uiblock->window);

				uiblock_setflag_children(uiblock,UIBLOCK_FLAG_REDRAW);
			}
			
			mutex_unlock(&uiblock->mutex);
		}
		
		ui_unlock();
		
		
		pi_usleep(10000);
	} while (!shutdown);

	endwin();
	
	mutex_destroy(&ui_mutex);
}

void uiblock_lock(uiblock_t *block) {
	mutex_lock(&block->mutex);
}

void uiblock_unlock(uiblock_t *block) {
	mutex_unlock(&block->mutex);
}

void uiblock_redraw(uiblock_t *block) {
	setflag(block->flags,UIBLOCK_FLAG_REDRAW);
}

void ui_init() {
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = handle_winch;
	sigaction(SIGWINCH, &sa, NULL);

	ui_thread=thread_begin(ui_thread_func,NULL);
}

void ui_shutdown() {
	ui_lock();
	setflag(ui_flags,UI_FLAG_SHUTDOWN);
	ui_unlock();
}

void ui_warn(char *text) {
	printf("warning: %s\n",text);
}

void uiblock_resize(uiblock_t *uiblock) {
	mutex_lock(&uiblock->mutex);
	setflag(uiblock->flags,UIBLOCK_FLAG_REALIGN);
	mutex_unlock(&uiblock->mutex);
}

uiblock_t *uiblock_create(void *(*callback)(uint8_t,uiblock_t *)) {
	uiblock_t *uiblock=malloc(sizeof(uiblock_t));
	memset(uiblock,0,sizeof(uiblock_t));

	mutex_init(&uiblock->mutex);
	
	uiblock->callback=callback;
	
	uiblock_resize(uiblock);
	
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

void uiblock_setparent(uiblock_t *block,uiblock_t *parent) {
	if (block!=parent) {
		if (block->parent==NULL) {
			uint16_t childpos=0;
			if (parent->children==NULL)
				parent->children=malloc(sizeof(uiblock_t *));
			else {
				while ((childpos<parent->children_count)&&(parent->children[childpos]!=NULL))
					childpos++;
				if (childpos==parent->children_count) {
					parent->children_count++;
					parent->children=realloc(parent->children,sizeof(uiblock_t *)*parent->children_count);
				}
			}
			parent->children[childpos]=block;
			block->parent=parent;
		}
		else
			log_s("WARNING: parent already set\n");
	}
	else
		log_s("WARNING: attempt to self parent");
}

#include <malloc.h>
#include <string.h>

#include "console.h"

#include "ui.h"

uiblock_t *consoleframe=NULL;
uiblock_t *consoletext=NULL;

mutex_t logdata_mutex;
char *logdata=NULL;
int32_t logdata_length=0;

void *consoleframe_callback(uint8_t event,uiblock_t *block) {
	if (event==UI_EVENT_REALIGN) {
//		log_s_needlock("resize frame\n",0);
		block->box=t_box(screen_width*0.1,screen_height*0.5,screen_width*0.9,screen_height*0.9);
	}
	else if (event==UI_EVENT_REDRAW) {
		box(block->window,0,0);
	}
}

void *consoletext_callback(uint8_t event,uiblock_t *block) {
	if (event==UI_EVENT_REALIGN) {
//		log_s_needlock("resize text\n",0);
		block->box=t_box(consoleframe->box.topleft.x+1,consoleframe->box.topleft.y+1,consoleframe->box.bottomright.x-1,consoleframe->box.bottomright.y-1);
	}
	else if (event==UI_EVENT_REDRAW) {
		mutex_lock(&logdata_mutex);
		if (logdata!=NULL) {
			mvwprintw(block->window,0,0,"%s",logdata);
		}
		mutex_unlock(&logdata_mutex);
	}
}

void console_init() {
	if (consoleframe==NULL) {
		mutex_init(&logdata_mutex);
		consoleframe=uiblock_create(consoleframe_callback);
		consoletext=uiblock_create(consoletext_callback);
		uiblock_setparent(consoletext,consoleframe);
		log_s("console initialized\n");
	}
/*	log_eoln();
	log_s("dfvsdfsf sf sdf sdf ");
	log_s("vbb bjcv bcvbh cvjbh cvjh dfjg hdfjg dfghdfjg hdfjgh djfhgdjfhgdfjg dfg dfg f gdg 123456789");
	log_eoln();
	log_eoln();
	log_s("sdf sdf sadf asdf sadfsdf\n sdrfwer wer wer \nsfsdfsdfsdf \nbsdrfsd\nsdf sdf s fsdf\n");*/
}

void console_shutdown() {
	if (consoleframe!=NULL) {
		uiblock_destroy(consoleframe);
		consoleframe=NULL;
		uiblock_destroy(consoletext);
		consoletext=NULL;
		mutex_destroy(&logdata_mutex);
	}
}

void log_s_needlock(char *text,uint8_t needlock) {
	// TODO: overflow checking
	int32_t text_length=strlen(text);
	mutex_lock(&logdata_mutex);
	if (logdata==NULL)
		logdata=malloc(text_length+1);
	else
		logdata=realloc(logdata,logdata_length+text_length+1);
	memcpy(logdata+logdata_length,text,text_length);
	logdata_length+=text_length;
	logdata[logdata_length]='\0';
	mutex_unlock(&logdata_mutex);
	if (needlock)
		uiblock_lock(consoleframe);
	uiblock_redraw(consoleframe);
	if (needlock)
		uiblock_unlock(consoleframe);
}

void log_s(char *text) {
	log_s_needlock(text,1);
}

void log_eoln() {
	log_s("\n");
}

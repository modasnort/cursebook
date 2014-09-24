#ifndef CONSOLE_H
#define	CONSOLE_H

#include "config.h"

void console_init();
void console_shutdown();

void log_s_needlock(char *,uint8_t);

void log_s(char *);
void log_eoln();

#endif


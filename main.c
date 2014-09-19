#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "thread.h"
#include "console.h"

int main() {

	threads_init();
	
	ui_init();
	console_init();

	threads_maintain();

	console_shutdown();
	ui_shutdown();
	
	threads_shutdown();

	return EXIT_SUCCESS;
}

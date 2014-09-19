#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "thread.h"
#include "console.h"

int main() {

	ui_init();
	console_init();

	threads_maintain();

	console_shutdown();
	ui_shutdown();

	return EXIT_SUCCESS;
}

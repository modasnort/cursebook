#include <stdio.h>
#include <stdlib.h>

#include "ui.h"
#include "thread.h"

int main() {

	ui_init();


	threads_wait();


	ui_shutdown();

	return EXIT_SUCCESS;
}

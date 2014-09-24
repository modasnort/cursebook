#include "pi.h"

#include <unistd.h>

int pi_usleep(uint32_t usec) {
#ifdef _XOPEN_SOURCE
	return usleep(usec);
#else
#error "unable to find usable usleep substitute"
#endif
}


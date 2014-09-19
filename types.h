#ifndef TYPES_H
#define	TYPES_H

#include <stdint.h>

#include "config.h"

typedef int16_t coord_t;

typedef struct point {
	coord_t x;
	coord_t y;
} point_t;
point_t t_point(coord_t,coord_t);

typedef struct box {
	point_t topleft;
	point_t bottomright;
} box_t;
box_t t_box(coord_t,coord_t,coord_t,coord_t);

#define hasflag(_var,_flag) (_var|_flag==_var)
#define setflag(_var,_flag) (_var|=_flag)
#define unsetflag(_var,_flag) (_var&=!_flag)

#endif



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

#define box_equal(_box1,_box2) ((_box1.topleft.x==_box2.topleft.x)&&(_box1.topleft.y==_box2.topleft.y)&&(_box1.bottomright.x==_box2.bottomright.x)&&(_box1.bottomright.y==_box2.bottomright.y))

#define hasflag(_var,_flag) ((_var&_flag)==_flag)
#define setflag(_var,_flag) (_var|=_flag)
#define unsetflag(_var,_flag) (_var&=~_flag)

#endif



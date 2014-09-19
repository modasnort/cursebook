#include "types.h"

point_t t_point(coord_t x,coord_t y) {
	point_t ret;
	ret.x=x;
	ret.y=y;
	return ret;
}

box_t t_box(coord_t x1,coord_t y1,coord_t x2,coord_t y2) {
	box_t ret;
	ret.topleft=t_point(x1,y1);
	ret.bottomright=t_point(x2,y2);
	return ret;
}

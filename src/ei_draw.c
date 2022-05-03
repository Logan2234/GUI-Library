#include "ei_draw.h"

void ei_fill	(ei_surface_t surface,
                 const ei_color_t* color,
                 const ei_rect_t* clipper)
{
    ei_draw_polygon(surface, NULL, *color, clipper);
}
#include "ei_draw.h"
#include "hw_interface.h"

uint32_t ei_map_rgba (ei_surface_t surface, ei_color_t color)
{
    uint32_t couleur = 0;
    couleur = (couleur + (color.red-'0'));
    couleur = (couleur + (color.green-'0')*256);
    couleur = (couleur + (color.blue-'0')*256*256);
    couleur = (couleur + (color.alpha-'0')*256*256*256);
    return couleur;
}


void ei_fill	(ei_surface_t surface,
                 const ei_color_t* color,
                 const ei_rect_t* clipper)
{
    // uint32_t couleur = ei_map_rgba(surface, ei_default_background_color);
    uint32_t *pixel_ptr = (uint32_t *)hw_surface_get_buffer(surface);
    ei_size_t main_window_size = hw_surface_get_size(surface);
	for (uint32_t i = 0; i < main_window_size.width * main_window_size.height; i++)
    {
        // *pixel_ptr++ = couleur;
    }
}
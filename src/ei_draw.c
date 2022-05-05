#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_autre_draw.h"

uint32_t ei_map_rgba (ei_surface_t surface, ei_color_t color)
{
    uint32_t couleur = 0;
    
    uint32_t red = color.red - '\0';
    uint32_t blue = (color.blue - '\0');
    uint32_t green = (color.green - '\0');
    if (!hw_surface_has_alpha(surface)) 
    {
        couleur = (color.red - '\0') * 256 * 256 + (color.blue - '\0') + (color.green - '\0') * 256;
    }
    else
    {
        int *red_i;
        int *green_i;
        int *blue_i;
        int *alpha_i;
        hw_surface_get_channel_indices(surface, red_i, green_i, blue_i, alpha_i);
        uint32_t alpha = (color.alpha - '\0');
        couleur = 0; //TODO le fait de gérer la transparence si nécessaire 
    }
    return couleur;

}


void ei_fill	(ei_surface_t surface,
                 const ei_color_t* color,
                 const ei_rect_t* clipper)
{
    uint32_t couleur = ei_map_rgba(surface, *color);
    uint32_t *pixel_ptr = (uint32_t *)hw_surface_get_buffer(surface);
    ei_size_t main_window_size = hw_surface_get_size(surface);
	for (uint32_t i = 0; i < main_window_size.width * main_window_size.height; i++)
    {
        *pixel_ptr++ = couleur;
    }
}

void ei_draw_polygon (ei_surface_t	surface,
                      const ei_linked_point_t*	first_point,
                      ei_color_t			color,
                      const ei_rect_t*		clipper)
{
    // TODO
}
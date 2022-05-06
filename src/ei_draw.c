#include "ei_draw.h"
#include "hw_interface.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color)
{
    uint32_t couleur = 0;

    uint32_t red = color.red - '\0';
    uint32_t blue = (color.blue - '\0');
    uint32_t green = (color.green - '\0');
    int red_i, green_i, blue_i, alpha_i;
    hw_surface_get_channel_indices(surface, &red_i, &green_i, &blue_i, &alpha_i);
    if (!hw_surface_has_alpha(surface))
    {
        couleur += (red_i == 0) ? (color.red - '\0') : (red_i == 1) ? (color.red - '\0') * 256 : (color.red - '\0') * 256 * 256;  
        couleur += (blue_i == 0) ? (color.blue - '\0') : (blue_i == 1) ? (color.blue - '\0') * 256 : (color.blue - '\0') * 256 * 256;  
        couleur += (green_i == 0) ? (color.green - '\0') : (green_i == 1) ? (color.green - '\0') * 256 : (color.green - '\0') * 256 * 256;  
    }
    else
    {
        uint32_t alpha = (color.alpha - '\0');
        couleur = 0; // TODO le fait de gérer la transparence si nécessaire
    }
    return couleur;
}

void ei_fill(ei_surface_t surface,
             const ei_color_t *color,
             const ei_rect_t *clipper)
{
    uint32_t couleur = ei_map_rgba(surface, *color);
    ei_point_t depart = clipper->top_left;
    ei_size_t main_window_size = hw_surface_get_size(surface);
    uint32_t *pixel_ptr = (uint32_t *)(hw_surface_get_buffer(surface)) + (depart.x + depart.y * main_window_size.width);
    for (uint32_t i = 0; i < clipper->size.height; i++)
    {
        for (uint32_t j = 0; j < clipper->size.width; j++)
        {
            *pixel_ptr++ = couleur;
        }
        pixel_ptr += main_window_size.width - clipper->size.width;
    }
}

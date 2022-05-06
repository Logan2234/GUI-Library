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
        couleur += (red_i == 0) ? (color.red - '\0') : (red_i == 1) ? (color.red - '\0') * 256
                                                                    : (color.red - '\0') * 256 * 256;
        couleur += (blue_i == 0) ? (color.blue - '\0') : (blue_i == 1) ? (color.blue - '\0') * 256
                                                                       : (color.blue - '\0') * 256 * 256;
        couleur += (green_i == 0) ? (color.green - '\0') : (green_i == 1) ? (color.green - '\0') * 256
                                                                          : (color.green - '\0') * 256 * 256;
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
    ei_size_t main_window_size = hw_surface_get_size(surface);
    uint32_t *origine = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t couleur = ei_map_rgba(surface, *color);
    if (clipper != NULL){
        ei_point_t depart = clipper->top_left;
        uint32_t *pixel_ptr = origine + (depart.x + depart.y * main_window_size.width);
        uint32_t *last_pixel_of_current_line = origine + (depart.y + 1) * main_window_size.width;
        uint32_t z = 0;
        uint32_t a = 0;
        for (uint32_t i = 0; i < clipper->size.height; i++)
        {
            /* On dessine toutes la partie rectangulaire */
            for (uint32_t j = 0; j < clipper->size.width; j++)
            {
                /* On gère le cas où on dépasse la bordure basse de l'écran en arrêtant les deux boucles*/
                if (pixel_ptr > origine + main_window_size.width * main_window_size.height)
                {
                    break;
                }
                if (pixel_ptr > last_pixel_of_current_line)
                {
                    z = j;
                    printf("%d\n", z);
                    break;
                }
                *pixel_ptr++ = couleur;
            }
            if (pixel_ptr > last_pixel_of_current_line)
            {
                a++;
                pixel_ptr += main_window_size.width - z;
                couleur = 0;
            }
            /* ceci étant la deuxième boucle */
            if (pixel_ptr <= last_pixel_of_current_line)
            {
                last_pixel_of_current_line += main_window_size.width;
                pixel_ptr += main_window_size.width - clipper->size.width;
            }
            if (pixel_ptr > origine + main_window_size.width * main_window_size.height)
            {
                break;
            }

        }
        printf("%d\n",a);
    }
    else
    {
        for (uint32_t i = 0; i < main_window_size.width * main_window_size.height; i++)
        {
            *origine++ = couleur;
        }
    }
}

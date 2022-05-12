#include "ei_draw.h"

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
        uint32_t alpha = (color.alpha - '\0'); // TODO gérer la transparence
        couleur += (red_i == 0) ? (color.red - '\0') : (red_i == 1) ? (color.red - '\0') * 256
                                                                    : (color.red - '\0') * 256 * 256;
        couleur += (blue_i == 0) ? (color.blue - '\0') : (blue_i == 1) ? (color.blue - '\0') * 256
                                                                       : (color.blue - '\0') * 256 * 256;
        couleur += (green_i == 0) ? (color.green - '\0') : (green_i == 1) ? (color.green - '\0') * 256
                                                                          : (color.green - '\0') * 256 * 256;
    }
    return couleur;
}

void ei_fill(ei_surface_t surface, const ei_color_t *color, const ei_rect_t *clipper)
{
    ei_size_t main_window_size = hw_surface_get_size(surface);
    uint32_t *origine = (uint32_t *)hw_surface_get_buffer(surface);
    uint32_t couleur = ei_map_rgba(surface, *color);
    if (clipper != NULL)
    {
        ei_point_t depart = clipper->top_left;
        uint32_t *pixel_ptr = origine + (depart.x + depart.y * main_window_size.width);
        uint32_t *last_pixel_of_current_line = origine + (depart.y + 1) * main_window_size.width;
        uint32_t last_value_of_j = 0;
        for (uint32_t i = 0; i < clipper->size.height; i++)
        {
            /* On dessine toutes la partie rectangulaire */
            for (uint32_t j = 0; j < clipper->size.width; j++)
            {
                /* On gère le cas où on dépasse la bordure basse de l'écran en arrêtant les deux boucles*/
                if (pixel_ptr > origine + main_window_size.width * main_window_size.height)
                    break;

                if (pixel_ptr == last_pixel_of_current_line)
                {
                    last_value_of_j = j;
                    break;
                }
                *pixel_ptr++ = couleur;
            }
            if (pixel_ptr == last_pixel_of_current_line && last_value_of_j != 0)
                pixel_ptr += main_window_size.width - last_value_of_j;

            else
                pixel_ptr += main_window_size.width - clipper->size.width;

            last_pixel_of_current_line += main_window_size.width;

            if (pixel_ptr > origine + main_window_size.width * main_window_size.height)
                break;
        }
    }
    else
    {
        for (uint32_t i = 0; i < main_window_size.width * main_window_size.height; i++)
            *origine++ = couleur;
    }
}

void ei_draw_text(ei_surface_t surface, const ei_point_t *where,
                  const char *text, ei_font_t font,
                  ei_color_t color, const ei_rect_t *clipper)
{
    if (text != NULL)
    {
        ei_surface_t *surface_source = hw_text_create_surface(text, font, color);
        ei_rect_t source = hw_surface_get_rect(surface_source);
        ei_rect_t dest;
        dest.size = (clipper == NULL) ? (hw_surface_get_size(surface_source)) : clipper->size;

        dest.top_left.x = where->x;
        dest.top_left.y = where->y;
        
        ei_copy_surface(surface, &dest, surface_source, &source, EI_TRUE);
    }
}

int ei_copy_surface(ei_surface_t destination, const ei_rect_t *dst_rect,
                    ei_surface_t source, const ei_rect_t *src_rect, ei_bool_t alpha)
{
    ei_size_t main_window_size_dest = hw_surface_get_size(destination);
    ei_size_t main_window_size_src = hw_surface_get_size(source);

    uint32_t *origine_dest = (uint32_t *)hw_surface_get_buffer(destination);
    uint32_t *origine_src = (uint32_t *)hw_surface_get_buffer(source);

    origine_dest += dst_rect->top_left.x + dst_rect->top_left.y * main_window_size_dest.width;

    int ir;
    int ig;
    int ib;
    int ia;

    hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);

    if (dst_rect != NULL)
    {
        uint32_t *pixel_ptr_dest = origine_dest;
        uint32_t *pixel_ptr_src = origine_src;

        uint32_t *last_pixel_of_current_line_dest = origine_dest + (dst_rect->top_left.y + 1) * dst_rect->size.width;

        uint32_t last_value_of_j = 0;

        for (uint32_t i = 0; i < src_rect->size.height; i++)
        {
            /* On dessine toutes la partie rectangulaire */
            for (uint32_t j = 0; j < src_rect->size.width; j++)
            {
                /* On gère le cas où on dépasse la bordure basse de l'écran en arrêtant les deux boucles*/
                if (pixel_ptr_dest == last_pixel_of_current_line_dest)
                {
                    last_value_of_j = j;
                    break;
                }
                if (alpha == EI_TRUE){
                    uint8_t *dest = (uint8_t *)pixel_ptr_dest;
                    uint8_t *src = (uint8_t *)pixel_ptr_src;


                    *(dest + ig) = *(src + ia) * *(src + ig) + (255 - *(src + ia)) * *(dest + ig) / 255;
                    *(dest + ir) = *(src + ia) * *(src + ir) + (255 - *(src + ia)) * *(dest + ir) / 255;
                    *(dest + ib) = *(src + ia) * *(src + ib) + (255 - *(src + ia)) * *(dest + ib) / 255;

                    *pixel_ptr_dest++;
                    *pixel_ptr_src++;
                }
                else {
                    *pixel_ptr_dest++ = *pixel_ptr_src++;
                }
            }
            if (pixel_ptr_dest == last_pixel_of_current_line_dest && last_value_of_j != 0)
                pixel_ptr_dest += dst_rect->size.width - last_value_of_j;

            else
                pixel_ptr_dest += main_window_size_dest.width - dst_rect->size.width;

            last_pixel_of_current_line_dest += dst_rect->size.width;
        }
    }
    else
    {
        for (uint32_t i = 0; i < dst_rect->size.width * dst_rect->size.height; i++)
        {
            *origine_dest++ = *origine_src++;
        }
    }
    hw_surface_unlock(destination);
    ei_linked_rect_t *liste_rects = calloc(1, sizeof(ei_linked_rect_t));
    liste_rects->rect = *dst_rect;
    hw_surface_update_rects(destination, liste_rects);
}
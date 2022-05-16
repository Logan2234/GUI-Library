#include "ei_draw.h"
#include "ei_autre_fonctions.h"

uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color)
{
    uint32_t couleur = 0;
    uint32_t red = color.red - '\0';
    uint32_t blue = (color.blue - '\0');
    uint32_t green = (color.green - '\0');

    int red_i, green_i, blue_i, alpha_i;
    hw_surface_get_channel_indices(surface, &red_i, &green_i, &blue_i, &alpha_i);

    couleur += (red_i == 0) ? (color.red - '\0') : (red_i == 1) ? (color.red - '\0') * 256
                                                                : (color.red - '\0') * 256 * 256;
    couleur += (blue_i == 0) ? (color.blue - '\0') : (blue_i == 1) ? (color.blue - '\0') * 256
                                                                   : (color.blue - '\0') * 256 * 256;
    couleur += (green_i == 0) ? (color.green - '\0') : (green_i == 1) ? (color.green - '\0') * 256
                                                                      : (color.green - '\0') * 256 * 256;
    return couleur;
}

extern ei_surface_t pick_surface;
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
        for (int i = 0; i < clipper->size.height; i++)
        {
            /* On dessine toutes la partie rectangulaire */
            for (int j = 0; j < clipper->size.width; j++)
            {
                /* On gère le cas où on dépasse la bordure haute ou basse de l'écran en arrêtant les deux boucles */
                if (pixel_ptr > origine + main_window_size.width * main_window_size.height || pixel_ptr < origine)
                    break;
                
                if (pixel_ptr == last_pixel_of_current_line)
                {
                    last_value_of_j = j;
                    break;
                }

                /* On gère la transparence */
                if (!hw_surface_has_alpha(surface))
                {
                    uint32_t old_couleur_entier = *pixel_ptr;
                    ei_color_t old_couleur = int_to_color(old_couleur_entier);
                    ei_color_t new_couleur = {(color->red * color->alpha + (255 - color->alpha) * old_couleur.red) / 255,
                                              (color->green * color->alpha + (255 - color->alpha) * old_couleur.green) / 255,
                                              (color->blue * color->alpha + (255 - color->alpha) * old_couleur.blue) / 255, 0xdd};
                    couleur = ei_map_rgba(surface, new_couleur);
                }
                *pixel_ptr++ = couleur;
            }

            if (pixel_ptr == last_pixel_of_current_line && last_value_of_j != 0)
                pixel_ptr += main_window_size.width - last_value_of_j;

            else
                pixel_ptr += main_window_size.width - clipper->size.width;

            last_pixel_of_current_line += main_window_size.width;

            /* Cas où on dépasse la bordure haute ou basse */
            if (pixel_ptr > origine + main_window_size.width * main_window_size.height || pixel_ptr < origine)
                break;
        }
    }
    else
    {
        for (int i = 0; i < main_window_size.width * main_window_size.height; i++)
            *origine++ = couleur;
    }
}

void ei_draw_text(ei_surface_t surface, const ei_point_t *where, const char *text,
                  ei_font_t font, ei_color_t color, const ei_rect_t *clipper)
{
    if (text != NULL)
    {
        ei_surface_t *surface_source = hw_text_create_surface(text, font, color);
        ei_rect_t source = hw_surface_get_rect(surface_source);
        ei_rect_t dest;
        dest.top_left.x = (clipper == NULL) ? where->x: (clipper->top_left.x > source.top_left.x + where->x) ? clipper->top_left.x : where->x;
        dest.top_left.y = (clipper == NULL) ? where->y : (clipper->top_left.y > source.top_left.y + where->y) ? clipper->top_left.y : where->y;
        //dest.top_left.x -= (source.size.width - source.size.width)/2;
        //dest.top_left.y -= (source.size.height - source.size.height)/2;
        // if (clipper == NULL)
        // {
        //     if (clipper->top_left.x > dest.top_left.x)
        //     {
        //         dest.size.width = (hw_surface_get_size(surface_source).width - dest.top_left.x > clipper->size.width) ?  clipper->size.width : hw_surface_get_size(surface_source).width - dest.top_left.x;
        //     }
        //     else if (clipper->top_left.x < dest.top_left.x)
        //     {
        //         dest.size.width = (clipper->size.width - source.top_left.x > 0) ? clipper->size.width - source.top_left.x : 0;
        //     }
        //     else
        //     {
        //         dest.size.width = (source.size.width > clipper->size.width) ? clipper->size.width : source.size.width;
        //     }
        //     if (clipper->top_left.y > dest.top_left.y)
        //     {
        //         dest.size.height = (source.size.height - dest.top_left.y > clipper->size.height) ?  clipper->size.height : source.size.height - dest.top_left.y;
        //     }
        //     else if (clipper->top_left.y < dest.top_left.y)
        //     {
        //         dest.size.height = (clipper->size.height - source.top_left.y > 0) ? clipper->size.height - source.top_left.y : 0;
        //     }
        //     else
        //     {
        //         dest.size.height = (source.size.height > clipper->size.height) ? clipper->size.height : source.size.height;
        //         dest.size.height = (source.size.height - dest.top_left.y > clipper->size.height) ?  clipper->size.height : source.size.height - dest.top_left.y;
        //     }
        // }
        // else
        // {
        //     dest.size.width = source.size.width;
        //     dest.size.height = source.size.height;
        // }
        dest.size.width = (clipper==NULL) ? hw_surface_get_size(surface_source).width : (hw_surface_get_size(surface_source).width > clipper->size.width) ?  clipper->size.width : hw_surface_get_size(surface_source).width;
        dest.size.height = (clipper==NULL) ? hw_surface_get_size(surface_source).height : (hw_surface_get_size(surface_source).height > clipper->size.height) ?  clipper->size.height : hw_surface_get_size(surface_source).height;
        //dest.size.width -= where->x;
        //dest.size.height -= where->y;
        ei_copy_surface(surface, &dest, surface_source, &source, EI_TRUE);
        hw_surface_free(surface_source);
    }
}

int ei_copy_surface(ei_surface_t destination, const ei_rect_t *dst_rect,
                    ei_surface_t source, const ei_rect_t *src_rect, ei_bool_t alpha)
{
    ei_size_t main_window_size_dest = hw_surface_get_size(destination);
    ei_size_t main_window_size_src = hw_surface_get_size(source);

    int ir;
    int ig;
    int ib;
    int ia;
    hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);

    int ir_;
    int ig_;
    int ib_;
    int ia_;
    hw_surface_get_channel_indices(destination, &ir_, &ig_, &ib_, &ia_);

    ei_rect_t *src_rect2 = calloc(1, sizeof(ei_rect_t));
    src_rect2->size = (src_rect == NULL) ? hw_surface_get_rect(source).size: src_rect->size;
    src_rect2->top_left = (src_rect == NULL) ? hw_surface_get_rect(source).top_left : src_rect->top_left;

    ei_rect_t *dst_rect2 = calloc(1, sizeof(ei_rect_t));
    dst_rect2->size = (dst_rect == NULL) ? hw_surface_get_rect(destination).size : dst_rect->size;
    dst_rect2->top_left = (dst_rect == NULL) ? hw_surface_get_rect(destination).top_left : dst_rect->top_left;

    uint32_t *origine_dest = (uint32_t *)hw_surface_get_buffer(destination);
    uint32_t *origine_src = (uint32_t *)hw_surface_get_buffer(source);
    origine_dest += dst_rect2->top_left.x + dst_rect2->top_left.y * main_window_size_dest.width;
    origine_src += src_rect2->top_left.x + src_rect2->top_left.y * main_window_size_src.width;

    if (dst_rect != NULL)
    {
        uint32_t *pixel_ptr_dest = origine_dest;
        uint32_t *pixel_ptr_src = origine_src;

        //uint32_t *last_pixel_of_current_line_dest = origine_dest + (dst_rect->top_left.y + 1) * dst_rect2->size.width;

        //uint32_t ecart_heigth = src_rect2->size.height - dst_rect2->size.height;
        //uint32_t ecart_width = src_rect2->size.width - dst_rect2->size.width;

        //uint32_t last_value_of_j = 0;
        for (uint32_t i = 0; i < (uint32_t)src_rect2->size.height; i++)
        {
            /* On dessine toutes la partie rectangulaire */
            if (i >= dst_rect2->size.height){
                pixel_ptr_src += main_window_size_src.width;
                continue;
            }
            for (uint32_t j = 0; j < (uint32_t)src_rect2->size.width; j++)
            {
                if (j>=dst_rect2->size.width){
                    *pixel_ptr_src++;
                    continue;
                }

                /* On gère le cas où on dépasse la bordure basse de l'écran en arrêtant les deux boucles*/
                // if (pixel_ptr_dest == last_pixel_of_current_line_dest)
                // {
                //     last_value_of_j = j;
                //     break;
                // }
                if (alpha == EI_TRUE)
                {
                    uint8_t *dest = (uint8_t *)pixel_ptr_dest;
                    uint8_t *src = (uint8_t *)pixel_ptr_src;

                    *(dest + ig_) = (*(src + ia) * *(src + ig) + (255 - *(src + ia)) * *(dest + ig_)) / 255;
                    *(dest + ir_) = (*(src + ia) * *(src + ir) + (255 - *(src + ia)) * *(dest + ir_)) / 255;
                    *(dest + ib_) = (*(src + ia) * *(src + ib) + (255 - *(src + ia)) * *(dest + ib_)) / 255;

                    *pixel_ptr_dest++;
                    *pixel_ptr_src++;
                }
                else
                    *pixel_ptr_dest++ = *pixel_ptr_src++;
            }
            //if (pixel_ptr_dest == last_pixel_of_current_line_dest)
              //  pixel_ptr_dest += dst_rect2->size.width - last_value_of_j;

            //else
            pixel_ptr_dest += (dst_rect2->size.width < src_rect2->size.width) ? main_window_size_dest.width - dst_rect2->size.width : main_window_size_dest.width - src_rect2->size.width;
            //pixel_ptr_dest += main_window_size_dest.width - src_rect2->size.width;
            //last_pixel_of_current_line_dest += dst_rect2->size.width;
        }
    }
    else
    {
        for (int i = 0; i < dst_rect2->size.width * dst_rect2->size.height; i++)
            *origine_dest++ = *origine_src++;
    }
    hw_surface_unlock(destination);
    ei_linked_rect_t *liste_rects = calloc(1, sizeof(ei_linked_rect_t));
    liste_rects->rect = *dst_rect2;
    hw_surface_update_rects(destination, liste_rects);
    free(liste_rects);
    free(src_rect2);
    free(dst_rect2);
 
}
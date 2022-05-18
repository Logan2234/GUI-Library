#include "ei_draw.h"
#include "ei_autre_fonctions.h"

/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
 * @return 			The 32 bit integer corresponding to the color. The alpha component
 *				of the color is ignored in the case of surfaces that don't have an
 *				alpha channel.
 */
uint32_t ei_map_rgba(ei_surface_t surface, ei_color_t color)
{
    // On travaille sur 32 bits, ainsi on ajoute le rouge, bleu, vert selon la position qu'ont les indices dans la surface.
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

extern int compte;


/**
 * \brief	Fills the surface with the specified color.
 *
 * @param	surface		The surface to be filled. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	color		The color used to fill the surface. If NULL, it means that the
 *				caller want it painted black (opaque).
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
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

/**
 * \brief	Draws text by calling \ref hw_text_create_surface.
 *
 * @param	surface 	Where to draw the text. The surface must be *locked* by
 *				\ref hw_surface_lock.
 * @param	where		Coordinates, in the surface, where to anchor the top-left corner of
 *				the rendered text.
 * @param	text		The string of the text. Can't be NULL.
 * @param	font		The font used to render the text. If NULL, the \ref ei_default_font
 *				is used.
 * @param	color		The text color. Can't be NULL. The alpha parameter is not used.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void ei_draw_text(ei_surface_t surface, const ei_point_t *where, const char *text,
                  ei_font_t font, ei_color_t color, const ei_rect_t *clipper)
{
    // On n'écrit le texte que s'il y en a.
    if (text != NULL)
    {
        ei_surface_t *surface_source = hw_text_create_surface(text, font, color);
        ei_rect_t source = hw_surface_get_rect(surface_source);

        // On calcule la surface destinataire avec le clipper. La destination est donc l'intersection entre la surface donnée et le clipper.
        ei_rect_t dest;
        dest.top_left.x = (clipper == NULL) ? where->x: (clipper->top_left.x > source.top_left.x + where->x) ? clipper->top_left.x : where->x;
        dest.top_left.y = (clipper == NULL) ? where->y : (clipper->top_left.y > source.top_left.y + where->y) ? clipper->top_left.y : where->y;
        dest.size.width = (clipper == NULL) ? hw_surface_get_size(surface_source).width : (hw_surface_get_size(surface_source).width > clipper->top_left.x + clipper->size.width - where->x) ?  (clipper->top_left.x + clipper->size.width - where->x < 0) ? 0 : clipper->top_left.x + clipper->size.width - where->x : hw_surface_get_size(surface_source).width;
        dest.size.height = (clipper == NULL) ? hw_surface_get_size(surface_source).height : (hw_surface_get_size(surface_source).height > clipper->top_left.y + clipper->size.height - where->y) ?  (clipper->top_left.y + clipper->size.height - where->y < 0) ? 0 : clipper->top_left.y + clipper->size.height - where->y : hw_surface_get_size(surface_source).height;

        ei_copy_surface(surface, &dest, surface_source, &source, EI_TRUE);
        hw_surface_free(surface_source);
    }
}

/**
 * \brief	Copies pixels from a source surface to a destination surface.
 *		The source and destination areas of the copy (either the entire surfaces, or
 *		subparts) must have the same size before considering clipping.
 *		Both surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param	destination	The surface on which to copy pixels.
 * @param	dst_rect	If NULL, the entire destination surface is used. If not NULL,
 *				defines the rectangle on the destination surface where to copy
 *				the pixels.
 * @param	source		The surface from which to copy pixels.
 * @param	src_rect	If NULL, the entire source surface is used. If not NULL, defines the
 *				rectangle on the source surface from which to copy the pixels.
 * @param	alpha		If true, the final pixels are a combination of source and
 *				destination pixels weighted by the source alpha channel and
 *				the transparency of the final pixels is set to opaque.
 *				If false, the final pixels are an exact copy of the source pixels,
 				including the alpha channel.
 *
 * @return			Returns 0 on success, 1 on failure (different sizes between source and destination).
 */
int ei_copy_surface(ei_surface_t destination, const ei_rect_t *dst_rect,
                    ei_surface_t source, const ei_rect_t *src_rect, ei_bool_t alpha)
{
    /* Tout d'abord, on ne copie l'image que si la taille de la surface et la desrination sont les memes ou qu'on ait des instructions sur où appliquer la fonction */
    ei_size_t size_src = hw_surface_get_size(source);
    ei_size_t size_dst = hw_surface_get_size(destination);
    if (src_rect == NULL && dst_rect == NULL && ((size_src.width != size_dst.width) || (size_src.height != size_dst.height)))
        return EXIT_FAILURE;
    
    else
    {
        ei_size_t main_window_size_dest = hw_surface_get_size(destination);
        ei_size_t main_window_size_src = hw_surface_get_size(source);

        ei_rect_t main_window_rect_dest = hw_surface_get_rect(destination);
        ei_rect_t main_window_rect_src = hw_surface_get_rect(source);

        // On récupère les indices des RGB qui peuvent être différente entre la source et la destination
        int ir, ig, ib, ia;
        int ir_, ig_, ib_, ia_;
        if (alpha == EI_TRUE) 
        {
            hw_surface_get_channel_indices(source, &ir, &ig, &ib, &ia);
            hw_surface_get_channel_indices(destination, &ir_, &ig_, &ib_, &ia_);
        }

        /*
        On gère le cas où dst_rect ou src_rect est nul, on créé deux nouveaux rectangle qu'on initialise soit avec src_rect et dest_rect si non nul,
        soit avec les rectangles associés à la surface source et la surface destination.
        */
        ei_rect_t src_rect2 = (src_rect == NULL) ? main_window_rect_src : *src_rect;

        ei_rect_t dst_rect2 = (dst_rect == NULL) ? main_window_rect_dest : *dst_rect;
        
        uint32_t *origine_dest = (uint32_t *)hw_surface_get_buffer(destination);
        uint32_t *origine_src = (uint32_t *)hw_surface_get_buffer(source);
        
        // On prend le premier point de remplissage sur la surface de destination en allant sur le pixel qui correspond au where.
        origine_dest += dst_rect2.top_left.x + dst_rect2.top_left.y * main_window_size_dest.width;
        origine_src += src_rect2.top_left.x + src_rect2.top_left.y * main_window_size_src.width;
        
        for (uint32_t i = 0; i < (uint32_t)src_rect2.size.height; i++)
        {
            // Si la source est plus grande que la destination, ou coupe ce qui dépasse.
            if (i >= dst_rect2.size.height)
            {
                origine_src += main_window_size_src.width;
                continue;
            }
            for (uint32_t j = 0; j < (uint32_t)src_rect2.size.width; j++)
            {
                // Pareil ici
                if (j >= dst_rect2.size.width)
                {
                    origine_src++;
                    continue;
                }

                // Soit on écrit un texte, et on applique la formule pour la transparence.
                if (alpha == EI_TRUE)
                {
                    uint8_t *dest = (uint8_t *)origine_dest;
                    uint8_t *src = (uint8_t *)origine_src;
                    *(dest + ig_) = (*(src + ia) * *(src + ig) + (255 - *(src + ia)) * *(dest + ig_)) / 255;
                    *(dest + ir_) = (*(src + ia) * *(src + ir) + (255 - *(src + ia)) * *(dest + ir_)) / 255;
                    *(dest + ib_) = (*(src + ia) * *(src + ib) + (255 - *(src + ia)) * *(dest + ib_)) / 255;
                    origine_dest++;
                    origine_src++;
                }
                // Soit on se contente de juste recopier les pixels (le cas pour une image).
                else
                    *origine_dest++ = *origine_src++;
            }
            // On retourne à la ligne pour faire la ligne d'après.
            if (dst_rect2.size.width < src_rect2.size.width)
                origine_dest += main_window_size_dest.width - dst_rect2.size.width; 
            else
            {
                origine_dest += main_window_size_dest.width - src_rect2.size.width;
                origine_src += main_window_size_src.width - src_rect2.size.width; 
            }
        }
        hw_surface_unlock(destination);
        ei_linked_rect_t *liste_rects = calloc(1, sizeof(ei_linked_rect_t));
        liste_rects->rect = dst_rect2;
        hw_surface_update_rects(destination, liste_rects);
        free(liste_rects);
    }
}
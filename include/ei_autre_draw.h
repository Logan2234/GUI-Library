
/* Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis.
 * Entrée : rectangle ei_rect_t, rayon et position. 0 si c'est tout le rectangle, 1 la partie haute, 2 partie base
 * Sortie : liste chainée */
ei_linked_point_t *ei_rounded_frame(ei_rect_t *rectangle, uint32_t rayon, uint8_t orientation);

void ei_draw_img(ei_surface_t surface, const ei_point_t *where, const char **filename, const ei_rect_t *clipper);

/* 
 * Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin
 * Sortie : liste chainée de points formant un arc 
 */
ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin);

/* Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis.
 * Entrée : rectangle ei_rect_t, rayon et position. 0 si c'est tout le rectangle, 1 la partie haute, 2 partie base
 * Sortie : liste chainée */
ei_linked_point_t *ei_rounded_frame(ei_rect_t *rectangle, uint32_t rayon, uint8_t orientation);


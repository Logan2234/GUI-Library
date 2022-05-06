
#ifndef PROJET_C_EI_AUTRE_DRAW_H
#define PROJET_C_EI_AUTRE_DRAW_H

#endif // PROJET_C_EI_AUTRE_DRAW_H

#include "ei_autre_struct.h"

/* Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin
 *
 * Return : liste chainée de points formant un arc */
ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin);

/* Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis.
 * Entrée : rectangle ei_rect_t, rayon et position. 0 si c'est tout le rectangle, 1 la partie haute, 2 partie base
 * Sortie : liste chainée */
ei_linked_point_t* ei_rounded_frame (ei_rect_t* rectangle, uint32_t rayon, uint8_t orientation);

/* Fonction pour dessiner un button
 * Entrée : celles de ei_draw_polygon mais avec un rectangle et un rayon à la place du firstpoint
 * Sortie : rien. Le button est dessiné */
void ei_draw_button	(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper);

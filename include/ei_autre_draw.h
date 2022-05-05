
#ifndef PROJET_C_EI_AUTRE_DRAW_H
#define PROJET_C_EI_AUTRE_DRAW_H

#endif // PROJET_C_EI_AUTRE_DRAW_H

#include "ei_widgetclass.h"
#include "ei_widget.h"

/* Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin
 *
 * Return : liste chainée de points formant un arc */
ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin);

/* Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis.
 * Entrée : rectangle ei_rect_t, rayon et position. 0 si c'est tout le rectangle, 1 la partie haute, 2 partie base
 * Sortie : liste chainée */
ei_linked_point_t* ei_rounded_frame (ei_rect_t* rectangle, uint32_t rayon, uint32_t orientation);
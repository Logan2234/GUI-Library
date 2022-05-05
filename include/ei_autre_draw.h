
#ifndef PROJET_C_EI_AUTRE_DRAW_H
#define PROJET_C_EI_AUTRE_DRAW_H

#endif //PROJET_C_EI_AUTRE_DRAW_H

#include "ei_widgetclass.h"
#include "ei_widget.h"

/* Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin
 *
 * Return : liste chainée de points formant un arc */
ei_linked_point_t* ei_draw_arc (ei_point_t center,
                                int rayon,
                                float angle_debut,
                                float angle_fin);
/**
 * @file ei_autre_draw.h
 * @brief Permet de créer une liste chainée pour pouvoir dessiner les bouttons
 *
 *
 */

#include "ei_types.h"

/**
 * @brief Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin.
 *
 * @return Sortie : liste chainée de points formant un arc
 *
 */
ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin);

/**
 * @brief Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis. Elle appelle ei_draw_arc.
 *
 * @param rectangle Le rectangle qu'il faut dessiner.
 *
 * @param rayon Le rayon de l'arrondi au niveau des angles.
 *
 * @param orientation Entier qui vaut 0 s'il faut dessiner tout le rectangle, 1 si c'est uniquement la partie haute et 2 si c'est unqiuement la partie base
 *
 * @return La liste chainée des points constituant le contour.
 *
 */
ei_linked_point_t *ei_rounded_frame(ei_rect_t *rectangle, uint32_t rayon, uint8_t orientation);

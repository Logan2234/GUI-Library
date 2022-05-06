#include <math.h>
#include <stdlib.h>

#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_autre_draw.h"

ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin)
{
    ei_linked_point_t *liste_point;
    ei_linked_point_t *suivant;
    float longueur_arc = abs((float)rayon * (angle_fin - angle_debut));
    float pas = (angle_fin - angle_debut) / longueur_arc;
    liste_point->point.x = (int)rayon * cos(angle_debut);
    liste_point->point.y = (int)rayon * sin(angle_debut);
    suivant = liste_point;
    for (uint32_t i = 1; i <= longueur_arc; i++)
    {
        int x = (int)rayon * cos(angle_debut + i * pas);
        int y = (int)rayon * sin(angle_debut + i * pas);
        while (suivant->next != NULL)
        {
            suivant = suivant->next;
        }
        if (suivant->next->point.x != x || suivant->next->point.y != y)
        {
            ei_linked_point_t *nouveau;
            nouveau->point.x = x;
            nouveau->point.y = y;
            nouveau->next = NULL;
            suivant->next = nouveau;
        }
    }
    return liste_point;
}

ei_linked_point_t* ei_rounded_frame (ei_rect_t* rectangle,
                                    uint32_t rayon,
                                    uint32_t orientation)
{
    if (orientation == 0) {     /* Cas où on dessine toute la forme */

        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, 3.1415, 1.57075);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, 1.57075, 0);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, 0, -1.57075);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        suivant = suivant->next;
        nouveau = ei_draw_arc(premier_point, rayon, -1.57075, -3.1415);
        suivant->next = nouveau;
        return liste_point;
    }
    if (orientation == 1) {     /* Cas où on dessine que la partie haute */
        int h = rectangle->size.height / 2;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, 3.1415, 1.57075);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, 1.57075, 0.7854);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        nouveau->point.x = suivant->point.x - h + rayon * (1 - cos(0.7854));
        nouveau->point.y = suivant->point.y + h - rayon * (1 - sin(0.7854));
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;
        nouveau->point.x = suivant->point.x - rectangle->size.width + 2 * h;
        nouveau->point.y = suivant->point.y;
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;
        premier_point.x = suivant->point.x - h + rayon;
        premier_point.y = suivant->point.y + h - rayon;
        nouveau = ei_draw_arc(premier_point, rayon, -2.3562, -3.1415);
        suivant->next = nouveau;
        return liste_point;
    }
    if (orientation == 2) {     /* Cas où on dessine que la partie basse */
        int h = rectangle->size.height / 2;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, 0.7854, 0);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, 0, -1.57075);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        suivant = suivant->next;
        nouveau = ei_draw_arc(premier_point, rayon, -1.57075, -2.3562);
        suivant->next = nouveau;
        nouveau->point.x = suivant->point.x + h - rayon * (1 - cos(0.7854));
        nouveau->point.y = suivant->point.y - h + rayon * (1 - sin(0.7854));
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;
        nouveau->point.x = suivant->point.x + rectangle->size.width - 2 * h;
        nouveau->point.y = suivant->point.y;
        nouveau->next = NULL;
        suivant->next = nouveau;
        return liste_point;
    }
    return NULL;
}

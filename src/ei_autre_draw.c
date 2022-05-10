#include <math.h>
#include <stdlib.h>

#include "ei_draw.h"
#include "ei_application.h"
#include "ei_autre_draw.h"
#include "ei_autre_struct.h"

/* Fonction qui crée un arc à partir d'un centre, un rayon et des angles de début et fin
 *
 * Return : liste chainée de points formant un arc */
ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin)
{
    ei_linked_point_t *liste_point = malloc(sizeof(ei_linked_point_t));
    int longueur_arc = abs((int)((float)rayon * (angle_fin - angle_debut)));
    float pas = (angle_fin - angle_debut) / (float)longueur_arc;
    liste_point->point.x = center.x + (int)((float)rayon * cosf(angle_debut));
    liste_point->point.y = center.y - (int)((float)rayon * sinf(angle_debut));
    liste_point->next = NULL;
    ei_linked_point_t *suivant = liste_point;

    for (uint32_t i = 1; i <= longueur_arc; i++)
    {

        int x = center.x + (int)((float)rayon * cosf(angle_debut + (float)i * pas));
        int y = center.y - (int)((float)rayon * sinf(angle_debut + (float)i * pas));
        while (suivant->next != NULL)
        {
            suivant = suivant->next;
        }
        if (suivant->point.x != x || suivant->point.y != y)
        {
            ei_linked_point_t *nouveau = malloc(sizeof(ei_linked_point_t));
            nouveau->point.x = x;
            nouveau->point.y = y;
            nouveau->next = NULL;
            suivant->next = nouveau;
        }
    }
    return liste_point;
}

/* Renvoie liste chainée de points représentant la forme d'un rectangle aux angles arrondis.
 * Entrée : rectangle ei_rect_t, rayon et position. 0 si c'est tout le rectangle, 1 la partie haute, 2 partie base
 * Sortie : liste chainée */
ei_linked_point_t *ei_rounded_frame(ei_rect_t *rectangle, uint32_t rayon, uint8_t orientation)
{
    if (orientation == 0) /* Cas où on dessine toute la forme */
    {
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int)rayon;
        premier_point.y = rectangle->top_left.y + (int)rayon;
        ei_linked_point_t *liste_point = ei_draw_arc(premier_point, rayon, (float)3.1415, (float)1.57075);
        ei_linked_point_t *suivant = liste_point;
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int)rayon;
        premier_point.y = rectangle->top_left.y + (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)1.57075, (float)0);
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int)rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)0, (float)-1.57075);
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + (int)rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)-1.57075, (float)-3.1415);
        return liste_point;
    }
    if (orientation == 1) /* Cas où on dessine que la partie haute */
    {
        int h = rectangle->size.height / 2;
        ei_linked_point_t *liste_point;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int)rayon;
        premier_point.y = rectangle->top_left.y + (int)rayon;
        liste_point = ei_draw_arc(premier_point, rayon, (float)3.1415, (float)1.57075);
        ei_linked_point_t *suivant = liste_point;
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int)rayon;
        premier_point.y = rectangle->top_left.y + (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)1.57075, (float)0.7854);
        while (suivant->next != NULL)
            suivant = suivant->next;

        ei_linked_point_t *nouveau = malloc(sizeof(ei_linked_point_t));
        nouveau->point.x = suivant->point.x - h + (int)((float)rayon * (1. - (float)cos(0.7854)));
        nouveau->point.y = suivant->point.y + h - (int)((float)rayon * (1. - (float)sin(0.7854)));
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;

        ei_linked_point_t *nouveau2 = malloc(sizeof(ei_linked_point_t));
        nouveau2->point.x = suivant->point.x - rectangle->size.width + 2 * h;
        nouveau2->point.y = suivant->point.y + rectangle->size.height - 2 * h;
        suivant->next = nouveau2;
        suivant = suivant->next;

        premier_point.x = suivant->point.x - h + (int)rayon;
        premier_point.y = suivant->point.y + h - (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)-2.3562, (float)-3.1415);
        ;
        return liste_point;
    }
    if (orientation == 2) /* Cas où on dessine que la partie basse */
    {
        int h = rectangle->size.height / 2;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int)rayon;
        premier_point.y = rectangle->top_left.y + (int)rayon;
        ei_linked_point_t *liste_point = ei_draw_arc(premier_point, rayon, (float)0.7854, (float)0);
        ei_linked_point_t *suivant = liste_point;
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int)rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)0, (float)-1.57075);
        while (suivant->next != NULL)
            suivant = suivant->next;

        premier_point.x = rectangle->top_left.x + (int)rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int)rayon;
        suivant->next = ei_draw_arc(premier_point, rayon, (float)-1.57075, (float)-2.3562);
        while (suivant->next != NULL)
            suivant = suivant->next;

        ei_linked_point_t *nouveau = malloc(sizeof(ei_linked_point_t));
        nouveau->point.x = suivant->point.x + h - (int)((float)rayon * (1. - (float)cos(0.7854)));
        nouveau->point.y = suivant->point.y - h + (int)((float)rayon * (1. - (float)sin(0.7854)));
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;

        ei_linked_point_t *nouveau2 = malloc(sizeof(ei_linked_point_t));
        nouveau2->point.x = suivant->point.x + rectangle->size.width - 2 * h;
        nouveau2->point.y = suivant->point.y;
        nouveau2->next = NULL;
        suivant->next = nouveau2;
        return liste_point;
    }
    return NULL;
}
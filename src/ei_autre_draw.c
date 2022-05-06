#include <math.h>
#include <stdlib.h>

#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_autre_draw.h"

ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin)
{
    ei_linked_point_t *liste_point;
    ei_linked_point_t *suivant;
    int longueur_arc = abs((int)((float)rayon * (angle_fin - angle_debut)));
    float pas = (angle_fin - angle_debut) / (float) longueur_arc;
    liste_point->point.x = center.x + (int)((float) rayon * cosf(angle_debut));
    liste_point->point.y = center.y + (int)((float) rayon * sinf(angle_debut));
    suivant = liste_point;
    for (uint32_t i = 1; i <= longueur_arc; i++)
    {
        int x = center.x + (int)((float) rayon * cosf(angle_debut + (float) i * pas));
        int y = center.y + (int)((float) rayon * sinf(angle_debut + (float) i * pas));
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
                                    uint8_t orientation)
{
    if (orientation == 0) {     /* Cas où on dessine toute la forme */

        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, (float) 3.1415, (float) 1.57075);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, (float) 1.57075, (float) 0);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, (float) 0, (float) -1.57075);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        suivant = suivant->next;
        nouveau = ei_draw_arc(premier_point, rayon, (float) -1.57075, (float) -3.1415);
        suivant->next = nouveau;
        return liste_point;
    }
    if (orientation == 1) {     /* Cas où on dessine que la partie haute */
        int h = rectangle->size.height / 2;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, (float) 3.1415, (float) 1.57075);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, (float) 1.57075, (float) 0.7854);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        nouveau->point.x = suivant->point.x - h + (int) ((float)rayon * (1. - (float) cos(0.7854)));
        nouveau->point.y = suivant->point.y + h - (int) ((float)rayon * (1. - (float) sin(0.7854)));
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;
        nouveau->point.x = suivant->point.x - rectangle->size.width + 2 * h;
        nouveau->point.y = suivant->point.y;
        nouveau->next = NULL;
        suivant->next = nouveau;
        suivant = suivant->next;
        premier_point.x = suivant->point.x - h + (int) rayon;
        premier_point.y = suivant->point.y + h - (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, (float) -2.3562, (float) -3.1415);
        suivant->next = nouveau;
        return liste_point;
    }
    if (orientation == 2) {     /* Cas où on dessine que la partie basse */
        int h = rectangle->size.height / 2;
        ei_point_t premier_point;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + (int) rayon;
        ei_linked_point_t *liste_point;
        liste_point = ei_draw_arc(premier_point, rayon, (float) 0.7854, (float) 0);
        ei_linked_point_t *suivant;
        suivant = liste_point;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        ei_linked_point_t *nouveau;
        premier_point.x = rectangle->top_left.x + rectangle->size.width - (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        nouveau = ei_draw_arc(premier_point, rayon, (float) 0, (float) -1.57075);
        suivant->next = nouveau;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        premier_point.x = rectangle->top_left.x + (int) rayon;
        premier_point.y = rectangle->top_left.y + rectangle->size.height - (int) rayon;
        suivant = suivant->next;
        nouveau = ei_draw_arc(premier_point, rayon, (float) -1.57075, (float) -2.3562);
        suivant->next = nouveau;
        nouveau->point.x = suivant->point.x + h - (int)((float)rayon * (1. - (float) cos(0.7854)));
        nouveau->point.y = suivant->point.y - h + (int)((float)rayon * (1. - (float) sin(0.7854)));
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

/* Pour l'instant, le bouton n'est que relevé */
void ei_draw_button	(ei_surface_t surface, ei_button_t* bouton, int* x, int* y, const ei_rect_t* clipper)
{
    ei_rect_t *rectangle;
    rectangle->top_left.x = *x;
    rectangle->top_left.y = *y;
    rectangle->size = *(bouton->requested_size);
    ei_linked_point_t *partie_haute = ei_rounded_frame(rectangle, *(bouton->corner_radius), 1);
    ei_linked_point_t *partie_basse = ei_rounded_frame(rectangle, *(bouton->corner_radius), 2);
    rectangle->top_left.x = *x + ((int) ((float)1 / 3) * *(bouton->corner_radius));
    rectangle->top_left.y = *y + ((int) ((float)1 / 3) * *(bouton->corner_radius));
    rectangle->size.width -= 2 * ((int) ((float)1 / 3) * *(bouton->corner_radius));
    rectangle->size.height -= 2 * ((int) ((float)1 / 3) * *(bouton->corner_radius));
    ei_linked_point_t *partie_milieu = ei_rounded_frame(rectangle, (int) ((float)2 / 3) * *(bouton->corner_radius), 1);


    ei_color_t color;
    color.alpha = "0xFF";
    color.blue = "0x64";
    color.red = "0x64";
    color.green = "0x64";
    ei_draw_polygon(surface, partie_haute, color, clipper);

    color.alpha = "0xFF";
    color.blue = "0xB4";
    color.red = "0xB4";
    color.green = "0xB4";
    ei_draw_polygon(surface, partie_basse, color, clipper);

    color.alpha = "0xFF";
    color.blue = "0x8B";
    color.red = "0x8B";
    color.green = "0x8B";
    ei_draw_polygon(surface, partie_milieu, color, clipper);

}

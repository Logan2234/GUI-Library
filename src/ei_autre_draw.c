#include <math.h>
#include <stdlib.h>

#include "ei_draw.h"
#include "ei_application.h"
#include "ei_autre_draw.h"

void print_liste(ei_linked_point_t *liste)
{
    ei_linked_point_t *sent = liste;
    while (sent != NULL)
    {
        printf(" %i , %i  \n", sent->point.x, sent->point.y);
        sent = sent->next;
    }
    printf("\n");
}

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

ei_linked_point_t *ei_rounded_frame(ei_rect_t *rectangle,
                                    uint32_t rayon,
                                    uint8_t orientation)
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

void free_linked_point_pointeur(ei_linked_point_t *liste)
{
    ei_linked_point_t *courant = liste;
    ei_linked_point_t *suivant = liste->next;
    while (suivant != NULL)
    {
        free(courant);
        courant = suivant;
        suivant = suivant->next;
    }
    free(courant);
}

/* Pour l'instant, le bouton n'est que relevé */

void ei_draw_button(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_button_t *bouton = (ei_button_t *)widget;
    ei_rect_t *rectangle = &(widget->screen_location);

    ei_color_t color = {0x64, 0x64, 0x64, 0xff};
    ei_color_t color2 = {0xB4, 0xB4, 0xB4, 0xff};
    ei_color_t color3 = {0x8B, 0x8B, 0x8B, 0xff};
    
    /* On dessine d'abord les parties hautes et basses */
    ei_linked_point_t *partie_haute = ei_rounded_frame(rectangle, *(bouton->corner_radius), 1);
    ei_linked_point_t *partie_basse = ei_rounded_frame(rectangle, *(bouton->corner_radius), 2);
    
    ei_draw_polygon(surface, partie_haute, color, clipper);
    ei_draw_polygon(surface, partie_basse, color2, clipper);

    /* Puis on dessine un plus petit rounded rectangle par dessus */
    rectangle->top_left.x += *((ei_toplevel_t *)widget)->border_width;
    rectangle->top_left.y += *((ei_toplevel_t *)widget)->border_width;
    rectangle->size.width -= 2 * *((ei_toplevel_t *)widget)->border_width;
    rectangle->size.height -= 2 * *((ei_toplevel_t *)widget)->border_width;
    ei_linked_point_t *partie_milieu = ei_rounded_frame(rectangle, (int)(2 * (float)(*(bouton->corner_radius)) / 3), 0);

    ei_draw_polygon(surface, partie_milieu, color3, clipper);

    /* Gestion de l'affichage du text sur le bouton */
    //widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;
    ei_color_t text_color = *((ei_button_t *)widget)->text_color;
    char **text = ((ei_button_t *)widget)->text;
    ei_point_t point = widget->screen_location.top_left;
    //ei_font_t font = (*((ei_button_t *)widget)->text_font != NULL) ? (NULL) : *((ei_button_t *)widget)->text_font;

    ei_draw_text(surface, &point, *text, NULL, text_color, NULL);

    free_linked_point_pointeur(partie_haute);
    free_linked_point_pointeur(partie_basse);
    free_linked_point_pointeur(partie_milieu);
}

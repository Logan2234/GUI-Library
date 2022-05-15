#include <math.h>
#include <stdlib.h>

#include "ei_autre_fonctions.h"
#include "ei_autre_draw.h"
#include "ei_autre_struct.h"

ei_linked_point_t *ei_draw_arc(ei_point_t center, uint32_t rayon, float angle_debut, float angle_fin)
{
    ei_linked_point_t *liste_point = malloc(sizeof(ei_linked_point_t));
    uint32_t longueur_arc = abs((int)((float)rayon * (angle_fin - angle_debut)));
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
            suivant = suivant->next;
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

void ei_draw_losange(ei_rect_t *rectangle, int largeur, ei_color_t color, ei_bool_t raised, ei_surface_t surface, ei_rect_t *clipper)
{
    ei_linked_point_t *partie_haute = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel_haut = partie_haute;
    ei_linked_point_t *partie_basse = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel_basse = partie_basse;
    ei_linked_point_t *partie_milieu = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel_milieu = partie_milieu;
    sentinel_haut->point.x = (rectangle->size.width / 2) + rectangle->top_left.x;
    sentinel_haut->point.y = rectangle->top_left.y;
    sentinel_milieu->point.x = sentinel_haut->point.x;
    sentinel_milieu->point.y = sentinel_haut->point.y + largeur;

    ei_linked_point_t *nouveau_haute = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *nouveau_milieu = calloc(1, sizeof(ei_linked_point_t));
    nouveau_haute->point.x = rectangle->size.width + rectangle->top_left.x;
    nouveau_haute->point.y = (rectangle->size.height / 2) + rectangle->top_left.y;
    nouveau_milieu = nouveau_haute;
    nouveau_milieu->point.x -= largeur;
    sentinel_basse = nouveau_haute;
    sentinel_haut->next = nouveau_haute;
    sentinel_milieu->next = nouveau_milieu;
    sentinel_haut = sentinel_haut->next;
    sentinel_milieu = sentinel_milieu->next;

    ei_linked_point_t *nouveau_milieu_1 = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *nouveau_basse = calloc(1, sizeof(ei_linked_point_t));
    nouveau_basse->point.x = (rectangle->size.width / 2) + rectangle->top_left.x;
    nouveau_basse->point.y = rectangle->size.height + rectangle->top_left.y;
    nouveau_milieu_1 = nouveau_basse;
    nouveau_milieu_1->point.y -= largeur;
    sentinel_milieu->next = nouveau_milieu_1;
    sentinel_milieu = sentinel_milieu->next;
    sentinel_basse->next = nouveau_basse;
    sentinel_basse = sentinel_basse->next;

    ei_linked_point_t *nouveau_milieu_2 = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *nouveau_basse_1 = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *nouveau_haute_1 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_haute_1->point.x = rectangle->top_left.x;
    nouveau_haute_1->point.y = (rectangle->size.height / 2) + rectangle->top_left.y;
    nouveau_basse_1 = nouveau_haute_1;
    nouveau_milieu_2 = nouveau_haute_1;
    nouveau_milieu_2->point.x += largeur;
    sentinel_haut->next = nouveau_haute_1;
    sentinel_milieu->next = nouveau_milieu_2;
    sentinel_basse->next = nouveau_basse_1;

    ei_color_t color1 = color;
    ei_color_t color2 = color;

    if (raised == EI_TRUE)
    {
        color1 = (ei_color_t){0xB4, 0xB4, 0xB4, 0xff};
        color2 = (ei_color_t){0x64, 0x64, 0x64, 0xff};
    }
    else
    {
        color1 = (ei_color_t){0x64, 0x64, 0x64, 0xff};
        color2 = (ei_color_t){0xB4, 0xB4, 0xB4, 0xff};
        color = (ei_color_t){0x9E, 0x0E, 0x40, 0xff};
    }

    ei_draw_polygon(surface, partie_haute, color1, clipper);
    ei_draw_polygon(surface, partie_basse, color2, clipper);
    ei_draw_polygon(surface, partie_milieu, color, clipper);

    free_linked_point_pointeur(partie_haute);
    free_linked_point_pointeur(partie_milieu);
    free_linked_point_pointeur(partie_basse);
}

void ei_draw_img(ei_surface_t surface, const ei_point_t *where, const char **filename, const ei_rect_t *clipper)
{
    if (filename != NULL)
    {
        ei_surface_t *surface_source = hw_image_load(*filename, surface);
        ei_rect_t source = hw_surface_get_rect(surface_source);
        ei_rect_t dest;
        dest.size = (clipper == NULL) ? (hw_surface_get_size(surface_source)) : clipper->size;

        dest.top_left.x = where->x;
        dest.top_left.y = where->y;

        ei_copy_surface(surface, &dest, surface_source, &source, EI_FALSE);
        // hw_surface_free(surface_source);
    }
}
#include "ei_geometrymanager.h"

/*
 Configure la géométrie d'un widget à l'aide du gestionnaire de géométrie "placer".
 * Si le widget était déjà géré par un autre gestionnaire de géométrie, alors il est d'abord
   supprimé du gestionnaire de géométrie précédent. 
 * Si le widget était déjà géré par le "placer", alors cela appelle simplement les mises à jour
   les paramètres placer : les arguments non NULL remplacent les valeurs précédentes.
 * Lorsque les arguments sont passés comme NULL, le placer utilise les valeurs par défaut (détaillées dans
   les descriptions des arguments ci-dessous). Si aucune taille n'est fournie (absolue ou
   relatif), alors la taille demandée du widget est utilisée, c'est-à-dire la taille minimale
   requis pour afficher son contenu. */
void ei_place(ei_widget_t *widget,
              ei_anchor_t *anchor,
              int *x,
              int *y,
              int *width,
              int *height,
              float *rel_x,
              float *rel_y,
              float *rel_width,
              float *rel_height)
{
    /* J'ai pas fait avec les relatives uniquement avec les absolus pour tester au début*/
    ei_widget_t *parent = widget->parent;
    widget->screen_location.size.width = *width; /* parent->screen_location->size->width * rel_width; */
    widget->screen_location.size.height = *height; /*parent->screen_location->size->height * rel_height;*/
    ei_point_t *top_left;
    int *width2 = *width;
    int *height2 = *height;
    if (*height == NULL){
        *height2 = widget->requested_size.height;
    }
    if (*width == NULL){
        *width2 = widget->requested_size.width;
    }
    switch (*anchor){
        case ei_anc_center:
            top_left->x = *x - *width2/2;
            top_left->y = *y - *height2/2;
        case ei_anc_north:
            top_left->x = *x - *width2/2;
            top_left->y = *y;
        case ei_anc_south:
            top_left -> x = *x - *width2/2;
            top_left -> y = *y - *height2;
        case ei_anc_east:
            top_left -> x = *x - *width2;
            top_left -> y = *y - *height2/2;
        case ei_anc_west:
            top_left -> x = *x;
            top_left -> y = *y - *height2/2;
        case ei_anc_northeast:
            top_left -> x = *x;
            top_left -> y = *y;
        case ei_anc_northwest:
            top_left -> x = *x - *width2;
            top_left -> y = *y;
        case ei_anc_southeast:
            top_left -> x = *x - *width2;
            top_left -> y = *y - *height2;
        case ei_anc_southwest:
            top_left -> x = *x;
            top_left -> y = *y - *height2;
    widget->screen_location.top_left = *top_left;
    }
}

void ei_geometrymanager_unmap(ei_widget_t*	widget)
{
    
}

ei_geometrymanager_t* ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{

}

void ei_geometrymanager_register (ei_geometrymanager_t* geometrymanager)
{

}


void ei_register_placer_manager (void)
{

}


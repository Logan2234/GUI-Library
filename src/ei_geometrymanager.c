#include "ei_geometrymanager.h"

/*
 Configure la géométrie d'un widget à l'aide du gestionnaire de géométrie "placer".
 * Si le widget était déjà géré par un autre gestionnaire de géométrie, alors il est d'abord
 * supprimé du gestionnaire de géométrie précédent.
 * Si le widget était déjà géré par le "placer", alors cela appelle simplement les mises à jour
 * les paramètres placer : les arguments non NULL remplacent les valeurs précédentes.
 * Lorsque les arguments sont passés comme NULL, le placer utilise les valeurs par défaut (détaillées dans
 * les descriptions des arguments ci-dessous). Si aucune taille n'est fournie (absolue ou
 * relatif), alors la taille demandée du widget est utilisée, c'est-à-dire la taille minimale
 * requis pour afficher son contenu. */
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
    ei_widget_t *parent = widget->parent;
    widget->screen_location;
    widget->screen_location->size->width = parent->screen_location->size->width * rel_width;
    widget->screen_location->size->height = parent->screen_location->size->height * rel_height;
    ei_point_t *top_left;
    switch (anchor){
        case ei_anc_center:
            top_left -> x = parent->screen_location->top_left->x + rel_x * parent->screen_location->size->width -  ;
            top_left -> y =;
    }
}

void ei_geometrymanager_unmap(ei_widget_t*	widget)
{
    include
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


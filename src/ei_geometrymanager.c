#include "ei_geometrymanager.h"

/*
 Configure la géométrie d'un widget à l'aide du gestionnaire de géométrie "placer".
 * Si le widget était déjà géré par un autre gestionnaire de géométrie, alors il est d'abord
   supprimé du gestionnaire de géométrie précédent. // TODO
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
    ei_point_t *top_left;
    top_left->x =0;
    top_left->y = 0;

    ei_widget_t *parent = widget->parent;
    int width_parent = 0;
    int height_parent = 0;

    if (parent != NULL){
        //printf("%d", top_left->x);
        top_left->x = parent->screen_location.top_left.x;
        top_left->y = parent->screen_location.top_left.x;
        width_parent = parent->screen_location.size.width;
        height_parent = parent->screen_location.size.height;
    }

    int width2;
    int height2;
    int rel_x2;
    int rel_y2;

    rel_x2 = ((rel_x == NULL) ? 0 : *rel_x);
    rel_y2 = ((rel_y == NULL) ? 0 : *rel_y); 
    height2 = ((height == NULL) ? widget->requested_size.height : *height);
    // height2 += *rel_height;
    width2 = ((width == NULL) ? widget->requested_size.width : *width);
    // width2 += *rel_width;

    widget->screen_location.size.width = width2; /* parent->screen_location->size->width * rel_width; */
    widget->screen_location.size.height = height2; /*parent->screen_location->size->height * rel_height;*/
    if (anchor == NULL)
    {
        top_left -> x += *x + rel_x2 * width_parent;
        top_left -> y += *y + rel_y2 * height_parent;    
    }
    else
    {
        switch (*anchor){
            case ei_anc_none:
                top_left -> x = *x + *rel_x * width_parent;
                top_left -> y = *y + *rel_y * height_parent;
            case ei_anc_center:
                top_left->x = *x + *rel_x * width_parent - (width2 + *rel_width)/2;
                top_left->y = *y + *rel_y * height_parent - height2/2;
            case ei_anc_north:
                top_left->x = *x + *rel_x * width_parent - (width2 + *rel_width)/2;
                top_left->y = *y + *rel_y * height_parent;
            case ei_anc_south:
                top_left -> x = *x + *rel_x * width_parent- (width2 + *rel_width)/2;
                top_left -> y = *y + *rel_y * height_parent- (height2 + *rel_height);
            case ei_anc_east:
                top_left -> x = *x + *rel_x * width_parent - (width2 + *rel_width);
                top_left -> y = *y + *rel_y * height_parent- (height2 + *rel_height)/2;
            case ei_anc_west:
                top_left -> x = *x + *rel_x * width_parent;
                top_left -> y = *y + *rel_y * height_parent- (height2 + *rel_height)/2;
            case ei_anc_northeast:
                top_left -> x = *x + *rel_x * width_parent- (width2 + *rel_width);
                top_left -> y = *y + *rel_y * height_parent;
            case ei_anc_northwest:
                top_left -> x = *x + *rel_x * width_parent;
                top_left -> y = *y + *rel_y * height_parent;
            case ei_anc_southeast:
                top_left -> x = *x + *rel_x * width_parent - (width2 + *rel_width);
                top_left -> y = *y + *rel_y * height_parent- (height2 + *rel_height);
            case ei_anc_southwest:
                top_left -> x = *x + *rel_x * width_parent;
                top_left -> y = *y + *rel_y * height_parent - (height2 + *rel_height);
        }
    }
    widget->screen_location.top_left = *top_left;
    free(top_left);
    free(parent);
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


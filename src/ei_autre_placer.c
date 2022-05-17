#include "ei_geometrymanager.h"
#include "ei_autre_placer.h"
#include "ei_autre_struct.h"

void placer_runfunc(ei_widget_t *widget)
{
    /* Gestion du paramètre geom_params du widget */
    int width_parent = widget->requested_size.width;
    int height_parent = widget->requested_size.height;

    /* Initialisation des variables pour contrer les NULL */
    ei_point_t *top_left = calloc(1, sizeof(ei_point_t));
    ei_placer_t *placer = (ei_placer_t *)widget->geom_params;
    if (widget->parent != NULL)
    {
        top_left->x = ((placer->rel_x != -1) ? widget->parent->content_rect->top_left.x : widget->parent->content_rect->top_left.x);
        top_left->y = ((placer->rel_y != -1) ? widget->parent->content_rect->top_left.y : widget->parent->content_rect->top_left.y);
        width_parent = widget->parent->content_rect->size.width;
        height_parent = widget->parent->content_rect->size.height;
    }
    placer->rel_x = ((placer->rel_x == -1) ? 0 : placer->rel_x);
    placer->rel_y = ((placer->rel_y == -1) ? 0 : placer->rel_y);

    int taille_width = (placer->rel_width == 0) ? placer->width : placer->rel_width * width_parent;
    int taille_height = (placer->rel_height == 0 ) ?placer->height : placer->rel_height * height_parent;
    if (placer->anchor == NULL)
    {
        top_left->x += placer->x + placer->rel_x * width_parent;
        top_left->y += placer->y + placer->rel_y * height_parent;
    }
    else
    {
        switch (*placer->anchor)
        {
        case ei_anc_none:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_center:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width / 2;
            top_left->y += placer->y + placer->rel_y * height_parent - placer->height / 2;
            break;
        case ei_anc_north:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width / 2;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_south:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width / 2;
            top_left->y += placer->y + placer->rel_y * height_parent - taille_height;
            break;
        case ei_anc_east:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width;
            top_left->y += placer->y + placer->rel_y * height_parent - taille_height / 2;
            break;
        case ei_anc_west:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent - taille_height / 2;
            break;
        case ei_anc_northeast:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_northwest:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_southeast:
            top_left->x += placer->x + placer->rel_x * width_parent - taille_width;
            top_left->y += placer->y + placer->rel_y * height_parent - taille_height;
            break;
        case ei_anc_southwest:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent - taille_height;
            break;
        }
    }

    /* Maintenant on remplace dans les données de widgets */
    widget->screen_location.size.width = taille_width;
    widget->screen_location.size.height = taille_height;
    widget->screen_location.top_left = *top_left;

    // if (!strcmp(widget->wclass->name, "frame"))
    // {
    //     printf("%i %i\n", widget->screen_location.size.width, *((ei_frame_t *)widget)->border_width);
    //     ei_frame_t *frame_aux =  (ei_frame_t *)widget;
    //     int *borderazo = frame_aux->border_width;
    //     printf("%i \n", *borderazo);
    //     // TODO Le problème vient de *((ei_frame_t *)widget)->border_width
    //     widget->content_rect->size.width = widget->screen_location.size.width ;//- 2 * *((ei_frame_t *)widget)->border_width;
    //     /*widget->content_rect->size.height = widget->screen_location.size.height - 2 * *((ei_frame_t *)widget)->border_width;
    //     widget->content_rect->top_left.x = widget->screen_location.top_left.x + *((ei_frame_t *)widget)->border_width;
    //     widget->content_rect->top_left.y = widget->screen_location.top_left.y + *((ei_frame_t *)widget)->border_width;
    //  }

    if (!strcmp(widget->wclass->name, "frame"))
    {
        widget->content_rect->size.width = widget->screen_location.size.width - 2 * *((ei_frame_t *)widget)->border_width;
        widget->content_rect->size.height = widget->screen_location.size.height - 2 * *((ei_frame_t *)widget)->border_width;
        widget->content_rect->top_left.x = widget->screen_location.top_left.x + *((ei_frame_t *)widget)->border_width;
        widget->content_rect->top_left.y = widget->screen_location.top_left.y + *((ei_frame_t *)widget)->border_width;
    }

     else if (!strcmp(widget->wclass->name, "button"))
     {
         widget->content_rect->size.width = widget->screen_location.size.width - 2 * *((ei_button_t *)widget)->border_width;
         widget->content_rect->size.height = widget->screen_location.size.height - 2 * *((ei_button_t *)widget)->border_width;
         widget->content_rect->top_left.x = widget->screen_location.top_left.x + *((ei_button_t *)widget)->border_width;
         widget->content_rect->top_left.y = widget->screen_location.top_left.y + *((ei_button_t *)widget)->border_width;
     }

     else if (!strcmp(widget->wclass->name, "toplevel"))
     {
         widget->content_rect->size.width = widget->screen_location.size.width;
         widget->content_rect->size.height = widget->screen_location.size.height;
         widget->content_rect->top_left.x = widget->screen_location.top_left.x;
         widget->content_rect->top_left.y = widget->screen_location.top_left.y + taille_header;
     }

     else
         widget->content_rect = &widget->screen_location;

     free(top_left);
}

void placer_releasefunc(struct ei_widget_t *widget)
{
    free((struct ei_placer_t *)widget->geom_params);
}

ei_geometrymanager_t *return_geometry_manager_placer()
{
    ei_geometrymanager_t *placer = calloc(1, sizeof(ei_geometrymanager_t));
    strcpy(placer->name, "placer");
    placer->runfunc = &placer_runfunc;
    placer->releasefunc = &placer_releasefunc;
    placer->next = NULL;

    return placer;
}
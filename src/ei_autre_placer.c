#include "ei_geometrymanager.h"
#include "ei_autre_placer.h"

void placer_runfunc(struct ei_placer_t *placer)
{
    /* Gestion du paramètre geom_params du widget */
    int width_parent = placer->widget->requested_size.width;
    int height_parent = placer->widget->requested_size.height;
    placer->widget->geom_params = ei_geometrymanager_from_name("placer");
    /* Initialisation des variables pour contrer les NULL */
    ei_point_t *top_left = calloc(1, sizeof(ei_point_t));
    if (placer->widget->parent != NULL)
    {
        top_left->x = ((placer->rel_x != -1) ? placer->widget->parent->content_rect->top_left.x : 0);
        top_left->y = ((placer->rel_y != -1) ? placer->widget->parent->content_rect->top_left.y : 0);
        width_parent = placer->widget->parent->content_rect->size.width;
        height_parent =placer->widget->parent->content_rect->size.height;
    }
    placer->rel_x = ((placer->rel_x == -1) ? 0 : placer->rel_x);
    placer->rel_y = ((placer->rel_y == -1) ? 0 : placer->rel_y);
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
            top_left->x += placer->x + placer->rel_x* width_parent - (placer->width + placer->rel_width) / 2;
            top_left->y += placer->y + placer->rel_y* height_parent - placer->height / 2;
            break;
        case ei_anc_north:
            top_left->x += placer->x + placer->rel_x * width_parent - (placer->width + placer->rel_width) / 2;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_south:
            top_left->x += placer->x + placer->rel_x * width_parent - (placer->width + placer->rel_width) / 2;
            top_left->y += placer->y + placer->rel_y * height_parent - (placer->height + placer->rel_height);
            break;
        case ei_anc_east:
            top_left->x += placer->x + placer->rel_x * width_parent - (placer->width + placer->rel_width);
            top_left->y += placer->y + placer->rel_y * height_parent - (placer->height + placer->rel_height) / 2;
            break;
        case ei_anc_west:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent - (placer->height + placer->rel_height) / 2;
            break;
        case ei_anc_northeast:
            top_left->x += placer->x + placer->rel_x * width_parent - (placer->width + placer->rel_width);
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_northwest:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent;
            break;
        case ei_anc_southeast:
            top_left->x += placer->x + placer->rel_x * width_parent - (placer->width + placer->rel_width);
            top_left->y += placer->y + placer->rel_y * height_parent - (placer->height + placer->rel_height);
            break;
        case ei_anc_southwest:
            top_left->x += placer->x + placer->rel_x * width_parent;
            top_left->y += placer->y + placer->rel_y * height_parent - (placer->height + placer->rel_height);
            break;
        }
    }

    /* Maintenant on remplace dans les données de widgets */
    placer->widget->content_rect->size.width = placer->width;
    placer->widget->content_rect->size.height = placer->height;
    
    placer->widget->content_rect->top_left = *top_left;
    free(top_left);

}

void placer_releasefunc(struct ei_widget_t *widget)
{
    // TODO
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
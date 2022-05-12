#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"

extern int widget_id;

struct ei_widget_t *toplevel_allocfunc(void)
{
    ei_toplevel_t *widget_toplevel = calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)widget_toplevel;
}

void toplevel_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free((ei_toplevel_t *)widget);
}

void toplevel_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_linked_point_t *premier_point = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel = premier_point;
    premier_point->point.x = widget->screen_location.top_left.x;
    premier_point->point.y = widget->screen_location.top_left.y;

    ei_linked_point_t *nouveau = calloc(1, sizeof(ei_linked_point_t));
    nouveau->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau->point.y = widget->screen_location.top_left.y;
    sentinel->next = nouveau;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau1 = calloc(1, sizeof(ei_linked_point_t));
    nouveau1->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau1->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sentinel->next = nouveau1;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau2->point.x = widget->screen_location.top_left.x;
    nouveau2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sentinel->next = nouveau2;

    ei_draw_polygon(surface, premier_point, *(((ei_toplevel_t *)widget)->color), clipper);

    // JE VAIS METTRE ICI LA CRÉATION DU BUTTON MAIS JE NE SAIS PAS S'IL DEVRAIT ALLER LÀ

    free_linked_point_pointeur(premier_point);

    /*
    // Il faut encore faire le callback du bouton et l'écriture
    // TODO
    ei_fill(surface, ((ei_toplevel_t *)widget)->color, clipper);
    ei_fill(pick_surface, widget->pick_color, clipper);
     */
}

void toplevel_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    widget_id++;

    widget->user_data = NULL;
    widget->destructor = NULL; /* Il faut créer la fonction */
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;

    widget->requested_size = default_toplevel_size;
    widget->screen_location = (ei_rect_t){0, 0, default_toplevel_size};
    widget->content_rect = &widget->screen_location;
}

void toplevel_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

ei_widgetclass_t *return_class_toplevel(void)
{
    ei_widgetclass_t *widgetclass_toplevel = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_toplevel->name, "toplevel");
    widgetclass_toplevel->allocfunc = &toplevel_allocfunc;
    widgetclass_toplevel->releasefunc = &toplevel_releasefunc;
    widgetclass_toplevel->drawfunc = &toplevel_drawfunc;
    widgetclass_toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
    widgetclass_toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
    widgetclass_toplevel->next = NULL;

    return widgetclass_toplevel;
}

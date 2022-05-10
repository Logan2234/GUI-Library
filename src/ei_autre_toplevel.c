#include "ei_autre_struct.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"

struct ei_widget_t *toplevel_allocfunc(void)
{
    ei_toplevel_t *widget_toplevel = calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)widget_toplevel;
}

void toplevel_releasefunc(struct ei_widget_t *widget)
{
    free((ei_toplevel_t *)widget);
}

void toplevel_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    // TODO
    ei_fill(surface, ((ei_toplevel_t *)widget)->color, clipper);
    ei_fill(pick_surface, widget->pick_color, clipper);
}

void toplevel_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = 2;

    widget->pick_color = &(ei_color_t){0x00, 0xff, 0x00, 0xff};

    widget->user_data = NULL;
    widget->destructor = NULL; /* Il faut créer la fonction */
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;

    widget->requested_size = (ei_size_t){320, 240};
    widget->screen_location = (ei_rect_t){0, 0, widget->requested_size};
    widget->content_rect = NULL;
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

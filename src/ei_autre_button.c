#include "ei_application.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_autre_struct.h"

struct ei_widget_t *button_allocfunc(void)
{
    ei_button_t *widget_button = calloc(1, sizeof(ei_button_t));
    return (ei_widget_t *)widget_button;
}

void button_releasefunc(struct ei_widget_t *widget)
{
    free((ei_button_t *)widget);
}

void button_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

/* À vérifier */
void button_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    // TODO
    // ei_fill(surface, ((ei_button_t *)widget)->color, clipper);
    // ei_fill(pick_surface, widget->pick_color, clipper);
}

void button_setdefaultsfunc(struct ei_widget_t *widget) /* À reviser */
{
    widget->pick_id = 0;

    widget->pick_color = &(ei_color_t){0x00, 0x00, 0x00, 0xff};

    widget->user_data = NULL;
    widget->destructor = NULL; /* Il faut créer la fonction */
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;

    widget->requested_size = (ei_size_t){100, 50};
    widget->screen_location = (ei_rect_t){0, 0, (ei_size_t){100, 50}};
    widget->content_rect = NULL;
}

ei_widgetclass_t *return_class_button(void)
{
    ei_widgetclass_t *widgetclass_button = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_button->name, "button");
    widgetclass_button->allocfunc = &button_allocfunc;
    widgetclass_button->releasefunc = &button_releasefunc;
    widgetclass_button->drawfunc = &button_drawfunc;
    widgetclass_button->setdefaultsfunc = &button_setdefaultsfunc;
    widgetclass_button->geomnotifyfunc = &button_geomnotifyfunc;
    widgetclass_button->next = NULL;

    return widgetclass_button;
}
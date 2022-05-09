#include "ei_autre_struct.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"

struct ei_widget_t *frame_allocfunc(void)
{
    ei_frame_t *widget_frame = calloc(1, sizeof(ei_frame_t));
    return (ei_widget_t *)widget_frame;
}

void frame_releasefunc(struct ei_widget_t *widget)
{
    free((ei_frame_t *)widget);
}

void frame_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    if (((ei_frame_t *)widget)->relief != ei_relief_none && ((ei_frame_t *)widget)->border_width > 0)
    {
        if (((ei_frame_t *)widget)->relief == ei_relief_raised)
        {

        }
        else
        {
            
        }
    }
    else
        ei_fill(surface, ((ei_frame_t *)widget)->color, clipper);
    ei_fill(pick_surface, widget->pick_color, clipper);
}

void frame_setdefaultsfunc(struct ei_widget_t *widget)
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

    widget->requested_size = (ei_size_t){600, 600};
    widget->screen_location = (ei_rect_t){0, 0, (ei_size_t){600, 600}};
    widget->content_rect = NULL;
}

void frame_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

ei_widgetclass_t *return_class_frame(void)
{
    ei_widgetclass_t *widgetclass_frame = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_frame->name, "frame");
    widgetclass_frame->allocfunc = &frame_allocfunc;
    widgetclass_frame->releasefunc = &frame_releasefunc;
    widgetclass_frame->drawfunc = &frame_drawfunc;
    widgetclass_frame->setdefaultsfunc = &frame_setdefaultsfunc;
    widgetclass_frame->geomnotifyfunc = &frame_geomnotifyfunc;
    widgetclass_frame->next = NULL;

    return widgetclass_frame;
}

#include "ei_autre_frame.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_types.h"

struct ei_widget_t* frame_allocfunc(void)
{
    ei_widget_t *widget_frame = calloc(1, sizeof(ei_widget_t));
    return widget_frame;
}

void frame_releasefunc(struct ei_widget_t* widget)
{
    free(widget);
    // TODO
}

void frame_drawfunc(struct ei_widget_t* widget,
                    ei_surface_t        surface,
                    ei_surface_t        pick_surface,
                    ei_rect_t*          clipper)
{
    ei_fill(surface, ((ei_frame_t *)widget)->color, clipper);
    ei_fill(pick_surface, widget->pick_color, clipper);
}

void frame_setdefaultsfunc(struct ei_widget_t* widget)
{
    widget->wclass = return_class_frame();
    widget->pick_id = 0;

    ei_color_t*	fond = &ei_default_background_color;
    widget->pick_color = fond;

    widget->user_data = NULL;
    widget->destructor = NULL; /* Il faut créer la fonction */
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;
    
    ei_size_t taille;
    taille.width = 600;
    taille.height = 600;
    
    widget->requested_size = taille;
    ei_rect_t rect;
    rect.top_left.x = 0;
    rect.top_left.y = 0;
    rect.size = taille;
    widget->screen_location = rect;
    widget->content_rect = NULL;
}

void frame_geomnotifyfunc(struct ei_widget_t* widget)
{
    // TODO
}

ei_widgetclass_t *return_class_frame()
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

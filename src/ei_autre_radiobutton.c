#include "ei_autre_struct.h"

struct ei_widget_t *radiobutton_allocfunc(void)
{
    return (ei_widget_t *)calloc(1, sizeof(ei_radiobutton_t));
}

void radiobutton_releasefunc(struct ei_widget_t *widget)
{
    free((ei_radiobutton_t *)widget);
}

void radiobutton_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{

    struct liste_textes *textes = ((ei_radiobutton_t *) widget)->textes;
    while (textes != NULL)
    {
        //  TODO
        textes = textes->next;
    }

}

void radiobutton_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

void radiobutton_setdefaultsfunc(struct ei_widget_t *widget) /* À reviser */
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

ei_widgetclass_t *return_class_radiobutton(void)
{
    ei_widgetclass_t *widgetclass_button = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_button->name, "radiobutton");
    widgetclass_button->allocfunc = &radiobutton_allocfunc;
    widgetclass_button->releasefunc = &radiobutton_releasefunc;
    widgetclass_button->drawfunc = &radiobutton_drawfunc;
    widgetclass_button->setdefaultsfunc = &radiobutton_setdefaultsfunc;
    widgetclass_button->geomnotifyfunc = &radiobutton_geomnotifyfunc;
    widgetclass_button->next = NULL;

    return widgetclass_button;
}
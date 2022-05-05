#include <string.h>

#include "ei_widget.h"
#include "ei_autre_struct.h"

extern struct liste_widgetclass *liste_widgetclass;

void ei_frame_configure(ei_widget_t *widget,
                        ei_size_t *requested_size,
                        const ei_color_t *color,
                        int *border_width,
                        ei_relief_t *relief,
                        char **text,
                        ei_font_t *text_font,
                        ei_color_t *text_color,
                        ei_anchor_t *text_anchor,
                        ei_surface_t *img,
                        ei_rect_t **img_rect,
                        ei_anchor_t *img_anchor)
{
    ((ei_frame_t *)widget)->requested_size = requested_size;
    ((ei_frame_t *)widget)->color = color;
    ((ei_frame_t *)widget)->border_width = border_width;
    ((ei_frame_t *)widget)->relief = relief;
    ((ei_frame_t *)widget)->text = text;
    ((ei_frame_t *)widget)->text_font = text_font;
    ((ei_frame_t *)widget)->text_color = text_color;
    ((ei_frame_t *)widget)->text_anchor = text_anchor;
    ((ei_frame_t *)widget)->img = img;
    ((ei_frame_t *)widget)->img_rect = img_rect;
    ((ei_frame_t *)widget)->img_anchor = img_anchor;
}

void ei_button_configure(ei_widget_t *widget,
                         ei_size_t *requested_size,
                         const ei_color_t *color,
                         int *border_width,
                         int *corner_radius,
                         ei_relief_t *relief,
                         char **text,
                         ei_font_t *text_font,
                         ei_color_t *text_color,
                         ei_anchor_t *text_anchor,
                         ei_surface_t *img,
                         ei_rect_t **img_rect,
                         ei_anchor_t *img_anchor,
                         ei_callback_t *callback,
                         void **user_param)
{
    ((ei_button_t *)widget)->requested_size = requested_size;
    ((ei_button_t *)widget)->color = color;
    ((ei_button_t *)widget)->border_width = border_width;
    ((ei_button_t *)widget)->corner_radius = corner_radius;
    ((ei_button_t *)widget)->relief = relief;
    ((ei_button_t *)widget)->text = text;
    ((ei_button_t *)widget)->text_font = text_font;
    ((ei_button_t *)widget)->text_color = text_color;
    ((ei_button_t *)widget)->text_anchor = text_anchor;
    ((ei_button_t *)widget)->img = img;
    ((ei_button_t *)widget)->img_rect = img_rect;
    ((ei_button_t *)widget)->img_anchor = img_anchor;
    ((ei_button_t *)widget)->callback = callback;
    ((ei_button_t *)widget)->user_param = user_param;
}

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name,
                              ei_widget_t *parent,
                              void *user_data,
                              ei_widget_destructor_t destructor)
{
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
    {
        if (!strcmp(liste_widgetclass->first_widgetclass->name, class_name))
        {
            ei_widget_t *class = liste_widgetclass->first_widgetclass->allocfunc();
            /* Puis on rentre les paramètres fournis en paramètre de ei_widget_create */
            class->wclass = liste_widgetclass->first_widgetclass;
            class->wclass->setdefaultsfunc(class); /* Notre nouveau widget prend les paramètres par défaut */
            class->parent = parent;
            class->user_data = user_data;
            class->destructor = destructor;
            return class;
        }
        else
        {
            liste_widgetclass = liste_widgetclass->next;
        }
    }
    return NULL;
}

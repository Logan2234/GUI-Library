#include "ei_widget.h"
#include "ei_autre_frame.h"
#include "ei_autre_struct.h"

#include <string.h>

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
    ((ei_frame_t*)widget)->requested_size = requested_size;
    // ((ei_frame_t*)widget)->color = color;
	// ((ei_frame_t*)widget)->border_width = border_width;
    ((ei_frame_t*)widget)->relief = relief;
    ((ei_frame_t*)widget)->text = text;
    ((ei_frame_t*)widget)->text_font = text_font;
    ((ei_frame_t*)widget)->text_color = text_color;
    ((ei_frame_t*)widget)->text_anchor = text_anchor;
    ((ei_frame_t*)widget)->img = img;
    ((ei_frame_t*)widget)->img_rect = img_rect;
    ((ei_frame_t*)widget)->img_anchor = img_anchor;
}

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name,
                              ei_widget_t *parent,
                              void *user_data,
                              ei_widget_destructor_t destructor)
{  
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL){
        if (!strcmp(liste_widgetclass->first_widgetclass->name, class_name))
        {
            ei_widget_t *class = liste_widgetclass->first_widgetclass->allocfunc();
            class->wclass = liste_widgetclass->first_widgetclass;
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
}



#include <string.h>

#include "ei_widget.h"
#include "ei_autre_struct.h"

extern struct liste_widgetclass *liste_widgetclass;

void ajout_relation_parent(ei_widget_t *pere, ei_widget_t *fils)
{
    if (pere->children_head == NULL)
    {
        pere->children_head = fils;
        pere->children_tail = fils;
    }
    else
    {
        pere->children_tail->next_sibling = fils;
        pere->children_tail = fils;
    }
}

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name,
                              ei_widget_t *parent,
                              void *user_data,
                              ei_widget_destructor_t destructor)
{
    struct liste_widgetclass *sent = liste_widgetclass;
    while (sent != NULL && sent->first_widgetclass != NULL)
    {
        if (!strcmp(sent->first_widgetclass->name, class_name))
        {
            ei_widget_t *class = sent->first_widgetclass->allocfunc();

            /* Puis on rentre les paramètres fournis en paramètre de ei_widget_create */
            class->wclass = sent->first_widgetclass;
            class->wclass->setdefaultsfunc(class); /* Notre nouveau widget prend les paramètres par défaut */
            class->parent = parent;
            class->user_data = user_data;
            class->destructor = destructor;

            /* Il ne faut pas oublier de dire au parent qu'il a un nouveau fils si jamais c'est pas la racine */
            (parent != NULL) ? ajout_relation_parent(parent, class) : NULL;

            return class;
        }
        else
            sent = sent->next;
    }
    return NULL;
}

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

void ei_toplevel_configure(ei_widget_t *widget,
                           ei_size_t *requested_size,
                           ei_color_t *color,
                           int *border_width,
                           char **title,
                           ei_bool_t *closable,
                           ei_axis_set_t *resizable,
                           ei_size_t **min_size)
{
    ((ei_toplevel_t *)widget)->requested_size = requested_size;
    ((ei_toplevel_t *)widget)->color = color;
    ((ei_toplevel_t *)widget)->border_width = border_width;
    ((ei_toplevel_t *)widget)->title = title;
    ((ei_toplevel_t *)widget)->closable = closable;
    ((ei_toplevel_t *)widget)->resizable = resizable;
    ((ei_toplevel_t *)widget)->min_size = min_size;
}
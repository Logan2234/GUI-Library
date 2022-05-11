#include <string.h>

#include "ei_widget.h"
#include "ei_autre_struct.h"
#include "ei_geometrymanager.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"

extern struct liste_widgetclass *liste_widgetclass;
extern struct liste_geometrymanager *liste_geometrymanager;

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name, ei_widget_t *parent, void *user_data, ei_widget_destructor_t destructor)
{
    struct liste_widgetclass *sent = liste_widgetclass;
    while (sent != NULL && sent->first_widgetclass != NULL)
    {
        if (!strcmp(sent->first_widgetclass->name, class_name))
        {
            ei_widget_t *widget = sent->first_widgetclass->allocfunc();

            /* Puis on rentre les paramètres fournis en paramètre de ei_widget_create */
            widget->wclass = sent->first_widgetclass;
            widget->wclass->setdefaultsfunc(widget); /* Notre nouveau widget prend les paramètres par défaut */
            widget->parent = parent;
            widget->user_data = user_data;
            widget->destructor = destructor;
            widget->geom_params = (ei_geometry_param_t *)(liste_geometrymanager->geometrymanager_cell);

            /* Il ne faut pas oublier de dire au parent qu'il a un nouveau fils si jamais c'est pas la racine */
            (parent != NULL) ? ajout_relation_parent(parent, widget) : NULL;
            return widget;
        }
        else
            sent = sent->next;
    }
    return NULL;
}

void ei_frame_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color, int *border_width,
                        ei_relief_t *relief, char **text, ei_font_t *text_font, ei_color_t *text_color,
                        ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect, ei_anchor_t *img_anchor)
{
    ei_anchor_t default_anchor_frame = ei_anc_center;
    ei_relief_t default_relief_frame = ei_relief_none;
    ei_frame_t *frame = (ei_frame_t *)widget;
    widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;
    frame->color = (color != NULL) ? color : (frame->color == NULL) ? &ei_default_background_color
                                                                    : frame->color;
    frame->border_width = (border_width != NULL) ? border_width : (frame->border_width == NULL) ? 0
                                                                                                : frame->border_width;
    frame->relief = (relief != NULL) ? relief : (frame->relief == NULL) ? &default_relief_frame
                                                                        : frame->relief;
    frame->text = text;
    frame->text_font = (text_font != NULL) ? text_font : (frame->text_font == NULL) ? ei_default_font
                                                                                    : frame->text_font;
    frame->text_color = (text_color != NULL) ? text_color : (frame->text_color == NULL) ? (ei_color_t *)&ei_font_default_color
                                                                                        : frame->text_color;
    frame->text_anchor = (text_anchor != NULL) ? text_anchor : (frame->text_anchor == NULL) ? &default_anchor_frame
                                                                                            : frame->text_anchor;
    frame->img = img;
    frame->img_rect = img_rect;
    frame->img_anchor = (img_anchor != NULL) ? img_anchor : (frame->img_anchor == NULL) ? &default_anchor_frame
                                                                                        : frame->img_anchor;
}

void ei_button_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color,
                         int *border_width, int *corner_radius, ei_relief_t *relief,
                         char **text, ei_font_t *text_font, ei_color_t *text_color,
                         ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect,
                         ei_anchor_t *img_anchor, ei_callback_t *callback, void **user_param)
{
    ei_button_t *bouton = (ei_button_t *)widget;
    ei_anchor_t default_anchor_button = ei_anc_center;
    ei_relief_t default_relief_button = ei_relief_raised;
    widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;
    bouton->color = (color != NULL) ? color : (bouton->color == NULL) ? &ei_default_background_color
                                                                      : bouton->color;
    bouton->border_width = (border_width != NULL) ? border_width : (bouton->border_width == NULL) ? (int *)&k_default_button_border_width
                                                                                                  : bouton->border_width;
    bouton->corner_radius = (corner_radius != NULL) ? corner_radius : (bouton->corner_radius == NULL) ? (int *)&k_default_button_corner_radius
                                                                                                      : bouton->corner_radius;
    bouton->relief = (relief != NULL) ? relief : (bouton->relief == NULL) ? &default_relief_button
                                                                          : bouton->relief;
    bouton->text = text;
    bouton->text_font = (text_font != NULL) ? text_font : (bouton->text_font == NULL) ? ei_default_font
                                                                                      : bouton->text_font;
    bouton->text_color = (text_color != NULL) ? text_color : (bouton->text_color == NULL) ? (ei_color_t *)&ei_font_default_color
                                                                                          : bouton->text_color;
    bouton->text_anchor = (text_anchor != NULL) ? text_anchor : (bouton->text_anchor == NULL) ? &default_anchor_button
                                                                                              : bouton->text_anchor;
    bouton->img = img;
    bouton->img_rect = img_rect;
    bouton->img_anchor = (img_anchor != NULL) ? img_anchor : (bouton->img_anchor == NULL) ? &default_anchor_button
                                                                                          : bouton->img_anchor;
    bouton->callback = callback;
    bouton->user_param = user_param;
}

void ei_toplevel_configure(ei_widget_t *widget, ei_size_t *requested_size, ei_color_t *color, int *border_width,
                           char **title, ei_bool_t *closable, ei_axis_set_t *resizable, ei_size_t **min_size)
{
    const int constante = 4;
    ei_axis_set_t default_axis = ei_axis_both;
    ei_bool_t vrai = EI_TRUE;
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    widget->requested_size = (requested_size != NULL) ? (*requested_size) : (&widget->requested_size == NULL) ? (ei_size_t){320, 240}
                                                                                                              : widget->requested_size;
    toplevel->color = (color != NULL) ? color : (&toplevel->color == NULL) ? (ei_color_t *)&ei_default_background_color
                                                                           : toplevel->color;
    toplevel->border_width = (border_width != NULL) ? border_width : (toplevel->border_width == NULL) ? (int *)&constante
                                                                                                      : toplevel->border_width;
    toplevel->title = (title != NULL) ? title : (toplevel->title == NULL) ? (char **)"Toplevel"
                                                                          : toplevel->title;
    toplevel->closable = (closable != NULL) ? closable : (toplevel->closable == NULL) ? &vrai
                                                                                      : toplevel->closable;
    toplevel->resizable = (resizable != NULL) ? resizable : (toplevel->resizable == NULL) ? &default_axis
                                                                                          : toplevel->resizable;
    toplevel->min_size = (min_size != NULL) ? min_size : (toplevel->min_size) ? (ei_size_t **)&(ei_size_t){160, 120}
                                                                              : toplevel->min_size;
}
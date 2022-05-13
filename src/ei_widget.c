#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"

extern struct liste_widgetclass *liste_widgetclass;
extern struct liste_geometrymanager *liste_geometrymanager;
extern ei_surface_t pick_surface;
extern int widget_id;

void ei_widget_destroy(ei_widget_t *widget)
{
    printf("->%s\n", widget->wclass->name);
    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
    {
        ei_widget_destroy(current_widget->next_sibling);
        current_widget->next_sibling = NULL;
    }

    if (current_widget->children_head != NULL)
    {
        ei_widget_destroy(current_widget->children_head);
        current_widget->children_head = NULL;
    }

    (widget->destructor != NULL) ? widget->destructor(widget) : NULL;
    widget->wclass->releasefunc(widget);
    widget = NULL;
}

ei_widget_t *ei_widget_pick(ei_point_t *where)
{
    uint32_t *picking_color_entier = (uint32_t *)hw_surface_get_buffer(pick_surface);
    picking_color_entier += where->x + where->y * hw_surface_get_size(pick_surface).width;
    return search_widget_by_id(ei_app_root_widget(), *picking_color_entier);
}

ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name, ei_widget_t *parent, void *user_data, ei_widget_destructor_t destructor)
{
    ei_widgetclass_t *widgetclass = ei_widgetclass_from_name(class_name);

    if (widgetclass == NULL)
        return NULL;

    ei_widget_t *widget = widgetclass->allocfunc();

    widget_id++;

    /* Puis on rentre les paramètres fournis en paramètre de ei_widget_create */
    widget->wclass = widgetclass;
    widget->wclass->setdefaultsfunc(widget); /* Notre nouveau widget prend les paramètres par défaut */
    widget->parent = parent;
    widget->user_data = user_data;
    widget->destructor = destructor;

    /* Il ne faut pas oublier de dire au parent qu'il a un nouveau fils si jamais c'est pas la racine */
    (parent != NULL) ? ajout_relation_parent(parent, widget) : NULL;
    return widget;
}

void ei_frame_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color, int *border_width,
                        ei_relief_t *relief, char **text, ei_font_t *text_font, ei_color_t *text_color,
                        ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect, ei_anchor_t *img_anchor)
{
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
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    widget->requested_size = (requested_size != NULL) ? (*requested_size) : (&widget->requested_size == NULL) ? (ei_size_t){320, 240}
                                                                                                              : widget->requested_size;
    toplevel->color = (color != NULL) ? color : (&toplevel->color == NULL) ? (ei_color_t *)&ei_default_background_color
                                                                           : toplevel->color;
    toplevel->border_width = (border_width != NULL) ? border_width : (toplevel->border_width == NULL) ? (int *)&constante_4
                                                                                                      : toplevel->border_width;
    toplevel->title = (title != NULL) ? title : (toplevel->title == NULL) ? &default_toplevel_title 
                                                                          : toplevel->title;
    toplevel->closable = (closable != NULL) ? closable : (toplevel->closable == NULL) ? &vrai
                                                                                      : toplevel->closable;
    toplevel->resizable = (resizable != NULL) ? resizable : (toplevel->resizable == NULL) ? &default_axis
                                                                                          : toplevel->resizable;
    toplevel->min_size = (min_size != NULL) ? min_size : (toplevel->min_size) ? (ei_size_t **)&(ei_size_t){160, 120}
                                                                              : toplevel->min_size;
}
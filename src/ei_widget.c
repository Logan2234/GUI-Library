#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"

extern struct liste_widgetclass *liste_widgetclass;
extern struct liste_geometrymanager *liste_geometrymanager;
extern ei_surface_t pick_surface;
extern int widget_id;

void ei_kill_widget(ei_widget_t *widget, ei_widget_t *origin)
{
    if (widget->children_head != NULL)
        ei_kill_widget(widget->children_head, origin);

    if (widget != origin && widget->next_sibling != NULL)
        ei_kill_widget(widget->next_sibling, origin);
    if (widget == origin && widget != ei_app_root_widget())
    {
        ei_widget_t *sent = origin->parent->children_head;
        (sent->pick_id == origin->pick_id) ? origin->parent->children_head = origin->parent->children_head->next_sibling : NULL;
        while (sent->next_sibling != NULL && sent->next_sibling->pick_id != origin->parent->pick_id)
            sent = sent->next_sibling;

        (sent->next_sibling != NULL) ? sent->next_sibling = sent->next_sibling->next_sibling : NULL;
    }
    (widget->destructor != NULL) ? widget->destructor(widget) : NULL;
    widget->wclass->releasefunc(widget);
}

void ei_widget_destroy(ei_widget_t *widget)
{
    ei_kill_widget(widget, widget);
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

    ei_color_t *color_frame = calloc(1, sizeof(ei_color_t));
    int *border_width_frame = calloc(1, sizeof(int));
    ei_relief_t *relief_frame = calloc(1, sizeof(ei_relief_t));
    char ** text_frame = calloc(1, sizeof(char *));
    ei_font_t *text_font_frame = calloc(1, sizeof(ei_font_t));
    ei_color_t *text_color_frame = calloc(1, sizeof(ei_color_t));
    ei_anchor_t *text_anchor_frame = calloc(1, sizeof(ei_anchor_t));
    ei_surface_t *img_frame = calloc(1, sizeof(ei_surface_t));
    ei_rect_t **img_rect_frame = calloc(1, sizeof(ei_rect_t *));
    ei_anchor_t *img_anchor_frame = calloc(1, sizeof(ei_anchor_t));

    *color_frame = (color != NULL) ? *color : (frame->color == NULL) ? ei_default_background_color : *frame->color;
    *border_width_frame = (border_width != NULL) ? *border_width : (frame->border_width == NULL) ? 0 : *frame->border_width;
    *relief_frame = (relief != NULL) ? *relief : (frame->relief == NULL) ? default_relief_frame : *frame->relief;
    text_frame = (text != NULL) ? text : NULL;
    *text_font_frame = (text_font != NULL) ? *text_font : (frame->text_font == NULL) ? ei_default_font : *frame->text_font;
    *text_color_frame = (text_color != NULL) ? *text_color : (frame->text_color == NULL) ? (ei_color_t)ei_font_default_color : *frame->text_color;
    *text_anchor_frame = (text_anchor != NULL) ? *text_anchor : (frame->text_anchor == NULL) ? default_anchor_frame : *frame->text_anchor;
    img_frame = (img != NULL) ? img : NULL;
    img_rect_frame = (img_rect != NULL) ? img_rect : NULL;
    *img_anchor_frame = (img_anchor != NULL) ? *img_anchor : (frame->img_anchor == NULL) ? default_anchor_frame : *frame->img_anchor;

    widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;
    
    frame->color = color_frame;
    frame->border_width = border_width_frame;
    frame->relief = relief_frame;
    frame->text = text_frame;
    frame->text_font = text_font_frame;
    frame->text_color = text_color_frame;
    frame->text_anchor = text_anchor_frame;
    frame->img = img_frame;
    frame->img_rect = img_rect_frame;
    frame->img_anchor = img_anchor_frame;
}

void ei_button_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color,
                         int *border_width, int *corner_radius, ei_relief_t *relief,
                         char **text, ei_font_t *text_font, ei_color_t *text_color,
                         ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect,
                         ei_anchor_t *img_anchor, ei_callback_t *callback, void **user_param)
{
    ei_button_t *bouton = (ei_button_t *)widget;

    ei_color_t *color_button = calloc(1, sizeof(ei_color_t));
    int *border_width_button = calloc(1, sizeof(int));
    int *corner_radius_button = calloc(1, sizeof(int));
    ei_relief_t *relief_button = calloc(1, sizeof(ei_relief_t));
    char ** text_button = calloc(1, sizeof(char *));
    ei_font_t *text_font_button = calloc(1, sizeof(ei_font_t));
    ei_color_t *text_color_button = calloc(1, sizeof(ei_color_t));
    ei_anchor_t *text_anchor_button = calloc(1, sizeof(ei_anchor_t));
    ei_surface_t *img_button = calloc(1, sizeof(ei_surface_t));
    ei_anchor_t *img_anchor_button = calloc(1, sizeof(ei_anchor_t));
    ei_rect_t **img_rect_button = calloc(1, sizeof(ei_rect_t *));
    ei_callback_t *callback_button = calloc(1, sizeof(ei_callback_t));
    void **user_param_button = calloc(1, sizeof(void *));

    *color_button = (color != NULL) ? *color : (bouton->color == NULL) ? ei_default_background_color : *bouton->color;
    *relief_button = (relief != NULL) ? *relief : (bouton->relief == NULL) ? default_relief_button : *bouton->relief;
    *border_width_button = (border_width != NULL) ? *border_width : (bouton->border_width == NULL) ? (int)k_default_button_border_width : *bouton->border_width;
    *corner_radius_button = (corner_radius != NULL) ? *corner_radius : (bouton->corner_radius == NULL) ? (int)k_default_button_corner_radius : *bouton->corner_radius;
    text_button = (text != NULL) ? text : NULL;
    *text_font_button = (text_font != NULL) ? *text_font : (bouton->text_font == NULL) ? ei_default_font : *bouton->text_font;
    *text_color_button = (text_color != NULL) ? *text_color : (bouton->text_color == NULL) ? (ei_color_t)ei_font_default_color : *bouton->text_color;
    *text_anchor_button = (text_anchor != NULL) ? *text_anchor : (bouton->text_anchor == NULL) ? default_anchor_button : *bouton->text_anchor;
    img_button = (img != NULL) ? img : NULL;
    img_rect_button = (img_rect != NULL) ? img_rect : NULL;
    *img_anchor_button = (img_anchor != NULL) ? *img_anchor : (bouton->img_anchor == NULL) ? default_anchor_button : *bouton->img_anchor;
    callback_button = (callback != NULL) ? callback : (bouton->callback != NULL) ? bouton->callback : NULL;
    user_param_button = (user_param != NULL) ? user_param : (bouton->user_param != NULL) ? bouton->user_param : NULL;

    widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;

    bouton->color = color_button;
    bouton->border_width = border_width_button;
    bouton->corner_radius = corner_radius_button;
    bouton->relief = relief_button;
    bouton->text = text_button;
    bouton->text_font = text_font_button;
    bouton->text_color = text_color_button;
    bouton->text_anchor = text_anchor_button;
    bouton->img = img_button;
    bouton->img_rect = img_rect_button;
    bouton->img_anchor = img_anchor_button;
    bouton->callback = callback_button;
    bouton->user_param = user_param_button;
}

void ei_toplevel_configure(ei_widget_t *widget, ei_size_t *requested_size, ei_color_t *color, int *border_width,
                           char **title, ei_bool_t *closable, ei_axis_set_t *resizable, ei_size_t **min_size)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    
    ei_color_t *color_toplevel = calloc(1, sizeof(ei_color_t));
    int *border_width_toplevel = calloc(1, sizeof(int));
    char ** title_toplevel = calloc(1, sizeof(char *));
    ei_bool_t *closable_toplevel = calloc(1, sizeof(ei_bool_t));
    ei_axis_set_t *resizable_toplevel = calloc(1, sizeof(ei_axis_set_t));
    ei_size_t **min_size_toplevel = calloc(1, sizeof(ei_size_t*));

    *color_toplevel = (color != NULL) ? *color : (toplevel->color == NULL) ? ei_default_background_color : *toplevel->color;
    *border_width_toplevel = (border_width != NULL) ? *border_width : (toplevel->border_width == NULL) ? 4 : *toplevel->border_width;
    title_toplevel = (title != NULL) ? title : NULL;
    *closable_toplevel = (closable != NULL) ? *closable : (toplevel->closable == NULL) ? EI_TRUE : *toplevel->closable;
    *resizable_toplevel = (resizable != NULL) ? *resizable : (toplevel->resizable == NULL) ? ei_axis_both : *toplevel->resizable;
    min_size_toplevel = (min_size != NULL) ? min_size : (toplevel->min_size == NULL) ? &default_toplevel_min_size : toplevel->min_size;

    widget->requested_size = (requested_size != NULL) ? (*requested_size) : (&widget->requested_size == NULL) ? (ei_size_t){320, 240} : widget->requested_size;
    
    toplevel->color = color_toplevel;
    toplevel->border_width = border_width_toplevel;
    toplevel->title = title_toplevel;
    toplevel->closable = closable_toplevel;
    toplevel->resizable = resizable_toplevel;
    toplevel->min_size = min_size_toplevel;
}
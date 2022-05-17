#include "ei_autre_fonctions.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"
#include "ei_autre_placer.h"
#include "ei_autre_event.h"

extern ei_surface_t racine_surface;
extern ei_surface_t pick_surface;
extern ei_bool_t is_resizing;
extern ei_bool_t is_moving;
extern ei_point_t origine_deplacement;

void draw_widgets_and_family(ei_widget_t *widget)
{
    if (widget->parent == NULL)
        widget->wclass->drawfunc(widget, racine_surface, pick_surface, NULL);
    else if (widget->geom_params != NULL)
    {
        (widget->parent->pick_id == 1)
            ? widget->wclass->drawfunc(widget, racine_surface, pick_surface, widget->content_rect)
            : widget->wclass->drawfunc(widget, racine_surface, pick_surface, widget->parent->content_rect);
    }
    else
        return;

    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        draw_widgets_and_family(current_widget->next_sibling);
    if (current_widget->children_head != NULL)
        draw_widgets_and_family(current_widget->children_head);
}

void free_linked_point_pointeur(ei_linked_point_t *liste)
{
    ei_linked_point_t *courant = liste;
    ei_linked_point_t *suivant = liste->next;
    while (suivant != NULL)
    {
        free(courant);
        courant = suivant;
        suivant = suivant->next;
    }
    free(courant);
}

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

ei_color_t int_to_color(uint32_t entier)
{
    // int ir;
    // int ig;
    // int ib;
    // int ia;
    // hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint8_t red, green, blue, alpha;
    // if (ir == 0){
    //     red = entier / (256 * 256);
    //     if (ig == 1){
    //         green = (entier - red) / 256;
    //         blue = (entier - blue - green);
    //     }
    //     else if (ib == 0){
    //         blue = (entier - red) / 256;
    //         green = (entier - blue - red);
    //     }
    // }
    red = entier / (256 * 256);
    green = (entier - red * 256 * 256) / 256;
    blue = (entier - red * 256 * 256 - green * 256);
    return (ei_color_t){red, green, blue, 0xff};
}

ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id)
{
    ei_widget_t *current_widget = widget;
    ei_widget_t *res = NULL;

    if (current_widget->pick_id == id)
        return current_widget;

    if (current_widget->children_head != NULL)
        res = search_widget_by_id(current_widget->children_head, id);

    if (res != NULL)
        return res;

    if (current_widget->next_sibling != NULL)
        res = search_widget_by_id(current_widget->next_sibling, id);

    if (res != NULL)
        return res;
    return NULL;
}

void init_toplevel(ei_widget_t *widget)
{
    ei_callback_t deplacement_callback = deplacement_toplevel;
    ei_callback_t fin_deplacement_callback = fin_deplacement_toplevel;
    ei_callback_t deplacement_actif_callback = deplacement_actif;
    ei_callback_t close_toplevel_widget = close_toplevel;

    if (!strcmp(widget->wclass->name, "toplevel"))
    {
        if (*((ei_toplevel_t *)widget)->closable == EI_TRUE)
        {
            ei_widget_t *button = ei_widget_create("button\0\0\0\0\0\0\0\0\0\0\0\0\0", widget, NULL, NULL);
            ei_button_configure(button, NULL, &close_button_color, &close_button_border_width, &close_button_corner_radius, &close_button_relief, &close_button_text, NULL, NULL, NULL, NULL, NULL, NULL, &close_toplevel_widget, NULL);
            ei_place(button, &close_button_anchor, &close_button_x, &close_button_y, &close_button_width, &close_button_height, &close_button_rel_x, &close_button_rel_y, NULL, NULL);
        }
        ei_bind(ei_ev_mouse_buttondown, widget, NULL, deplacement_callback, NULL);
        ei_bind(ei_ev_mouse_buttonup, widget, NULL, fin_deplacement_callback, NULL);
        ei_bind(ei_ev_mouse_move, widget, NULL, deplacement_actif_callback, NULL);
    }
    if (widget->next_sibling != NULL)
        return init_toplevel(widget->next_sibling);
    if (widget->children_head != NULL)
        return init_toplevel(widget->children_head);
}

void update_surface(ei_linked_rect_t *rectangles_list, ei_bool_t ponctuel)
{

    hw_surface_lock(ei_app_root_surface());
    draw_widgets_and_family(ei_app_root_widget());
    hw_surface_unlock(ei_app_root_surface());
    hw_surface_update_rects(ei_app_root_surface(), NULL);
}

void lighten_color(ei_color_t *couleur)
{
    couleur->red = (couleur->red <= 205) ? couleur->red + 50 : 255;
    couleur->green = (couleur->green <= 205) ? couleur->green + 50 : 255;
    couleur->blue = (couleur->blue <= 205) ? couleur->blue + 50 : 255;
}

void darken_color(ei_color_t *couleur)
{
    couleur->red = (couleur->red >= 50) ? couleur->red - 50 : 0;
    couleur->green = (couleur->green >= 50) ? couleur->green - 50 : 0;
    couleur->blue = (couleur->blue >= 50) ? couleur->blue - 50 : 0;
}

ei_point_t compute_location(ei_widget_t *widget, ei_anchor_t *ancre, ei_bool_t about_text)
{

    int largeur_contenu;
    int hauteur_contenu;

    if (about_text == EI_TRUE)
    {
        ei_surface_t text_surface;
        if (!strcmp(widget->wclass->name, "frame") == 1)
        {
            text_surface = hw_text_create_surface(*((ei_frame_t *)widget)->text, *((ei_frame_t *)widget)->text_font, *((ei_frame_t *)widget)->text_color);
        }
        else
        {
            text_surface = hw_text_create_surface(*((ei_button_t *)widget)->text, *((ei_button_t *)widget)->text_font, *((ei_button_t *)widget)->text_color);
        }
        largeur_contenu = hw_surface_get_size(text_surface).width;
        hauteur_contenu = hw_surface_get_size(text_surface).height;
        hw_surface_free(text_surface);
    }
    else
    {
        if (!strcmp(widget->wclass->name, "frame") == 1)
        {
            largeur_contenu = hw_surface_get_size(((ei_frame_t *)widget)->img).width;
            hauteur_contenu = hw_surface_get_size(((ei_frame_t *)widget)->img).height;
        }
        else
        {
            largeur_contenu = hw_surface_get_size(((ei_button_t *)widget)->img).width;
            hauteur_contenu = hw_surface_get_size(((ei_button_t *)widget)->img).height;
        }
    }

    ei_point_t point = widget->content_rect->top_left;
    int largeur_parent = widget->content_rect->size.width;
    int hauteur_parent = widget->content_rect->size.height;

    if (ancre == NULL)
    {
        point.x += (largeur_parent - largeur_contenu) / 2;
        point.y += (hauteur_parent - hauteur_contenu) / 2;
    }
    else
    {
        switch (*ancre)
        {
        case ei_anc_none:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_northwest:
            break;
        case ei_anc_north:
            point.x += (largeur_parent - largeur_contenu) / 2;
            break;
        case ei_anc_northeast:
            point.x += (largeur_parent - largeur_contenu);
            break;
        case ei_anc_west:
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_center:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_east:
            point.x += (largeur_parent - largeur_contenu);
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_southwest:
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        case ei_anc_south:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        case ei_anc_southeast:
            point.x += (largeur_parent - largeur_contenu);
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        }
    }
    return point;
}

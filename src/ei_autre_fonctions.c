#include "ei_autre_fonctions.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"
#include "ei_autre_struct.h"

extern ei_surface_t racine_surface;
extern ei_surface_t pick_surface;

void draw_widgets_and_family(ei_widget_t *widget)
{
    (widget->parent == NULL) ? widget->wclass->drawfunc(widget, racine_surface, pick_surface, NULL) : widget->wclass->drawfunc(widget, racine_surface, pick_surface, &(widget->screen_location));
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
    uint8_t red, green, blue;
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
    return (ei_color_t){red, green, blue, 0x00};
}

ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id)
{
    ei_widget_t *current_widget = widget;
    if (current_widget->pick_id == id)
        return current_widget;
    if (current_widget->next_sibling != NULL)
        return search_widget_by_id(current_widget->next_sibling, id);
    if (current_widget->children_head != NULL)
        return search_widget_by_id(current_widget->children_head, id);
}

ei_bool_t close_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param)
{
    ei_widget_destroy(widget->parent);
    return EI_TRUE;
}

ei_callback_t close_toplevel_widget = close_toplevel;

void create_close_button_for_each_toplevel(ei_widget_t *widget)
{
    if (!strcmp(widget->wclass->name, "toplevel") && *((ei_toplevel_t *)widget)->closable == EI_TRUE)
    {
        ei_widget_t *button = ei_widget_create("button", widget, NULL, NULL);
        ei_button_configure(button, NULL, &close_button_color, &close_button_border_width, &close_button_corner_radius, NULL, &close_button_text, NULL, NULL, NULL, NULL, NULL, NULL, &close_toplevel_widget, NULL);
        ei_place(button, &close_button_anchor, NULL, NULL, &close_button_width, &close_button_height, &close_button_rel_x, &close_button_rel_y, NULL, NULL);
    }
    if (widget->next_sibling != NULL)
        return create_close_button_for_each_toplevel(widget->next_sibling);
    if (widget->children_head != NULL)
        return create_close_button_for_each_toplevel(widget->children_head);
}

void update_surface(ei_linked_rect_t *rectangles_list)
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

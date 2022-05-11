#include "ei_autre_fonctions.h"
#include "hw_interface.h"
#include "ei_application.h"

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

void free_widgets_and_family(ei_widget_t *widget)
{
    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        free_widgets_and_family(current_widget->next_sibling);

    if (current_widget->children_head != NULL)
        free_widgets_and_family(current_widget->children_head);

    (widget->destructor != NULL) ? widget->destructor(widget) : NULL;
    free(widget);
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
        printf("%s", fils->wclass->name);
        pere->children_tail->next_sibling = fils;
        pere->children_tail = fils;
    }
}

ei_color_t int_to_color(uint32_t entier)
{
    uint8_t red, green, blue;
    red = entier / (256*256); 
    green = (entier-blue) / 256; 
    blue = (entier-blue-green);
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

ei_widget_t *search_widget_by_click(ei_event_t *event)
{
    ei_point_t point = event->param.mouse.where;

    uint32_t *picking_color_entier = (uint32_t *)hw_surface_get_buffer(pick_surface);
    picking_color_entier += point.x + point.y * hw_surface_get_size(pick_surface).width;

    return search_widget_by_id(ei_app_root_widget(), *picking_color_entier);
}
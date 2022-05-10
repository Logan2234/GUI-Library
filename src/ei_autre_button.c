#include "ei_draw.h"
#include "ei_autre_struct.h"
#include "ei_autre_draw.h"
#include "ei_autre_fonctions.h"

extern widget_id;

struct ei_widget_t *button_allocfunc(void)
{
    return (ei_widget_t *)calloc(1, sizeof(ei_button_t));
}

void button_releasefunc(struct ei_widget_t *widget)
{
    free((ei_button_t *)widget);
}

void button_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_button_t *bouton = (ei_button_t *)widget;
    ei_rect_t *rectangle = &(widget->screen_location);

    ei_color_t color = {0x64, 0x64, 0x64, 0xff};
    ei_color_t color2 = {0xB4, 0xB4, 0xB4, 0xff};
    ei_color_t color3 = {0x8B, 0x8B, 0x8B, 0xff};

    /* On dessine d'abord les parties hautes et basses */
    ei_linked_point_t *partie_haute = ei_rounded_frame(rectangle, *(bouton->corner_radius), 1);
    ei_linked_point_t *partie_basse = ei_rounded_frame(rectangle, *(bouton->corner_radius), 2);

    ei_draw_polygon(surface, partie_haute, color2, clipper);
    ei_draw_polygon(surface, partie_basse, color, clipper);

    /* Puis on dessine un plus petit rounded rectangle par dessus */
    rectangle->top_left.x += *((ei_toplevel_t *)widget)->border_width;
    rectangle->top_left.y += *((ei_toplevel_t *)widget)->border_width;
    rectangle->size.width -= 2 * *((ei_toplevel_t *)widget)->border_width;
    rectangle->size.height -= 2 * *((ei_toplevel_t *)widget)->border_width;
    ei_linked_point_t *partie_milieu = ei_rounded_frame(rectangle, (int)(2 * (float)(*(bouton->corner_radius)) / 3), 0);

    ei_draw_polygon(surface, partie_milieu, color3, clipper);

    /* Gestion de l'affichage du text sur le bouton */
    ei_color_t text_color = *((ei_button_t *)widget)->text_color;
    char **text = ((ei_button_t *)widget)->text;
    ei_point_t point = widget->screen_location.top_left;
    ei_font_t font = (*((ei_button_t *)widget)->text_font != NULL) ? (ei_default_font) : *((ei_button_t *)widget)->text_font;
    ei_draw_text(surface, &point, *text, ei_default_font, text_color, NULL);

    free_linked_point_pointeur(partie_haute);
    free_linked_point_pointeur(partie_basse);
    free_linked_point_pointeur(partie_milieu);
}

void button_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

void button_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = 0;

    widget->pick_color = &(ei_color_t){0x00, 0x00, 0x00, 0x00};

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

ei_widgetclass_t *return_class_button(void)
{
    ei_widgetclass_t *widgetclass_button = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_button->name, "button");
    widgetclass_button->allocfunc = &button_allocfunc;
    widgetclass_button->releasefunc = &button_releasefunc;
    widgetclass_button->drawfunc = &button_drawfunc;
    widgetclass_button->setdefaultsfunc = &button_setdefaultsfunc;
    widgetclass_button->geomnotifyfunc = &button_geomnotifyfunc;
    widgetclass_button->next = NULL;

    return widgetclass_button;
}
#include "ei_draw.h"
#include "ei_autre_struct.h"
#include "ei_autre_draw.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"

extern int widget_id;

struct ei_widget_t *button_allocfunc(void)
{
    ei_button_t *widget_button = calloc(1, sizeof(ei_button_t));
    return (ei_widget_t *)widget_button;
}

void button_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free((ei_button_t *)widget);
}

uint8_t add_50_or_not(uint8_t entier){
    if (entier <= 205){
        return entier + 50;
    }
    return entier;
}

uint8_t remove_50_or_not(uint8_t entier){
    if (entier >= 50){
        return entier - 50;
    }
    return entier;
}

void button_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_button_t *bouton = (ei_button_t *)widget;
    ei_rect_t rectangle = widget->screen_location;

    ei_color_t color = *bouton->color;
    ei_color_t color2 = *bouton->color;
    ei_color_t color3 = *bouton->color;

    if (*bouton->relief == ei_relief_sunken)
    {
        color = (ei_color_t){add_50_or_not(color.red), add_50_or_not(color.green), add_50_or_not(color.blue), 0xff};
        color2 = (ei_color_t){remove_50_or_not(color.red), remove_50_or_not(color.green), remove_50_or_not(color.blue), 0xff};
    }
    else if (*bouton->relief == ei_relief_raised)
    {
        color = (ei_color_t){remove_50_or_not(color.red), remove_50_or_not(color.green), remove_50_or_not(color.blue), 0xff};
        color2 = (ei_color_t){add_50_or_not(color.red), add_50_or_not(color.green), add_50_or_not(color.blue), 0xff};
    }

    /* On dessine d'abord les parties hautes et basses */
    ei_linked_point_t *partie_haute = ei_rounded_frame(&rectangle, *bouton->corner_radius, 1);
    ei_linked_point_t *partie_basse = ei_rounded_frame(&rectangle, *bouton->corner_radius, 2);

    ei_draw_polygon(surface, partie_haute, color2, clipper);
    ei_draw_polygon(surface, partie_basse, color, clipper);

    /* Puis on dessine un plus petit rounded rectangle par dessus */
    rectangle.top_left.x += *((ei_toplevel_t *)widget)->border_width;
    rectangle.top_left.y += *((ei_toplevel_t *)widget)->border_width;
    rectangle.size.width -= 2 * *((ei_toplevel_t *)widget)->border_width;
    rectangle.size.height -= 2 * *((ei_toplevel_t *)widget)->border_width;
    ei_linked_point_t *partie_milieu = ei_rounded_frame(&rectangle, (int)(2 * (float)(*bouton->corner_radius) / 3), 0);

    ei_draw_polygon(surface, partie_milieu, color3, clipper);

    /* Gestion de l'affichage du text sur le bouton */
    ei_color_t text_color = *bouton->text_color;
    char **text = bouton->text;
    ei_font_t font = (*bouton->text_font != NULL) ? (ei_default_font) : *bouton->text_font;
    int largeur_texte = hw_surface_get_size(hw_text_create_surface(*text, font, color)).width;
    int hauteur_texte = hw_surface_get_size(hw_text_create_surface(*text, font, color)).height;
    int largeur_boutton = widget->screen_location.size.width;
    int hauteur_boutton = widget->screen_location.size.height;
    ei_point_t point = widget->screen_location.top_left;
    ei_anchor_t *anchor = bouton->text_anchor;
    if (anchor == NULL)
    {
        point.x += (largeur_boutton - largeur_texte) / 2;
        point.y += (hauteur_boutton - hauteur_texte) / 2;
    }
    else
    {
        switch (*anchor)
        {
        case ei_anc_none:
            point.x += (largeur_boutton - largeur_texte) / 2;
            point.y += (hauteur_boutton - hauteur_texte) / 2;
            break;
        case ei_anc_northwest:
            break;
        case ei_anc_north:
            point.x += (largeur_boutton - largeur_texte) / 2;
            break;
        case ei_anc_northeast:
            point.x += (largeur_boutton - largeur_texte);
            break;
        case ei_anc_west:
            point.y += (hauteur_boutton - hauteur_texte) / 2;
            break;
        case ei_anc_center:
            point.x += (largeur_boutton - largeur_texte) / 2;
            point.y += (hauteur_boutton - hauteur_texte) / 2;
            break;
        case ei_anc_east:
            point.x += (largeur_boutton - largeur_texte);
            point.y += (hauteur_boutton - hauteur_texte) / 2;
            break;
        case ei_anc_southwest:
            point.y += (hauteur_boutton - hauteur_texte);
            break;
        case ei_anc_south:
            point.x += (largeur_boutton - largeur_texte) / 2;
            point.y += (hauteur_boutton - hauteur_texte);
            break;
        case ei_anc_southeast:
            point.x += (largeur_boutton - largeur_texte);
            point.y += (hauteur_boutton - hauteur_texte);
            break;
        }
    }
    ei_draw_text(surface, &point, *text, ei_default_font, text_color, NULL);

    free_linked_point_pointeur(partie_haute);
    free_linked_point_pointeur(partie_basse);
    free_linked_point_pointeur(partie_milieu);

    ei_fill(pick_surface, widget->pick_color, clipper);
}

void button_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

void button_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    widget_id++;

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
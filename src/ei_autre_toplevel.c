#include "ei_autre_struct.h"
#include "ei_application.h"
#include "ei_draw.h"
#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_autre_fonctions.h"

extern int widget_id;

struct ei_widget_t *toplevel_allocfunc(void)
{
    ei_toplevel_t *widget_toplevel = calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)widget_toplevel;
}

void toplevel_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free((ei_toplevel_t *)widget);
}

void toplevel_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_linked_point_t *premier_point = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel = premier_point;
    premier_point->point.x = 0;
    premier_point->point.y = 0;

    ei_linked_point_t *nouveau = calloc(1, sizeof(ei_linked_point_t));
    nouveau->point.x = widget->screen_location.size.width;
    nouveau->point.y = 0;
    sentinel->next = nouveau;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau1 = calloc(1, sizeof(ei_linked_point_t));
    nouveau1->point.x = widget->screen_location.size.width;
    nouveau1->point.y = widget->screen_location.size.height;
    sentinel->next = nouveau1;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau2->point.x = 0;
    nouveau2->point.y = widget->screen_location.size.height;
    sentinel->next = nouveau1;

    ei_draw_polygon(surface, premier_point, *(((ei_toplevel_t *) widget)->color), clipper);

    // JE VAIS METTRE ICI LA CRÉATION DU BUTTON MAIS JE NE SAIS PAS S'IL DEVRAIT ALLER LÀ


    ei_widget_t*	button;
    int taille_bouton = widget->screen_location.size.height / 2;
    ei_size_t	button_size		= {taille_bouton,taille_bouton};
    int		button_x		= taille_bouton / 2;
    int		button_y		= taille_bouton / 2;
    char*		button_title		= "Close";
    ei_color_t	button_color		= {0xFA, 0x20, 0x20, 0xff};
    ei_relief_t	button_relief		= ei_relief_raised;
    int		button_border_width	= 2;
    int		button_corner_radius	= taille_bouton / 2;
    button = ei_widget_create("button", widget, NULL, NULL);
    ei_button_configure	(button, &button_size, &button_color,
                            &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, NULL, NULL,
                            NULL, NULL, NULL, NULL, NULL);
    ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );



    /*
    // Il faut encore faire le callback du bouton et l'écriture
    // TODO
    ei_fill(surface, ((ei_toplevel_t *)widget)->color, clipper);
    ei_fill(pick_surface, widget->pick_color, clipper);
     */
}

void toplevel_setdefaultsfunc(struct ei_widget_t *widget)
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

    widget->requested_size = (ei_size_t){320, 240};
    widget->screen_location = (ei_rect_t){0, 0, widget->requested_size};
    widget->content_rect = NULL;
}

void toplevel_geomnotifyfunc(struct ei_widget_t *widget)
{
    // TODO
}

ei_widgetclass_t *return_class_toplevel(void)
{
    ei_widgetclass_t *widgetclass_toplevel = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_toplevel->name, "toplevel");
    widgetclass_toplevel->allocfunc = &toplevel_allocfunc;
    widgetclass_toplevel->releasefunc = &toplevel_releasefunc;
    widgetclass_toplevel->drawfunc = &toplevel_drawfunc;
    widgetclass_toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
    widgetclass_toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
    widgetclass_toplevel->next = NULL;

    return widgetclass_toplevel;
}

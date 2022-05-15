#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"
#include "ei_autre_draw.h"
#include "ei_application.h"

extern int widget_id;
extern ei_surface_t pick_surface;
extern ei_bool_t deplacement;
extern ei_point_t origine_deplacement;


struct ei_widget_t *toplevel_allocfunc(void)
{
    ei_toplevel_t *widget_toplevel = calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)widget_toplevel;
}

void toplevel_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free(widget->geom_params);
    free((ei_toplevel_t *)widget);
}

void toplevel_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    /* On trace le fond */
    ei_linked_point_t *premier_point = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel = premier_point;
    premier_point->point.x = widget->screen_location.top_left.x;
    premier_point->point.y = widget->screen_location.top_left.y + 35;

    ei_linked_point_t *nouveau = calloc(1, sizeof(ei_linked_point_t));
    nouveau->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau->point.y = widget->screen_location.top_left.y + 35;
    sentinel->next = nouveau;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau1 = calloc(1, sizeof(ei_linked_point_t));
    nouveau1->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau1->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sentinel->next = nouveau1;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau2->point.x = widget->screen_location.top_left.x;
    nouveau2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sentinel->next = nouveau2;

    ei_rect_t new_clipper_toplevel = *clipper;

    new_clipper_toplevel.top_left.y += 35;
    new_clipper_toplevel.size.height -= 35;

    ei_draw_polygon(surface, premier_point, *toplevel->color, &new_clipper_toplevel);

    free_linked_point_pointeur(premier_point);

    /* On trace les bordures */
    ei_linked_point_t *contour = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sent = contour;

    contour->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    contour->point.y = widget->screen_location.top_left.y + 35;

    ei_linked_point_t *nouveau_contour = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    nouveau_contour->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width;
    sent->next = nouveau_contour;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour2->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
    nouveau_contour2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width;
    sent->next = nouveau_contour2;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour3 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour3->point.x = widget->screen_location.top_left.x + widget->screen_location.size.height + *toplevel->border_width;
    nouveau_contour3->point.y = widget->screen_location.top_left.y + 35;
    sent->next = nouveau_contour3;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour4 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour4->point.x = widget->screen_location.top_left.x + widget->screen_location.size.height;
    nouveau_contour4->point.y = widget->screen_location.top_left.y + 35;
    sent->next = nouveau_contour4;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour5 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour5->point.x = widget->screen_location.top_left.x + widget->screen_location.size.height;
    nouveau_contour5->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sent->next = nouveau_contour5;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour6 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour6->point.x = widget->screen_location.top_left.x;
    nouveau_contour6->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height;
    sent->next = nouveau_contour6;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour7 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour7->point.x = widget->screen_location.top_left.x;
    nouveau_contour7->point.y = widget->screen_location.top_left.y + 35;
    sent->next = nouveau_contour7;

    ei_draw_polygon(surface, contour, (ei_color_t){0x63, 0x69, 0x70, 0xff}, NULL);

    free_linked_point_pointeur(contour);

    /* On trace le header */
    ei_linked_point_t *header = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel2 = header;
    header->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    header->point.y = widget->screen_location.top_left.y + 35;

    ei_linked_point_t *nouveau_header = calloc(1, sizeof(ei_linked_point_t));
    nouveau_header->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
    nouveau_header->point.y = widget->screen_location.top_left.y + 35;
    sentinel2->next = nouveau_header;
    sentinel2 = sentinel2->next;

    ei_point_t centre_droit = {widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width - 15, widget->screen_location.top_left.y + 15};
    ei_linked_point_t *nouveau_header2 = ei_draw_arc(centre_droit, 15, 0, 1.57075);
    sentinel2->next = nouveau_header2;
    while (sentinel2->next != NULL)
        sentinel2 = sentinel2->next;

    ei_point_t centre_gauche = {widget->screen_location.top_left.x - *toplevel->border_width + 15, widget->screen_location.top_left.y + 15};
    ei_linked_point_t *nouveau_header3 = ei_draw_arc(centre_gauche, 15, 1.57075, 3.1415);
    sentinel2->next = nouveau_header3;

    ei_draw_polygon(surface, header, (ei_color_t){0x63, 0x69, 0x70, 0xff}, NULL);

    free_linked_point_pointeur(header);

    /* On trace un petit triangle qui indique qu'on peut resize la fenêtre */
    if (*toplevel->resizable != ei_axis_none)
    {
        ei_linked_point_t *resize_indicator = calloc(1, sizeof(ei_linked_point_t));
        ei_linked_point_t *sentinel3 = resize_indicator;
        resize_indicator->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
        resize_indicator->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height - 12;

        ei_linked_point_t *resize_indicator2 = calloc(1, sizeof(ei_linked_point_t));
        resize_indicator2->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
        resize_indicator2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width;
        sentinel3->next = resize_indicator2;
        sentinel3 = sentinel3->next;

        ei_linked_point_t *resize_indicator3 = calloc(1, sizeof(ei_linked_point_t));
        resize_indicator3->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width - 12;
        resize_indicator3->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width;
        sentinel3->next = resize_indicator3;

        ei_draw_polygon(surface, resize_indicator, (ei_color_t){0x63, 0x69, 0x70, 0xff}, NULL);

        free_linked_point_pointeur(resize_indicator);
    }

    /* Et enfin, on trace le titre */
    ei_point_t titre_pos = widget->screen_location.top_left;
    titre_pos.x += 35;
    titre_pos.y += 4;
    ei_draw_text(surface, &titre_pos, *toplevel->title, ei_default_font, (ei_color_t){0xFF, 0xFF, 0xFF, 0xFF}, NULL);

    ei_rect_t new_clipper_including_header = {(ei_point_t){widget->screen_location.top_left.x - *toplevel->border_width, widget->screen_location.top_left.y}, (ei_size_t){widget->requested_size.width + *toplevel->border_width * 2, widget->requested_size.height + *toplevel->border_width}};
    ei_fill(pick_surface, widget->pick_color, &new_clipper_including_header);
}

void toplevel_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    widget->user_data = NULL;
    widget->destructor = NULL; /* Il faut créer la fonction */
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;

    widget->requested_size = default_toplevel_size;
    widget->screen_location = (ei_rect_t){0, 0, default_toplevel_size};
    widget->content_rect = &widget->screen_location;
}

void toplevel_geomnotifyfunc(struct ei_widget_t *widget)
{
    widget->geom_params->manager->runfunc(widget);
    ei_widget_t *sent = widget->children_head;
    while (sent != NULL)
    {
        sent->wclass->geomnotifyfunc(sent);
        sent = sent->next_sibling;
    }
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

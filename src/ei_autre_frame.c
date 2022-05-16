#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"

extern int widget_id;
extern ei_surface_t racine_surface;

struct ei_widget_t *frame_allocfunc(void)
{
    ei_frame_t *widget_frame = calloc(1, sizeof(ei_frame_t));
    return (ei_widget_t *)widget_frame;
}

void frame_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free(widget->content_rect);
    free(widget->geom_params);
    free(((ei_frame_t *)widget)->border_width);
    free(((ei_frame_t *)widget)->color);
    free(((ei_frame_t *)widget)->img);
    free(((ei_frame_t *)widget)->img_anchor);
    free(((ei_frame_t *)widget)->relief);
    free(((ei_frame_t *)widget)->text_anchor);
    free(((ei_frame_t *)widget)->text_color);
    free(((ei_frame_t *)widget)->text_font);
    free((ei_frame_t *)widget);
}

void frame_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    int h = widget->requested_size.height / 2;

    if (*frame->relief != ei_relief_none && *frame->border_width != 0)
    {
        ei_linked_point_t *zone_rectangle = calloc(1, sizeof(ei_linked_point_t));
        zone_rectangle->next = calloc(1, sizeof(ei_linked_point_t));
        zone_rectangle->next->next = calloc(1, sizeof(ei_linked_point_t));
        zone_rectangle->next->next->next = calloc(1, sizeof(ei_linked_point_t));
        zone_rectangle->next->next->next->next = calloc(1, sizeof(ei_linked_point_t));

        /* D'abord on ajoute le point supérieur gauche */
        zone_rectangle->point = widget->screen_location.top_left;

        /* Puis le coin supérieur droit */
        zone_rectangle->next->point = (ei_point_t){widget->screen_location.top_left.x + widget->requested_size.width, widget->screen_location.top_left.y};

        /* Ajout des points intermédiaires */
        zone_rectangle->next->next->point = (ei_point_t){widget->screen_location.top_left.x + widget->requested_size.width - h, widget->screen_location.top_left.y + h};
        zone_rectangle->next->next->next->point = (ei_point_t){widget->screen_location.top_left.x + h, widget->screen_location.top_left.y + h};

        /* Et enfin le coin inférieur gauche pour créer la partie supérieure */
        zone_rectangle->next->next->next->next->point = (ei_point_t){widget->screen_location.top_left.x, widget->screen_location.top_left.y + widget->requested_size.height};

        ei_color_t color1 = *frame->color;
        ei_color_t color2 = *frame->color;
        lighten_color(&color1);
        darken_color(&color2);

        if (*frame->relief == ei_relief_raised)
        {
            ei_draw_polygon(surface, zone_rectangle, color1, NULL);

            /* Le premier point devient le point en bas à droite pour dessiner la partie inférieure */
            zone_rectangle->point = (ei_point_t){widget->screen_location.top_left.x + widget->requested_size.width, widget->screen_location.top_left.y + widget->requested_size.height};

            ei_draw_polygon(surface, zone_rectangle, color2, NULL);
        }
        else
        {
            ei_draw_polygon(surface, zone_rectangle, color1, NULL);

            /* Le premier point devient le point en bas à droite pour dessiner la partie inférieure */
            zone_rectangle->point = (ei_point_t){widget->screen_location.top_left.x + widget->requested_size.width, widget->screen_location.top_left.y + widget->requested_size.height};

            ei_draw_polygon(surface, zone_rectangle, color2, NULL);
        }
        free(zone_rectangle->next->next->next->next);
        free(zone_rectangle->next->next->next);
        free(zone_rectangle->next->next);
        free(zone_rectangle->next);
        free(zone_rectangle);
    }
    ei_fill(surface, frame->color, clipper);

    /* Dessin du texte si nécessaire */
    if (frame->text != NULL)
    {
        ei_point_t where = compute_location(widget, frame->text_anchor);
        if (*frame->relief != ei_relief_none && *frame->border_width != 0)
            ei_draw_text(surface, &where, *frame->text, *frame->text_font, *frame->text_color, widget->content_rect);
        else
            ei_draw_text(surface, &where, *frame->text, *frame->text_font, *frame->text_color, clipper);
    }

    /* Dessin de l'image si nécessaire */
    if (frame->img != NULL && frame->text == NULL)
    {
        // ei_point_t where = compute_location(widget, frame->img_anchor);
        // hw_surface_set_origin(*frame->img, where);
        (frame->img_rect != NULL) ? ei_copy_surface(surface, clipper, frame->img, *frame->img_rect, EI_FALSE)
                                  : ei_copy_surface(surface, clipper, frame->img, NULL, EI_FALSE);
    }

    /* Dessin de la surface offscreen de picking */
    ei_fill(pick_surface, widget->pick_color, &widget->screen_location);
}

void frame_setdefaultsfunc(struct ei_widget_t *widget)
{
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    widget->user_data = NULL;
    widget->destructor = NULL;
    widget->parent = NULL;
    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->next_sibling = NULL;
    widget->geom_params = NULL;

    /* Si c'est la racine */
    if (widget_id == 1)
    {
        widget->requested_size = hw_surface_get_size(racine_surface);
        widget->screen_location = hw_surface_get_rect(racine_surface);
    }

    else
    {
        widget->requested_size = default_frame_size;
        widget->screen_location = (ei_rect_t){0, 0, default_frame_size};
    }

    ei_rect_t *content_rect_frame = calloc(1, sizeof(ei_rect_t));
    *content_rect_frame = widget->screen_location;
    widget->content_rect = content_rect_frame;
}

void frame_geomnotifyfunc(struct ei_widget_t *widget)
{
    widget->geom_params->manager->runfunc(widget);
}

ei_widgetclass_t *return_class_frame(void)
{
    ei_widgetclass_t *widgetclass_frame = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_frame->name, "frame");
    widgetclass_frame->allocfunc = &frame_allocfunc;
    widgetclass_frame->releasefunc = &frame_releasefunc;
    widgetclass_frame->drawfunc = &frame_drawfunc;
    widgetclass_frame->setdefaultsfunc = &frame_setdefaultsfunc;
    widgetclass_frame->geomnotifyfunc = &frame_geomnotifyfunc;
    widgetclass_frame->next = NULL;

    return widgetclass_frame;
}

#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"

extern int widget_id;

ei_widget_t *frame_allocfunc(void)
{
    ei_frame_t *widget_frame = calloc(1, sizeof(ei_frame_t));
    return (ei_widget_t *)widget_frame;
}

static void frame_releasefunc(ei_widget_t *widget)
{
    free(widget->pick_color);
    free(widget->content_rect);
    free(widget->geom_params);
    free(((ei_frame_t *)widget)->border_width);
    free((ei_color_t *)((ei_frame_t *)widget)->color);
    // free(*((ei_frame_t *)widget)->img_rect);
    free(((ei_frame_t *)widget)->img_rect);
    free(((ei_frame_t *)widget)->img_anchor);
    free(((ei_frame_t *)widget)->relief);
    // free(*((ei_frame_t *)widget)->text);
    free(((ei_frame_t *)widget)->text);
    free(((ei_frame_t *)widget)->text_anchor);
    free(((ei_frame_t *)widget)->text_color);
    free(((ei_frame_t *)widget)->text_font);
    free((ei_frame_t *)widget);
}

static void frame_drawfunc(ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_frame_t *frame = (ei_frame_t *)widget;

    if (*frame->relief != ei_relief_none && *frame->border_width != 0)
    {
        ei_linked_point_t *zone_rectangle = calloc(1, sizeof(ei_linked_point_t));
        int h = widget->requested_size.height / 2;

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

    ei_bool_t has_been_resized = EI_FALSE;

    /* Dessin du texte si nécessaire */
    if (*frame->text != NULL)
    {
        ei_surface_t surface_text = hw_text_create_surface(*frame->text, *frame->text_font, *frame->text_color);
        ei_size_t taille_frame = hw_surface_get_size(surface_text);

        free(surface_text);

        if (widget->screen_location.size.height < taille_frame.height)
        {
            ((ei_placer_t *)widget->geom_params)->height = taille_frame.height + *frame->border_width * 2;
            widget->requested_size.height = taille_frame.height + *frame->border_width * 2;
            has_been_resized = EI_TRUE;
        }

        if (widget->screen_location.size.width < taille_frame.width)
        {
            ((ei_placer_t *)widget->geom_params)->width = taille_frame.width + *frame->border_width * 2;
            widget->requested_size.width = taille_frame.width + *frame->border_width * 2;
            has_been_resized = EI_TRUE;
        }

        if (has_been_resized)
        {
            widget->geom_params->manager->runfunc(widget);
            frame_drawfunc(widget, surface, pick_surface, clipper);
        }

        ei_point_t where = compute_location(widget, frame->text_anchor, EI_TRUE);
        ei_draw_text(surface, &where, *frame->text, *frame->text_font, *frame->text_color, clipper);
    }

    /* Dessin de l'image si nécessaire */
    else if (frame->img != NULL && *frame->text == NULL)
    {
        /* La frame prend la taille de l'image rect si celui-ci existe et est plus grand */
        if (*frame->img_rect != NULL)
        {
            if (widget->screen_location.size.height < (*frame->img_rect)->size.height)
            {
                ((ei_placer_t *)widget->geom_params)->height = (*frame->img_rect)->size.height;
                widget->requested_size.height = (*frame->img_rect)->size.height;
                has_been_resized = EI_TRUE;
            }

            if (widget->screen_location.size.width < (*frame->img_rect)->size.width)
            {
                ((ei_placer_t *)widget->geom_params)->width = (*frame->img_rect)->size.width;
                widget->requested_size.width = (*frame->img_rect)->size.width;
                has_been_resized = EI_TRUE;
            }
        }
        /* Sinon on considère simplement la taille de l'image */
        else
        {
            ei_size_t taille_frame = hw_surface_get_size(*frame->img);
            if (widget->screen_location.size.height < taille_frame.height)
            {
                ((ei_placer_t *)widget->geom_params)->height = taille_frame.height;
                widget->requested_size.height = taille_frame.height;
                has_been_resized = EI_TRUE;
            }

            if (widget->screen_location.size.width < taille_frame.width)
            {
                ((ei_placer_t *)widget->geom_params)->width = taille_frame.width;
                widget->requested_size.width = taille_frame.width;
                has_been_resized = EI_TRUE;
            }
        }

        if (has_been_resized)
        {
            widget->geom_params->manager->runfunc(widget);
            frame_drawfunc(widget, surface, pick_surface, clipper);
        }

        // TODO Gestion de l'ancre !!!
        // ei_point_t where = compute_location(widget, frame->img_anchor, EI_FALSE);
        // hw_surface_set_origin(frame->img, where);
        (*frame->img_rect != NULL) ? ei_copy_surface(surface, widget->content_rect, *frame->img, *frame->img_rect, EI_FALSE)
                                   : ei_copy_surface(surface, widget->content_rect, *frame->img, NULL, EI_FALSE);
    }

    /* Dessin de la surface offscreen de picking */
    ei_fill(pick_surface, widget->pick_color, &widget->screen_location);
}

static void frame_setdefaultsfunc(ei_widget_t *widget)
{
    /* Gestion du pick_id et de la couleur associée au pick_id */
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    /* Si c'est la racine dans ce cas la requested size, le screen location deviennent l'écran tout entier */
    if (widget_id == 1)
    {
        ei_surface_t racine_surface = ei_app_root_surface();
        widget->requested_size = hw_surface_get_size(racine_surface);
        widget->screen_location = hw_surface_get_rect(racine_surface);
    }

    /* Gestion du content_rect, si ce n'est pas la racine, prend la valeur widget->screen_location = {0, 0, 0, 0} */
    ei_rect_t *content_rect = calloc(1, sizeof(ei_rect_t));
    *content_rect = widget->screen_location;
    widget->content_rect = content_rect;

    /* Et enfin, on lui donne une configuration de base */
    ei_frame_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

static void frame_geomnotifyfunc(ei_widget_t *widget)
{
    if (widget->pick_id != 1)
        widget->geom_params->manager->runfunc(widget);

    ei_widget_t *sent = widget->children_head;
    while (sent != NULL)
    {
        sent->wclass->geomnotifyfunc(sent);
        sent = sent->next_sibling;
    }
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

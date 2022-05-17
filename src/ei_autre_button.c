#include "ei_autre_struct.h"
#include "ei_autre_draw.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"
#include "ei_autre_placer.h"

extern int widget_id;
extern rect_to_update;

struct ei_widget_t *button_allocfunc(void)
{
    ei_button_t *widget_button = calloc(1, sizeof(ei_button_t));
    return (ei_widget_t *)widget_button;
}

void button_releasefunc(struct ei_widget_t *widget)
{
    free(widget->pick_color);
    free(widget->geom_params);
    free(widget->content_rect);
    free((ei_color_t *)((ei_button_t *)widget)->color);
    free(((ei_button_t *)widget)->border_width);
    free(((ei_button_t *)widget)->corner_radius);
    free(((ei_button_t *)widget)->relief);
    // free(*((ei_button_t *)widget)->text);
    free(((ei_button_t *)widget)->text);
    free(((ei_button_t *)widget)->text_font);
    free(((ei_button_t *)widget)->text_color);
    free(((ei_button_t *)widget)->text_anchor);
    // free(*((ei_button_t *)widget)->img_rect);
    free(((ei_button_t *)widget)->img_rect);
    free(((ei_button_t *)widget)->img_anchor);
    // free(*((ei_button_t *)widget)->user_param);
    free(((ei_button_t *)widget)->user_param);
    free(((ei_button_t *)widget));
}

void button_drawfunc(struct ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_button_t *bouton = (ei_button_t *)widget;

    ei_color_t color = *bouton->color;
    ei_color_t color2 = *bouton->color;
    ei_color_t color3 = *bouton->color;

    if (*bouton->relief == ei_relief_sunken)
    {
        lighten_color(&color);
        darken_color(&color2);
    }
    else if (*bouton->relief == ei_relief_raised)
    {
        darken_color(&color);
        lighten_color(&color2);
    }

    /* On dessine d'abord les parties hautes et basses */
    ei_linked_point_t *partie_haute = ei_rounded_frame(&widget->screen_location, *bouton->corner_radius, 1);
    ei_linked_point_t *partie_basse = ei_rounded_frame(&widget->screen_location, *bouton->corner_radius, 2);

    ei_rect_t new_clipper = *clipper;
    if (*bouton->text != NULL && !strcmp(*bouton->text, " "))
    {
        new_clipper = *widget->parent->parent->content_rect;
        ei_draw_polygon(surface, partie_haute, color2, &new_clipper);
        ei_draw_polygon(surface, partie_basse, color, &new_clipper);
    }
    else
    {
        ei_draw_polygon(surface, partie_haute, color2, &widget->screen_location);
        ei_draw_polygon(surface, partie_basse, color, &widget->screen_location);
    }

    /* Puis on dessine un plus petit rounded rectangle par dessus */
    ei_rect_t rectangle = widget->screen_location;
    rectangle.top_left.x += *bouton->border_width;
    rectangle.top_left.y += *bouton->border_width;
    rectangle.size.width -= 2 * *bouton->border_width;
    rectangle.size.height -= 2 * *bouton->border_width;
    ei_linked_point_t *partie_milieu = ei_rounded_frame(&rectangle, (int)(2 * (float)(*bouton->corner_radius) / 3), 0);

    ei_draw_polygon(surface, partie_milieu, color3, &new_clipper);

    /* Dessin du texte si nécessaire */
    if (*bouton->text != NULL && strcmp(*bouton->text, " "))
    {
        ei_surface_t surface_text = hw_text_create_surface(*bouton->text, *bouton->text_font, *bouton->text_color);
        ei_size_t taille_bouton = hw_surface_get_size(surface_text);
        free(surface_text);

        if (widget->screen_location.size.height <= taille_bouton.height)
        {
            ((ei_placer_t *)widget->geom_params)->height = taille_bouton.height + *bouton->border_width * 2;
            widget->requested_size.height = taille_bouton.height + *bouton->border_width * 2;
        }
        if (widget->screen_location.size.width <= taille_bouton.width)
        {
            ((ei_placer_t *)widget->geom_params)->width = taille_bouton.width + *bouton->border_width * 2;
            widget->requested_size.width = taille_bouton.width + *bouton->border_width * 2;
        }
        widget->geom_params->manager->runfunc(widget);

        ei_point_t where = compute_location(widget, bouton->text_anchor, EI_TRUE);
        ei_draw_text(surface, &where, *bouton->text, *bouton->text_font, *bouton->text_color, clipper);
    }

    /* Dessin de l'image si nécessaire */
    else if (bouton->img != NULL && *bouton->text == NULL)
    {
        /* Le bouton prend la taille de l'image rect si celui-ci existe et est plus grand */
        if (*bouton->img_rect != NULL)
        {
            if (widget->screen_location.size.height <= (*bouton->img_rect)->size.height)
            {
                ((ei_placer_t *)widget->geom_params)->height = (*bouton->img_rect)->size.height;
                widget->requested_size.height = (*bouton->img_rect)->size.height;
            }

            if (widget->screen_location.size.width <= (*bouton->img_rect)->size.width)
            {
                ((ei_placer_t *)widget->geom_params)->width = (*bouton->img_rect)->size.width;
                widget->requested_size.width = (*bouton->img_rect)->size.width;
            }
        }
        /* Sinon on considère simplement la taille de l'image */
        else
        {
            ei_size_t taille_bouton = hw_surface_get_size(*bouton->img);
            if (widget->screen_location.size.height <= taille_bouton.height)
            {
                ((ei_placer_t *)widget->geom_params)->height = taille_bouton.height;
                widget->requested_size.height = taille_bouton.height;
            }

            if (widget->screen_location.size.width <= taille_bouton.width)
            {
                ((ei_placer_t *)widget->geom_params)->width = taille_bouton.width;
                widget->requested_size.width = taille_bouton.width;
            }
        }

        widget->geom_params->manager->runfunc(widget);
        // ei_point_t where = compute_location(widget, bouton->img_anchor, EI_FALSE);
        // hw_surface_set_origin(*bouton->img, (ei_point_t){100, 100});
        (*bouton->img_rect != NULL) ? ei_copy_surface(surface, widget->content_rect, *bouton->img, *bouton->img_rect, EI_FALSE)
                                    : ei_copy_surface(surface, widget->content_rect, *bouton->img, NULL, EI_FALSE);
    }

    free_linked_point_pointeur(partie_haute);
    free_linked_point_pointeur(partie_basse);
    free_linked_point_pointeur(partie_milieu);

    /* La pick_color n'est dessinée que si l'on reste dans le clipper bien sûr */
    if (widget->screen_location.top_left.x <= new_clipper.top_left.x + new_clipper.size.width &&
        widget->screen_location.top_left.x + widget->screen_location.size.width >= new_clipper.top_left.x &&
        widget->screen_location.top_left.y <= new_clipper.top_left.y + new_clipper.size.height &&
        widget->screen_location.top_left.y + widget->screen_location.size.height >= new_clipper.top_left.y)
        ei_fill(pick_surface, widget->pick_color, &widget->screen_location);
}

void button_geomnotifyfunc(struct ei_widget_t *widget)
{
    widget->geom_params->manager->runfunc(widget);
}

ei_bool_t relief_toggle(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    if (event->param.mouse.button == ei_mouse_button_left)
    {
        ei_button_t *bouton = ((ei_button_t *)widget);
        ei_widget_t *pointed_widget = ei_widget_pick(&event->param.mouse.where);

        /* S'il s'agit d'un mouvement du clic gauche, dans ce cas on cherche à savoir si on est ou pas sur le même bouton */
        if (event->param.mouse.button == ei_mouse_button_left && event->type == ei_ev_mouse_move && last_clicked_widget != NULL)
        {
            *((ei_button_t *)last_clicked_widget)->relief = (last_clicked_widget != pointed_widget) ? ei_relief_raised : ei_relief_sunken;
            update_surface(rect_to_update);
        }

        /* Si il s'agit d'une intéraction brève avec le bouton, on change son relief */
        else if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup)
        {
            *bouton->relief = (*bouton->relief == ei_relief_raised && event->type != ei_ev_mouse_buttonup) ? ei_relief_sunken : ei_relief_raised;

            if (event->type == ei_ev_mouse_buttondown)
                last_clicked_widget = widget;

            update_surface(rect_to_update);
        }

        /* Si on relâche le bouton, on appelle le callback */
        else if (event->type == ei_ev_mouse_buttonup && last_clicked_widget == widget)
        {
            (bouton->callback != NULL) ? (*bouton->callback)(widget, event, *bouton->user_param) : 0;
            last_clicked_widget = NULL;
        }
    }

    return EI_FALSE;
}

void button_setdefaultsfunc(struct ei_widget_t *widget)
{
    /* Gestion du pick_id et de la couleur associée au pick_id */
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    /* Nécessité d'avoir un calloc pour le content_rect ? */
    ei_rect_t *content_rect = calloc(1, sizeof(ei_rect_t));
    *content_rect = widget->screen_location;
    widget->content_rect = content_rect;

    /* Et enfin, on lui donne une configuration de base */
    ei_button_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    ei_callback_t shrink = relief_toggle;
    ei_bind(ei_ev_mouse_buttondown, widget, NULL, shrink, NULL);
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, shrink, NULL);
    ei_bind(ei_ev_mouse_move, widget, NULL, shrink, NULL);
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
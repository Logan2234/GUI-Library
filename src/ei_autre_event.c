#include "ei_autre_event.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"
#include "ei_autre_placer.h"

extern ei_bool_t is_moving;
extern ei_bool_t is_resizing;
extern ei_bool_t arret_final;
extern ei_point_t origine_deplacement;
extern ei_linked_rect_t *rect_to_update;
static ei_widget_t *last_clicked_widget = NULL;

ei_bool_t recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag)
{
    ei_eventtype_t a_chercher = event->type;
    ei_bool_t sortie = EI_FALSE;
    struct liste_eventtypes_t *sentinel = liste;
    while (sentinel != NULL && sentinel->eventtype != a_chercher)
        sentinel = sentinel->next;

    if (sentinel != NULL)
    {
        struct liste_events_widgets *courant = sentinel->liste;
        while (courant != NULL)
        {
            if (a_chercher == courant->eventtype)
            {
                if (specifique == EI_FALSE || (specifique == EI_TRUE && ((courant->widget == NULL && widget == NULL && !strcmp(courant->tag, tag)) || courant->widget->pick_id == widget->pick_id)))
                {
                    sortie = courant->callback(courant->widget, event, courant->user_param);
                    if (sortie || arret_final)
                        break;
                }
            }
            courant = courant->next;
        }
    }
    return sortie;
}

void free_liste_event_widget(struct liste_events_widgets *liste)
{
    struct liste_events_widgets *courant = liste;
    struct liste_events_widgets *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free(courant);
    }
}

void free_liste_eventtypes(struct liste_eventtypes_t *liste)
{
    struct liste_eventtypes_t *courant = liste;
    struct liste_eventtypes_t *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free_liste_event_widget(courant->liste);
        free(courant);
    }
}

/************ FONCTIONS DE CALLBACK DÉJÀ CRÉÉES ************/

ei_bool_t relief_toggle(ei_widget_t *widget, ei_event_t *event)
{
    if (event->param.mouse.button == ei_mouse_button_left)
    {
        ei_button_t *bouton = ((ei_button_t *)widget);
        ei_widget_t *pointed_widget = ei_widget_pick(&event->param.mouse.where);

        /* S'il s'agit d'un mouvement du clic gauche, dans ce cas on cherche à savoir si on est ou pas sur le même bouton */
        if (event->param.mouse.button == ei_mouse_button_left && event->type == ei_ev_mouse_move && last_clicked_widget != NULL)
        {
            *((ei_button_t *)last_clicked_widget)->relief = (last_clicked_widget != pointed_widget) ? ei_relief_raised : ei_relief_sunken;
        }

        /* Si il s'agit d'une intéraction brève avec le bouton, on change son relief */
        else if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup)
        {
            *bouton->relief = (*bouton->relief == ei_relief_raised && event->type != ei_ev_mouse_buttonup) ? ei_relief_sunken : ei_relief_raised;

            if (event->type == ei_ev_mouse_buttondown)
                last_clicked_widget = widget;
        }

        /* Si on relâche le bouton, on appelle le callback */
        if (event->type == ei_ev_mouse_buttonup && last_clicked_widget == widget)
        {
            (bouton->callback != NULL) ? (*bouton->callback)(widget, event, *bouton->user_param) : 0;
            last_clicked_widget = NULL;
        }
    }
    return EI_TRUE;
}

ei_bool_t close_toplevel(ei_widget_t *widget)
{
    ei_widget_destroy(widget->parent);
    printf("Quit\n");
    return EI_TRUE;
}

ei_bool_t deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    if (!strcmp(widget->wclass->name, "toplevel") &&
        event->param.mouse.where.x <= widget->screen_location.top_left.x + widget->screen_location.size.width &&
        widget->screen_location.top_left.y <= event->param.mouse.where.y &&
        event->param.mouse.where.y <= widget->screen_location.top_left.y + taille_header)
    {
        is_moving = EI_TRUE;
        origine_deplacement.x = event->param.mouse.where.x;
        origine_deplacement.y = event->param.mouse.where.y;
    }

    if (!strcmp(widget->wclass->name, "toplevel") && *toplevel->resizable != ei_axis_none &&
        widget->content_rect->top_left.x + widget->content_rect->size.width - 15 <= event->param.mouse.where.x && event->param.mouse.where.x <= widget->content_rect->top_left.x + widget->content_rect->size.width + *toplevel->border_width &&
        widget->content_rect->top_left.y + widget->content_rect->size.height - 15 <= event->param.mouse.where.y && event->param.mouse.where.y <= widget->content_rect->top_left.y + widget->content_rect->size.height + *toplevel->border_width)
        is_resizing = EI_TRUE;

    return EI_TRUE;
}

ei_bool_t deplacement_actif(ei_widget_t *widget, struct ei_event_t *event)
{
    if (is_moving == EI_FALSE && is_resizing == EI_FALSE)
        return EI_FALSE;

    else
    {
        if (is_moving == EI_TRUE)
        {
            // if (0.01 * (float) event->param.mouse.where.x <= (float) widget->parent->content_rect->size.width + (float) widget->parent->content_rect->top_left.x - (float) widget->screen_location.top_left.x - 50. && 0.01 * (float)event->param.mouse.where.x >= (float)widget->parent->screen_location.top_left.x - (float)widget->screen_location.top_left.x && 0.01 * (float) event->param.mouse.where.y <= (float) widget->parent->content_rect->size.height + (float) widget->parent->content_rect->top_left.y - (float) widget->screen_location.top_left.y - 35. && event->param.mouse.where.y >= 50 + widget->parent->screen_location.top_left.y) {
            int delta_x = event->param.mouse.where.x - origine_deplacement.x;
            int delta_y = event->param.mouse.where.y - origine_deplacement.y;
            if ((widget->screen_location.top_left.x + delta_x < widget->parent->screen_location.top_left.x + widget->parent->content_rect->size.width - widget->content_rect->size.width &&
                 widget->screen_location.top_left.x + delta_x > widget->parent->screen_location.top_left.x))
            {
                widget->screen_location.top_left.x += delta_x;
                widget->content_rect->top_left.x += delta_x;
                ((ei_placer_t *)widget->geom_params)->x += delta_x;
                origine_deplacement.x = event->param.mouse.where.x;
            }
            if (
                (
                    (widget->parent->pick_id == 1 && widget->screen_location.top_left.y + delta_y < widget->parent->screen_location.top_left.y + widget->parent->content_rect->size.height - taille_header) ||
                    (widget->parent->pick_id != 1 && widget->screen_location.top_left.y + delta_y < widget->parent->screen_location.top_left.y + widget->parent->content_rect->size.height)) &&
                ((widget->parent->pick_id == 1 && widget->screen_location.top_left.y + delta_y > widget->parent->screen_location.top_left.y) ||
                 (widget->parent->pick_id != 1 && widget->screen_location.top_left.y + delta_y > widget->parent->screen_location.top_left.y + taille_header)))
            {
                widget->screen_location.top_left.y += delta_y;
                widget->content_rect->top_left.y += delta_y;
                ((ei_placer_t *)widget->geom_params)->y += delta_y;
                origine_deplacement.y = event->param.mouse.where.y;
            }
        }
        else if (is_resizing == EI_TRUE)
        {
            ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
            ei_size_t minimo = **(toplevel->min_size);
            if (*toplevel->resizable == ei_axis_x || *toplevel->resizable == ei_axis_both)
            {
                if (event->param.mouse.where.x - widget->screen_location.top_left.x > minimo.width)
                {
                    widget->screen_location.size.width =
                        event->param.mouse.where.x - widget->screen_location.top_left.x;
                    ((ei_placer_t *)widget->geom_params)->width = event->param.mouse.where.x - widget->screen_location.top_left.x;
                    widget->content_rect->size.width =
                        event->param.mouse.where.x - widget->content_rect->top_left.x;
                }
            }

            if (*toplevel->resizable == ei_axis_y || *toplevel->resizable == ei_axis_both)
            {
                if (event->param.mouse.where.y - widget->screen_location.top_left.y > minimo.height)
                {
                    ((ei_placer_t *)widget->geom_params)->height = event->param.mouse.where.y - widget->screen_location.top_left.y - taille_header;
                    widget->screen_location.size.height = event->param.mouse.where.y - widget->screen_location.top_left.y - taille_header;
                    widget->content_rect->size.height = event->param.mouse.where.y - widget->content_rect->top_left.y;
                }
            }
        }
        widget->wclass->geomnotifyfunc(widget);
        return EI_TRUE;
    }
}

ei_bool_t fin_deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event)
{

    if (is_moving == EI_FALSE && is_resizing == EI_FALSE)
        return EI_FALSE;

    else
    {
        if (is_moving == EI_TRUE)
        {
            is_moving = EI_FALSE;
        }
        else if (is_resizing == EI_TRUE)
        {
            ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
            ei_size_t minimo = **(toplevel->min_size);
            if (*toplevel->resizable == ei_axis_x || *toplevel->resizable == ei_axis_both)
            {
                if (event->param.mouse.where.x - widget->screen_location.top_left.x > minimo.width)
                {
                    widget->screen_location.size.width = event->param.mouse.where.x - widget->screen_location.top_left.x;
                    widget->content_rect->size.width = event->param.mouse.where.x - widget->content_rect->top_left.x;
                }
            }
            if (*toplevel->resizable == ei_axis_y || *toplevel->resizable == ei_axis_both)
            {
                if (event->param.mouse.where.y - widget->screen_location.top_left.y > minimo.height)
                {
                    widget->screen_location.size.height = event->param.mouse.where.y - widget->screen_location.top_left.y - taille_header;
                    widget->content_rect->size.height = event->param.mouse.where.y - widget->content_rect->top_left.y;
                }
            }
            is_resizing = EI_FALSE;
            widget->requested_size = widget->screen_location.size;
        }
        return EI_TRUE;
    }
}

#include "ei_autre_event.h"
#include "ei_autre_global_var.h"
#include "ei_application.h"
#include "ei_autre_fonctions.h"

extern ei_bool_t is_moving;
extern ei_bool_t is_resizing;
extern ei_bool_t arret_final;

static ei_point_t origine_deplacement;
ei_size_t starting_distance_resize;
static ei_widget_t *last_clicked_widget = NULL;

/**
 * @brief Fonction de recherche des callbacks pour un événement donné.
 * 
 * @param liste Une liste chainée de liste chainées (*liste).
 * 
 * @param event L'évènement dont on doit trouver les callback associés (*event)
 * 
 * @param specifique booleen pour savoir si l'évènement est spécifique à un widget ou à un tag
 * 
 * @param widget Valable uniquement si specifique vaut true.
 * 
 * @param tag Valable uniquement si specifique vaut true.
 * 
 * @return Un booleen indiquant si la fonction a trouvé un callback associé à cet événement.
 * 
 */
ei_bool_t recherche_traitants_event(liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag)
{
    ei_eventtype_t a_chercher = event->type;
    ei_bool_t sortie = EI_FALSE;
    liste_eventtypes_t *sentinel = liste;
    while (sentinel != NULL && sentinel->eventtype != a_chercher)
        sentinel = sentinel->next;

    if (sentinel != NULL)
    {
        liste_events *courant = sentinel->liste;
        while (courant != NULL)
        {
            if (a_chercher == courant->eventtype)
            {
                if (specifique == EI_TRUE && widget == NULL && tag == NULL) // Cas impossible. On sort
                    return sortie;

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

/**
 * @brief Permet de free la liste des events
 * 
 * @param liste Liste d'évènement à free
 * 
 */
static void free_liste_event_widget(liste_events *liste)
{
    liste_events *courant = liste;
    liste_events *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free(courant);
    }
}

/**
 * @brief Fonction de libération de la mémoire associée à une liste_eventtypes_t (cf. ei_autre_struct.h).
 * 
 * @param liste Correspond à la liste qu'il faut libérer en mémoire.
 * 
 */
void free_liste_eventtypes(liste_eventtypes_t *liste)
{
    liste_eventtypes_t *courant = liste;
    liste_eventtypes_t *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free_liste_event_widget(courant->liste);
        free(courant);
    }
}

/************ FONCTIONS DE CALLBACK DÉJÀ CRÉÉES ************/

/**
 * @brief Fonction générale utilisée comme callback lors de l'intéraction avec des boutons.
 * Cette fonction permet de gérer entre autres le changement de relief lors de l'intéraction
 * et l'appel de la fonction callback associée au bouton après le clic.
 * 
 * @param widget
 * 
 * @param event
 * 
 * @return
 */
ei_bool_t relief_toggle(ei_widget_t *widget, ei_event_t *event, void *user_params)
{
    ei_bool_t retour = EI_FALSE;
    ei_rect_t old_rect = widget->screen_location;

    if (event->param.mouse.button == ei_mouse_button_left)
    {
        ei_button_t *bouton = ((ei_button_t *)widget);
        ei_widget_t *pointed_widget = ei_widget_pick(&event->param.mouse.where);

        /* S'il s'agit d'un mouvement du clic gauche, dans ce cas on cherche à savoir si on est ou pas sur le même bouton */
        if (event->param.mouse.button == ei_mouse_button_left && event->type == ei_ev_mouse_move && last_clicked_widget != NULL)
        {
            ei_relief_t old_relief = *((ei_button_t *)last_clicked_widget)->relief;
            *((ei_button_t *)last_clicked_widget)->relief = (last_clicked_widget != pointed_widget) ? ei_relief_raised : ei_relief_sunken;
            if (old_relief != *((ei_button_t *)last_clicked_widget)->relief)
                retour = EI_TRUE;
        }

        /* Si il s'agit d'une intéraction brève avec le bouton, on change son relief */
        else if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup)
        {
            *bouton->relief = (*bouton->relief == ei_relief_raised && event->type != ei_ev_mouse_buttonup) ? ei_relief_sunken : ei_relief_raised;

            if (event->type == ei_ev_mouse_buttondown)
                last_clicked_widget = widget;

            retour = EI_TRUE;
        }

        /* Si on relâche le bouton, on appelle le callback */
        if (event->type == ei_ev_mouse_buttonup && last_clicked_widget == widget)
        {
            (*bouton->callback != NULL) ? (*bouton->callback)(widget, event, *bouton->user_param) : 0;
            last_clicked_widget = NULL;
            retour = EI_TRUE;
        }
    }

    if (retour)
        ei_app_invalidate_rect(&old_rect);

    return retour;
}

/** 
 * @brief
 * Fonction callback appelée lors du relachement d'un close_button des toplevel
 * Elle a pour effet de détruire tout les éléments s'y trouvant à l'intérieur.
 * 
 * @param widget Le widget (en l'occurance un toplebvel) qu'il faut fermer.
 * 
 * @return TRUE lorsqu'il est bien fermé.
 * 
 */
ei_bool_t close_toplevel(ei_widget_t *widget, ei_event_t *event, void *user_params)
{
    ei_rect_t old_rect = widget->parent->screen_location;
    old_rect.size.height += taille_header + *((ei_toplevel_t *)widget->parent)->border_width;
    old_rect.size.width += 2 * *((ei_toplevel_t *)widget->parent)->border_width;
    old_rect.top_left.x -= *((ei_toplevel_t *)widget->parent)->border_width;

    ei_app_invalidate_rect(&old_rect);
    
    ei_widget_destroy(widget->parent);

    return EI_TRUE;
}

/**
 * @brief Initialisation du déplacement du toplevel
 * 
 * @param widget Le widget (toplevel en l'occurance) que l'on déplace.
 *
 * @param event L'évènement associé (resize ou déplacement).
 *
 * @return Booléen retournant s'il y a une action à associer au widget.
 * 
 */ 
ei_bool_t deplacement_toplevel(ei_widget_t *widget, ei_event_t *event, void *user_params)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;
    if (!strcmp(widget->wclass->name, "toplevel") &&
        widget->screen_location.top_left.x <= event->param.mouse.where.x &&
        event->param.mouse.where.x <= widget->screen_location.top_left.x + widget->screen_location.size.width &&
        widget->screen_location.top_left.y <= event->param.mouse.where.y &&
        event->param.mouse.where.y <= widget->screen_location.top_left.y + taille_header)
    {
        is_moving = EI_TRUE;
        put_on_head(widget);
        origine_deplacement.x = event->param.mouse.where.x;
        origine_deplacement.y = event->param.mouse.where.y;
    }

    if (!strcmp(widget->wclass->name, "toplevel") && *toplevel->resizable != ei_axis_none &&
        widget->content_rect->top_left.x + widget->content_rect->size.width - 15 <= event->param.mouse.where.x && event->param.mouse.where.x <= widget->content_rect->top_left.x + widget->content_rect->size.width + *toplevel->border_width &&
        widget->content_rect->top_left.y + widget->content_rect->size.height - 15 <= event->param.mouse.where.y && event->param.mouse.where.y <= widget->content_rect->top_left.y + widget->content_rect->size.height + *toplevel->border_width)
        is_resizing = EI_TRUE;
        starting_distance_resize.width = widget->screen_location.top_left.x + widget->screen_location.size.width - event->param.mouse.where.x;
        starting_distance_resize.height = widget->screen_location.top_left.y + widget->screen_location.size.height - event->param.mouse.where.y;

    return EI_FALSE;
}

/**
 * @brief Permet de déplacer le widget et de l'afficher se déplaçant.
 * 
 * @param widget Le widget (toplevel en l'occurance) que l'on déplace.
 * 
 * @param event L'évènement associé (resize ou déplacement).
 * 
 * @return Booléen retournant s'il y a une action à associer au widget.
 * 
 */ 
ei_bool_t deplacement_actif(ei_widget_t *widget, ei_event_t *event, void *user_params)
{
    if (is_moving == EI_FALSE && is_resizing == EI_FALSE)
        return EI_FALSE;

    else
    {
        /* On sauvegarde l'ancien espace occupé par le toplevel afin de mettre à jour seulement ce rect */
        ei_rect_t old_rect = widget->screen_location;
        old_rect.size.height += taille_header + *((ei_toplevel_t *)widget)->border_width;
        old_rect.size.width += 2 * *((ei_toplevel_t *)widget)->border_width;
        old_rect.top_left.x -= *((ei_toplevel_t *)widget)->border_width;

        if (is_moving == EI_TRUE)
        {
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
            if (((widget->parent->pick_id == 1 && widget->screen_location.top_left.y + delta_y < widget->parent->screen_location.top_left.y + widget->parent->content_rect->size.height - taille_header) ||
                 (widget->parent->pick_id != 1 && widget->screen_location.top_left.y + delta_y < widget->parent->screen_location.top_left.y + widget->parent->content_rect->size.height)) &&
                ((widget->parent->pick_id == 1 && widget->screen_location.top_left.y + delta_y + 1 > widget->parent->screen_location.top_left.y) ||
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
                if (event->param.mouse.where.x - widget->screen_location.top_left.x + starting_distance_resize.width> minimo.width)
                {
                    widget->screen_location.size.width =
                        event->param.mouse.where.x - widget->screen_location.top_left.x + starting_distance_resize.width;
                    ((ei_placer_t *)widget->geom_params)->width = event->param.mouse.where.x - widget->screen_location.top_left.x + starting_distance_resize.width;
                    widget->content_rect->size.width =
                        event->param.mouse.where.x - widget->content_rect->top_left.x + starting_distance_resize.width;
                }
            }

            if (*toplevel->resizable == ei_axis_y || *toplevel->resizable == ei_axis_both)
            {
                if (event->param.mouse.where.y - widget->screen_location.top_left.y  + starting_distance_resize.height > minimo.height)
                {
                    ((ei_placer_t *)widget->geom_params)->height = event->param.mouse.where.y - widget->screen_location.top_left.y + starting_distance_resize.height;
                    widget->screen_location.size.height = event->param.mouse.where.y - widget->screen_location.top_left.y + starting_distance_resize.height;
                    widget->content_rect->size.height = event->param.mouse.where.y - widget->content_rect->top_left.y + starting_distance_resize.height;
                }
            }
        }
        widget->wclass->geomnotifyfunc(widget);

        /* La nouvelle position du rect global du toplevel permettant d'update selon cette partie de l'écran */
        ei_rect_t new_rect = widget->screen_location;
        new_rect.size.height += taille_header + *((ei_toplevel_t *)widget)->border_width;
        new_rect.size.width += 2 * *((ei_toplevel_t *)widget)->border_width;
        new_rect.top_left.x -= *((ei_toplevel_t *)widget)->border_width;

        ei_app_invalidate_rect(&old_rect);
        ei_app_invalidate_rect(&new_rect);

        return EI_TRUE;
    }
}

/**
 * @brief Déclare la fin du déplacement du toplevel.
 * 
 * @param widget le toplevel auquel l'event s'arrete.
 * 
 * @param event L'évènement qui agit sur le toplevel.
 * 
 * @return False s'il n'y avait pas de resize ni de mouvement, et True si le changement est finie.
 * 
 */ 
ei_bool_t fin_deplacement_toplevel(ei_widget_t *widget, ei_event_t *event, void *user_params)
{
    if (is_moving == EI_FALSE && is_resizing == EI_FALSE)
        return EI_FALSE;

    else
    {
        if (is_moving == EI_TRUE)
            is_moving = EI_FALSE;

        else if (is_resizing == EI_TRUE)
        {
            is_resizing = EI_FALSE;
            widget->requested_size = widget->screen_location.size;
        }

        return EI_TRUE;
    }
}

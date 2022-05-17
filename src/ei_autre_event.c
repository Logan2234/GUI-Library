#include "ei_autre_event.h"
#include "ei_autre_fonctions.h"

extern ei_bool_t arret;
extern ei_bool_t arret_final;
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
                    sortie = EI_TRUE;
                    arret = courant->callback(courant->widget, event, courant->user_param);
                    if (arret == EI_TRUE || arret_final == EI_TRUE)
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
            update_surface(rect_to_update, EI_TRUE);
        }

        /* Si il s'agit d'une intéraction brève avec le bouton, on change son relief */
        else if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup)
        {
            *bouton->relief = (*bouton->relief == ei_relief_raised && event->type != ei_ev_mouse_buttonup) ? ei_relief_sunken : ei_relief_raised;

            if (event->type == ei_ev_mouse_buttondown)
                last_clicked_widget = widget;

            update_surface(rect_to_update, EI_TRUE);
        }

        /* Si on relâche le bouton, on appelle le callback */
        if (event->type == ei_ev_mouse_buttonup && last_clicked_widget == widget)
        {
            (bouton->callback != NULL) ? (*bouton->callback)(widget, event, *bouton->user_param) : 0;
            last_clicked_widget = NULL;
        }
    }

    return EI_FALSE;
}
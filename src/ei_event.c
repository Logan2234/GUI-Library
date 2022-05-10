#include "ei_event.h"
#include "ei_autre_event.h"

extern struct liste_eventtypes_t *liste_events_widgets;

/* ON PEUT OPTIMISER AVEC UN DICTIONNAIRE. VOIR APRÈS SI ÇA FONCTIONNE */

void ei_bind(ei_eventtype_t eventtype, ei_widget_t *widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
    struct liste_eventtypes_t *precedant = liste_events_widgets;
    struct liste_eventtypes_t *sentinel = liste_events_widgets;

    while (sentinel != NULL && sentinel->eventtype != eventtype)
    {
        precedant = sentinel;
        sentinel = sentinel->next;
    }

    if (sentinel == NULL)
    {
        struct liste_eventtypes_t *nouveau_eventtype = calloc(1, sizeof(liste_eventtypes_t));
        struct liste_events_widgets *nouveau_widget = calloc(1, sizeof(struct liste_events_widgets));

        nouveau_eventtype->eventtype = eventtype;
        nouveau_widget->widget = widget;
        nouveau_widget->eventtype = eventtype;
        nouveau_widget->user_param = user_param;
        nouveau_widget->callback = callback;
        nouveau_eventtype->liste = nouveau_widget;
        if (liste_events_widgets == NULL)
            liste_events_widgets = nouveau_eventtype;
        else
            precedant->next = nouveau_eventtype;
    }
    else
    {
        struct liste_events_widgets *suivant = sentinel->liste;
        while (suivant->next != NULL)
        {
            if (suivant->widget->pick_id == widget->pick_id || (suivant->widget == NULL && widget == NULL))
            {
                suivant->callback = callback;
                suivant->user_param = user_param;
                return;
            }
            suivant = suivant->next;
        }
        if (suivant->widget->pick_id == widget->pick_id)
        {
            suivant->callback = callback;
            suivant->user_param = user_param;
            return;
        }
        else
        {
            struct liste_events_widgets *nouveau = calloc(1, sizeof(struct liste_events_widgets));
            nouveau->widget = widget;
            nouveau->eventtype = eventtype;
            nouveau->user_param = user_param;
            nouveau->callback = callback;
            suivant->next = nouveau;
        }
    }
}

// Pas serein pour les free
void ei_unbind(ei_eventtype_t eventtype, ei_widget_t *widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
    struct liste_eventtypes_t *sentinel = liste_events_widgets;
    while (sentinel != NULL && sentinel->eventtype != eventtype)
        sentinel = sentinel->next;

    if (sentinel != NULL)
    {
        struct liste_events_widgets *liste_widget = sentinel->liste;
        if (liste_widget != NULL)
        {
            struct liste_events_widgets *suivant = liste_widget;
            struct liste_events_widgets *ancien = liste_widget;
            if ((suivant->widget->pick_id == widget->pick_id || (suivant->widget == NULL && widget == NULL)) && suivant->callback == callback &&
                suivant->user_param == user_param && suivant->eventtype == eventtype)
            {
                liste_widget = liste_widget->next;
                free(ancien);
                return;
            }
            while (suivant->next != NULL)
            {
                if (suivant->next->widget->pick_id == widget->pick_id && suivant->callback == callback &&
                    suivant->user_param == user_param && suivant->eventtype == eventtype)
                {
                    ancien = suivant->next;
                    suivant->next = suivant->next->next;
                    free(ancien);
                    return;
                }
                else
                {
                    ancien = suivant;
                    suivant = suivant->next;
                }
            }
            if ((suivant->widget->pick_id == widget->pick_id || (suivant->widget == NULL && widget == NULL)) && suivant->callback == callback &&
                suivant->user_param == user_param && suivant->eventtype == eventtype)
            {
                suivant = ancien->next;
                ancien->next = NULL;
                free(suivant);
            }
        }
    }
}

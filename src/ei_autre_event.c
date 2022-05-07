#include "ei_autre_event.h"

void ajout_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget, ei_eventtype_t eventtype, ei_callback_t callback, void *user_param)
{
    struct liste_events_widgets *nouveau;
    nouveau->widget = widget;
    nouveau->eventtype = eventtype;
    nouveau->callback = callback;
    nouveau->user_param = user_param;
    nouveau->next = NULL;
    if (liste == NULL)
    {
        liste = nouveau;
        return;
    } else {
        struct liste_events_widgets *suivant = liste;
        while (suivant->next != NULL) {
            suivant = suivant->next;
        }
        suivant->next = nouveau;
    }
}

void supprimer_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget)
{
    if (liste != NULL) {
        struct liste_events_widgets *suivant = liste;
        struct liste_events_widgets *ancien;
        if (suivant->widget.pick_id == widget.pick_id) {
            liste = liste->next;
            return;
        }
        while (suivant->next != NULL) {
            if (suivant->next->widget.pick_id == widget.pick_id) {
                suivant->next = suivant->next->next;
                return;
            } else {
                ancien = suivant;
                suivant = suivant->next;
            }
        }
        if (suivant->widget.pick_id == widget.pick_id) {
            ancien->next = NULL;
        }
    }
}
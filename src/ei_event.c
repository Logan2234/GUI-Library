#include "ei_event.h"
#include "ei_autre_event.h"

struct liste_events_widgets *liste_widget;

/* ON PEUT OPTIMISER AVEC UN DICTIONNAIRE. VOIR APRÈS SI ÇA FONCTIONNE */

void ei_bind(ei_eventtype_t eventtype,
             ei_widget_t *widget,
             ei_tag_t tag,
             ei_callback_t callback,
             void *user_param)
{
    /*
    struct liste_events_widgets *suivant = liste_widget;
    while (suivant->next != NULL)
    {
        if (suivant->widget.pick_id == widget->pick_id)
        {
            suivant->callback = callback;
            suivant->user_param = user_param;
            suivant->eventtype = eventtype;
            return;
        }
        suivant = suivant->next;
    }
    if (suivant->widget.pick_id == widget->pick_id)
    {
        suivant->callback = callback;
        suivant->user_param = user_param;
        suivant->eventtype = eventtype;
        return;
    } else {
        struct liste_events_widgets *nouveau;
        nouveau->widget = *widget;
        nouveau->eventtype = eventtype;
        nouveau->user_param = user_param;
        nouveau->callback = callback;
        nouveau->next = NULL;
        suivant->next = nouveau;
    }
     */
    // TODO
}

void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t *widget,
               ei_tag_t tag,
               ei_callback_t callback,
               void *user_param)
{
    /*
    if (liste_widget != NULL) {
        struct liste_events_widgets *suivant = liste_widget;
        struct liste_events_widgets *ancien;
        if (suivant->widget.pick_id == widget->pick_id && suivant->callback == callback && suivant->user_param == user_param && suivant->eventtype == eventtype) {
            liste_widget = liste_widget->next;
            return;
        }
        while (suivant->next != NULL) {
            if (suivant->next->widget.pick_id == widget->pick_id && suivant->callback == callback && suivant->user_param == user_param && suivant->eventtype == eventtype) {
                suivant->next = suivant->next->next;
                return;
            } else {
                ancien = suivant;
                suivant = suivant->next;
            }
        }
        if (suivant->widget.pick_id == widget->pick_id && suivant->callback == callback && suivant->user_param == user_param && suivant->eventtype == eventtype) {
            ancien->next = NULL;
        }
    }
     */
    // TODO
}

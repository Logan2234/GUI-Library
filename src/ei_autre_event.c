#include "ei_autre_event.h"
extern ei_bool_t arret;

/* FONCTIONS INCOMPLÈTES ET DONT JE DOUTE DE SON UTILITÉ
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
    ei_eventtype_t a_chercher = (ei_eventtype_t) event;
    struct liste_eventtypes_t *sentinel = liste;
    while (sentinel != NULL && sentinel->eventtype != a_chercher)
        sentinel = sentinel->next;

    if (sentinel != NULL) {
        struct liste_events_widgets *suivant = sentinel->liste;
        struct liste_events_widgets *ancien;
        if (suivant->widget.pick_id == widget.pick_id) {
            *liste = *(liste->next);
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
 */



void recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget)
{
    ei_eventtype_t a_chercher = event->type;
    struct liste_eventtypes_t *sentinel = liste;
    while (sentinel != NULL && sentinel->eventtype != a_chercher)
        sentinel = sentinel->next;

    if (sentinel != NULL) {

        struct liste_events_widgets *courant = sentinel->liste;
        while (courant != NULL) {
            if (a_chercher == courant->eventtype) {
                if (specifique == EI_FALSE || (specifique == EI_TRUE && courant->widget->pick_id == widget->pick_id)) {
                    arret = courant->callback(courant->widget, event, courant->user_param);
                    if (arret == EI_TRUE)
                        break;
                }
            }
            courant = courant->next;
        }
    }
}

void free_liste_event_widget(struct liste_events_widgets *liste){
    struct liste_events_widgets *courant = liste;
    struct liste_events_widgets *suivant = liste;
    while (suivant != NULL) {
        courant = suivant;
        suivant = suivant->next;
        free(courant);
    }
}

void free_liste_eventtypes(struct liste_eventtypes_t *liste){
    struct liste_eventtypes_t *courant = liste;
    struct liste_eventtypes_t *suivant = liste;
    while (suivant != NULL) {
        courant = suivant;
        suivant = suivant->next;
        free_liste_event_widget(courant->liste);
        free(courant);
    }
}
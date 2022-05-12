#include "ei_event.h"
#include "ei_autre_struct.h"

void ajout_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget, ei_eventtype_t eventtype, ei_callback_t callback, void *user_param);

void supprimer_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget);

void recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget);

void free_liste_event_widget(struct liste_events_widgets *liste);

void free_liste_eventtypes(struct liste_eventtypes_t *liste);
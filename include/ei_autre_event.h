#include "ei_event.h"
#include "ei_autre_struct.h"

uint8_t recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag);

void free_liste_event_widget(struct liste_events_widgets *liste);

void free_liste_eventtypes(struct liste_eventtypes_t *liste);
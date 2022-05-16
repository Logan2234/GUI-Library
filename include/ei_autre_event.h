#include "ei_event.h"
#include "ei_autre_struct.h"

ei_bool_t recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag);

void free_liste_eventtypes(struct liste_eventtypes_t *liste);
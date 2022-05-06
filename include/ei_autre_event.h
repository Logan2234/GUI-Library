#ifndef PROJETC_IG_EI_AUTRE_EVENT_H
#define PROJETC_IG_EI_AUTRE_EVENT_H

#endif //PROJETC_IG_EI_AUTRE_EVENT_H

#include "ei_event.h"

/* Liste chainée liant widgets, events et callback */
typedef struct liste_events_widgets
{
    ei_widget_t widget;
    ei_event_t event;
    ei_callback_t callback;
    void *user_param;
    struct liste_events_widgets* next;
} liste_events;

void ajout_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget, ei_event_t event, ei_callback_t callback, void *user_param);

void supprimer_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget);
#include "ei_event.h"
#include "ei_application.h"

/* Liste chainée liant widgets, events et callback */
typedef struct liste_events_widgets
{
    ei_widget_t *widget;
    ei_eventtype_t eventtype;
    ei_callback_t callback;
    void *user_param;
    struct liste_events_widgets* next;
} liste_events;

/* Liste chainée de eventype menant vers de liste_events_widgets */
typedef struct liste_eventtypes_t
{
    ei_eventtype_t eventtype;
    struct liste_events_widgets* liste;
    struct liste_eventtypes_t* next;
} liste_eventtypes_t;

/*
 * Fonction gérant l'enfoncement du bouton lors de l'appui
 */
ei_bool_t button_click(ei_widget_t* widget, ei_event_t* event, void* user_param);

void ajout_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget, ei_eventtype_t eventtype, ei_callback_t callback, void *user_param);

void supprimer_liste_event_widget(struct liste_events_widgets *liste, ei_widget_t widget);

void recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget);

void free_liste_event_widget(struct liste_events_widgets *liste);

void free_liste_eventtypes(struct liste_eventtypes_t *liste);
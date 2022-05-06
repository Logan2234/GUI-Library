#include "ei_event.h"
#include "ei_autre_event.h"

struct liste_events_widgets *liste_widget;

void ei_bind(ei_eventtype_t eventtype,
             ei_widget_t *widget,
             ei_tag_t tag,
             ei_callback_t callback,
             void *user_param)
{
    // TODO
}

void ei_unbind(ei_eventtype_t eventtype,
               ei_widget_t *widget,
               ei_tag_t tag,
               ei_callback_t callback,
               void *user_param)
{
    // TODO
}


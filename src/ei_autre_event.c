#include "ei_autre_event.h"

extern ei_bool_t arret;

uint8_t recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag)
{
    ei_eventtype_t a_chercher = event->type;
    uint8_t sortie = 0;
    struct liste_eventtypes_t *sentinel = liste;
    while (sentinel != NULL && sentinel->eventtype != a_chercher)
        sentinel = sentinel->next;

    if (sentinel != NULL)
    {
        struct liste_events_widgets *courant = sentinel->liste;
        while (courant != NULL)
        {
            if (a_chercher == courant->eventtype)
            {
                if (specifique == EI_FALSE || (specifique == EI_TRUE && ((courant->widget == NULL && widget == NULL && !strcmp(courant->tag, tag)) || courant->widget->pick_id == widget->pick_id)))
                {
                    sortie = 1;
                    arret = courant->callback(courant->widget, event, courant->user_param);
                    if (arret == EI_TRUE)
                        break;
                }
            }
            courant = courant->next;
        }
    }
    return sortie;
}

void free_liste_event_widget(struct liste_events_widgets *liste)
{
    struct liste_events_widgets *courant = liste;
    struct liste_events_widgets *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free(courant);
    }
}

void free_liste_eventtypes(struct liste_eventtypes_t *liste)
{
    struct liste_eventtypes_t *courant = liste;
    struct liste_eventtypes_t *suivant = liste;
    while (suivant != NULL)
    {
        courant = suivant;
        suivant = suivant->next;
        free_liste_event_widget(courant->liste);
        free(courant);
    }
}
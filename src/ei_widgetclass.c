#include "ei_widgetclass.h"

struct liste_widgetclass
{
    ei_widgetclass_t *first_widgetclass;
    struct liste_widgetclass *next;
};


void ei_widgetclass_register (ei_widgetclass_t* widgetclass)
{
    struct liste_widgetclass *liste_widgetclass = calloc(1, sizeof(struct liste_widgetclass));
    if (liste_widgetclass->first_widgetclass == NULL){
        liste_widgetclass->first_widgetclass = widgetclass;
    }
    else
    {
        struct liste_widgetclass *new_cell = calloc(1, sizeof(struct liste_widgetclass));
        new_cell->first_widgetclass = widgetclass;
        liste_widgetclass->next = new_cell;
    }
}
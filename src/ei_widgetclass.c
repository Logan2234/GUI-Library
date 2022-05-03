#include "ei_widgetclass.h"
#include "ei_autre_struct.h"

extern struct liste_widgetclass *liste_widgetclass;

void ei_widgetclass_register (ei_widgetclass_t* widgetclass)
{
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
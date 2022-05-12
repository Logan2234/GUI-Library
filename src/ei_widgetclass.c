#include "ei_widgetclass.h"
#include "ei_autre_struct.h"

extern struct liste_widgetclass *liste_widgetclass;

void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
    if (liste_widgetclass->first_widgetclass == NULL)
        liste_widgetclass->first_widgetclass = widgetclass;
    else
    {
        struct liste_widgetclass *sent = liste_widgetclass;
        while (sent->next != NULL)
            sent = sent->next;
            
        struct liste_widgetclass *new = calloc(1, sizeof(struct liste_widgetclass));
        new->first_widgetclass = widgetclass;
        sent->next = new;
    }
}

ei_widgetclass_t *ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
    struct liste_widgetclass *sent = liste_widgetclass;
    while (strcmp(sent->first_widgetclass->name, name) && sent->next != NULL)
        sent = sent->next;

    if (!strcmp(sent->first_widgetclass->name, name))
        return sent->first_widgetclass;
    return NULL;
}

void ei_frame_register_class(void)
{
    ei_widgetclass_t *frame = return_class_frame();
    ei_widgetclass_register(frame);
}

void ei_button_register_class(void)
{
    ei_widgetclass_t *button = return_class_button();
    ei_widgetclass_register(button);
}

void ei_toplevel_register_class(void)
{
    ei_widgetclass_t *toplevel = return_class_toplevel();
    ei_widgetclass_register(toplevel);
}
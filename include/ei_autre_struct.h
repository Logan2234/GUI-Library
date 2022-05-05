#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"

struct liste_widgetclass
{
    ei_widgetclass_t *first_widgetclass;
    struct liste_widgetclass *next;
};

struct liste_geometrymanager
{
    ei_geometrymanager_t *geometrymanager_cell;
    struct liste_geometrymanager *next;
};

ei_geometrymanager_t *return_geometry_manager_placer();
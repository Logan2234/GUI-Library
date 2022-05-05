#include "ei_geometrymanager.h"

void placer_runfunc(struct ei_widget_t *widget)
{
}

void placer_releasefunc(struct ei_widget_t *widget)
{
}

ei_geometrymanager_t *return_geometry_manager_placer()
{
    ei_geometrymanager_t *placer = calloc(1, sizeof(ei_geometrymanager_t));
    strcpy(placer->name, "placer");
    placer->runfunc = &placer_runfunc;
    placer->releasefunc = &placer_releasefunc;
    placer->next = NULL;

    return placer;
}
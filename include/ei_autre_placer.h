#include "ei_widget.h"
#include "ei_geometrymanager.h"

typedef struct ei_placer_t{
    ei_geometry_param_t manager;
    ei_anchor_t *anchor;
    int x;
    int y;
    float rel_x;
    float rel_y;
    int width;
    int height;
    float rel_width;
    float rel_height;
} ei_placer_t;

void placer_runfunc(ei_widget_t *widget);

void placer_releasefunc(struct ei_widget_t *widget);

//ei_geometrymanager_t *return_geometry_manager_placer();


#ifndef PROJETC_IG_EI_AUTRE_PLACER_H
#define PROJETC_IG_EI_AUTRE_PLACER_H

#endif //PROJETC_IG_EI_AUTRE_PLACER_H

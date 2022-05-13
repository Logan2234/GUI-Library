#include "ei_widget.h"
#include "ei_geometrymanager.h"

typedef struct ei_placer_t{
    ei_widget_t *widget;
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

void placer_runfunc(struct ei_placer_t *placer);

void placer_releasefunc(struct ei_widget_t *widget);

//ei_geometrymanager_t *return_geometry_manager_placer();
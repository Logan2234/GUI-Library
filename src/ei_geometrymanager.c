#include "ei_autre_struct.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"
#include "ei_placer.h"

extern struct liste_geometrymanager *liste_geometrymanager;

void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
    if (liste_geometrymanager->geometrymanager_cell == NULL)
        liste_geometrymanager->geometrymanager_cell = geometrymanager;

    else
    {
        struct liste_geometrymanager *sent = liste_geometrymanager;
        while (sent->next != NULL)
            sent = sent->next;

        struct liste_geometrymanager *new_cell = calloc(1, sizeof(struct liste_geometrymanager));
        new_cell->geometrymanager_cell = geometrymanager;
        sent->next = new_cell;
    }
}

void ei_geometrymanager_unmap(ei_widget_t *widget)
{
    widget->geom_params->manager->releasefunc(widget);
    widget->geom_params = NULL;
    ei_app_invalidate_rect(&widget->screen_location);
    widget->screen_location = (!strcmp(widget->wclass->name, "frame"))    ? (ei_rect_t){0, 0, default_frame_size}
                              : (!strcmp(widget->wclass->name, "button")) ? (ei_rect_t){0, 0, default_button_size}
                                                                          : (ei_rect_t){0, 0, default_toplevel_size};
}

ei_geometrymanager_t *ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{
    struct liste_geometrymanager *sent = liste_geometrymanager;
    while (strcmp(sent->geometrymanager_cell->name, name) && sent->next != NULL)
        sent = sent->next;

    if (!strcmp(sent->geometrymanager_cell->name, name))
        return sent->geometrymanager_cell;
}

void ei_register_placer_manager(void)
{
    ei_geometrymanager_t *placer = return_geometry_manager_placer();
    ei_geometrymanager_register(placer);
}


void ei_place(ei_widget_t *widget, ei_anchor_t *anchor, int *x, int *y, int *width,
              int *height, float *rel_x, float *rel_y, float *rel_width, float *rel_height)
{
    int width_parent = widget->requested_size.width;
    int height_parent = widget->requested_size.height;
    struct ei_placer_t *placer;
    placer->widget = widget;
    placer->anchor = anchor;
    placer->x = ((x == NULL) ? widget->content_rect->top_left.x : *x);
    placer->y = ((y == NULL) ? widget->content_rect->top_left.y : *y);
    placer->rel_x = ((rel_x == NULL) ? -1 : *rel_x);
    placer->rel_y = ((rel_y == NULL) ? -1 : *rel_y);
    placer->rel_width = ((rel_width == NULL) ? 0 : *rel_width);
    placer->rel_height = ((rel_height == NULL) ? 0 : *rel_height);
    placer->width = ((width == NULL) ? (rel_width == NULL) ? widget->requested_size.width : 0 : *width);
    placer->width += placer->rel_width * width_parent;
    placer->height = ((height == NULL) ? (rel_height == NULL) ? widget->requested_size.height : 0 : *height);
    placer->height += placer->rel_height* height_parent;
    placer_runfunc(placer);
}

void ei_geometrymanager_releasefunc (struct ei_widget_t* widget){

}
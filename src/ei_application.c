#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_autre_frame.h"
#include "ei_widgetclass.h"
#include "ei_autre_struct.h"

struct liste_widgetclass *liste_widgetclass;
static ei_surface_t racine_surface;
static ei_surface_t pick_surface;
static ei_widget_t *widget_racine;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();
    liste_widgetclass = calloc(1, sizeof(struct liste_widgetclass));
    ei_widgetclass_register(return_class_frame());
    // ei_geometrymanager_register();
    racine_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(racine_surface, main_window_size, EI_TRUE);
    widget_racine = ei_widget_create("frame", NULL, NULL, NULL);
    widget_racine->wclass->setdefaultsfunc(widget_racine);
    ei_frame_configure(widget_racine, NULL, &ei_default_background_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_run()
{
    ei_widget_t *racine = ei_app_root_widget();
    hw_surface_lock(racine_surface);
    racine->wclass->drawfunc(racine, racine_surface, pick_surface, NULL);
    hw_surface_unlock(racine_surface);
   	hw_surface_update_rects(racine_surface, NULL);
    getchar();
}

void ei_app_free()
{
    ei_widgetclass_t *suivant;
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL){
        suivant = liste_widgetclass->next;
        liste_widgetclass->first_widgetclass->releasefunc(liste_widgetclass->first_widgetclass);
        liste_widgetclass = suivant;
    }
}

ei_widget_t* ei_app_root_widget(void)
{
    return widget_racine;
}
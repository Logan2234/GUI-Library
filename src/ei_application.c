#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "ei_autre_struct.h"

struct liste_widgetclass *liste_widgetclass;
struct liste_geometrymanager *liste_geometrymanager;
static ei_surface_t racine_surface;
static ei_surface_t pick_surface;
static ei_widget_t *widget_racine;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();

    liste_widgetclass = calloc(1, sizeof(struct liste_widgetclass));
    liste_geometrymanager = calloc(1, sizeof(struct liste_geometrymanager));

    /* Enregistrement des différentes classes de widget */
    ei_widgetclass_register(return_class_frame());
    ei_widgetclass_register(return_class_button());
    ei_widgetclass_register(return_class_toplevel());
    /* Enregistrement des différents gestionnaires de géométrie */
    ei_geometrymanager_register(return_geometry_manager_placer());

    /* Création du widget root et de ses surfaces */
    widget_racine = ei_widget_create("frame", NULL, NULL, NULL);
    racine_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(racine_surface, main_window_size, EI_TRUE);
    ei_frame_configure(widget_racine, NULL, &ei_default_background_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_run()
{
    ei_widget_t *racine = ei_app_root_widget();
    hw_surface_lock(racine_surface);
    racine->wclass->drawfunc(racine, racine_surface, pick_surface, NULL);
    hw_surface_unlock(racine_surface);
    hw_surface_update_rects(racine_surface, NULL);
    struct ei_event_t *event = malloc(sizeof(ei_event_t));
    while (event->type != ei_ev_mouse_move)
    {
        hw_event_wait_next(event);
    }
}

void ei_app_free()
{
    // TODO Il faut créer une liste chainée ou jsp mais pour enregistrer les widgets
    // while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL){
    //     liste_widgetclass->first_widgetclass->releasefunc(liste_widgetclass->first_widgetclass);
    //     liste_widgetclass = liste_widgetclass->next;
    // }

    /* On libère la liste chaînée des widget class */
    while (liste_widgetclass != NULL)
    {
        struct liste_widgetclass *next = liste_widgetclass->next;
        free(liste_widgetclass);
        liste_widgetclass = next;
    }

    /* On libère la liste chaînée des geometry manager */
    while (liste_geometrymanager != NULL)
    {
        struct liste_geometrymanager *next = liste_geometrymanager->next;
        free(liste_geometrymanager);
        liste_geometrymanager = next;
    }
}

ei_widget_t *ei_app_root_widget(void)
{
    return widget_racine;
}
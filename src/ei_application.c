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

void draw_widgets_and_family(ei_widget_t *widget)
{
    (widget->parent == NULL) ? widget->wclass->drawfunc(widget, racine_surface, pick_surface, NULL) : widget->wclass->drawfunc(widget, racine_surface, pick_surface, &(widget->screen_location));
    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        draw_widgets_and_family(current_widget->next_sibling);
    if (current_widget->children_head != NULL)
        draw_widgets_and_family(current_widget->children_head);
}

void ei_app_run()
{
    hw_surface_lock(racine_surface);
    // hw_surface_get_buffer(racine_surface); // TODO A utiliser qq part
    draw_widgets_and_family(widget_racine);
    hw_surface_unlock(racine_surface);
    hw_surface_update_rects(racine_surface, NULL);
    struct ei_event_t *event = malloc(sizeof(ei_event_t));
    while (event->type != ei_ev_mouse_buttondown)
    {
        hw_event_wait_next(event);
    }
    free(event);
}

void free_widgets_and_family(ei_widget_t *widget)
{
    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        free_widgets_and_family(current_widget->next_sibling);

    if (current_widget->children_head != NULL)
        free_widgets_and_family(current_widget->children_head);

    (widget->destructor != NULL) ? widget->destructor(widget) : NULL;
    free(widget);
}

void ei_app_free()
{
    /* On libère les surfaces */
    hw_surface_free(pick_surface);

    /* On supprime tout les widgets */
    free_widgets_and_family(widget_racine);

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

    /* On libère les ressources créées par hw_init */
    hw_quit();
}

ei_widget_t *ei_app_root_widget(void)
{
    return widget_racine;
}

ei_surface_t ei_app_root_surface(void)
{
    return racine_surface;
}

void ei_app_invalidate_rect(ei_rect_t *rect)
{
    // TODO
}

void ei_app_quit_request(void)
{
    // TODO
}

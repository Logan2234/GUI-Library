#include "ei_application.h"
#include "ei_event.h"
#include "ei_autre_event.h"
#include "ei_autre_fonctions.h"

/* Paramètres globaux de l'appli */
struct liste_widgetclass *liste_widgetclass;
struct liste_geometrymanager *liste_geometrymanager;
struct liste_eventtypes_t *liste_events_widgets;
ei_linked_rect_t *rect_to_update;
ei_widget_t *widget_racine;
ei_surface_t racine_surface;
ei_surface_t pick_surface;
int widget_id = 0;
ei_bool_t deplacement = EI_FALSE;
ei_bool_t re_size = EI_FALSE;
ei_point_t origine_deplacement;
ei_bool_t arret = EI_FALSE;
ei_bool_t arret_final = EI_FALSE;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();

    liste_widgetclass = calloc(1, sizeof(struct liste_widgetclass));
    liste_geometrymanager = calloc(1, sizeof(struct liste_geometrymanager));
    liste_events_widgets = calloc(1, sizeof(struct liste_eventtypes_t));
    rect_to_update = calloc(1, sizeof(ei_linked_rect_t));

    /* Enregistrement des différentes classes de widget */
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();

    /* Enregistrement des différents gestionnaires de géométrie */
    ei_geometrymanager_register(return_geometry_manager_placer());

    /* Création du widget root et de ses surfaces */
    racine_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(racine_surface, main_window_size, EI_TRUE);
    widget_racine = ei_widget_create("frame\0\0\0\0\0\0\0\0\0\0\0\0\0\0 ", NULL, NULL, NULL);
    ei_frame_configure(widget_racine, NULL, &ei_default_background_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void ei_app_run()
{
    init_toplevel(widget_racine);

    struct ei_event_t *event = calloc(1, sizeof(ei_event_t));
    ei_widget_t *pressed_widget = NULL;
    ei_widget_t *released_widget;
    update_surface(rect_to_update);
    // double frame_rate = (double) 1 / 128;
    // double avant = hw_now();

    while (arret_final == EI_FALSE) // Comment faire pour annoncer qu'on quit
    {
        // update_surface(rect_to_update);
        // double avant = hw_now();
        // while (hw_now() - avant <= frame_rate) {
        hw_event_wait_next(event);
        if (event->type < 5)
        {
            recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);
            // while (hw_now() - avant > (double) ((float) 1 / 60 ))
            //    continue;
            update_surface(rect_to_update);
            // avant = hw_now();
        }

        /* Cas où on appuie avec le clic gauche */
        else if (event->type == ei_ev_mouse_buttondown)
        {
            pressed_widget = ei_widget_pick(&event->param.mouse.where);
            recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL);
        }

        /* Cas où on relache le clic gauche */
        else if (event->type == ei_ev_mouse_buttonup && event->param.mouse.button == ei_mouse_button_left)
        {
            released_widget = ei_widget_pick(&event->param.mouse.where);
            recherche_traitants_event(liste_events_widgets, event, EI_TRUE, released_widget, NULL);
            pressed_widget = NULL;
            released_widget = NULL;
        }

        /* Si on ressort du bouton avec le clic appuyé, on redonne la forme normale du potentiel bouton cliqué et inversement */
        else if (event->type == ei_ev_mouse_move)
        {
            if (deplacement || re_size)
            {
                recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL);
                // while (hw_now() - avant < (double) ((float) 1 / 60 ))
                //    continue;
                update_surface(rect_to_update);
                // avant = hw_now();
            }
            else if (pressed_widget != NULL && !strcmp(pressed_widget->wclass->name, "button"))
            {
                recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL);
            }
        }
    }
    free(event);
}
int compte = 0;
void ei_app_free()
{
    /* On libère les surfaces */
    hw_surface_free(pick_surface);

    /* On supprime tout les widgets */
    ei_widget_destroy(widget_racine);

    /* On libère la liste chaînée des widget class */
    while (liste_widgetclass != NULL)
    {
        struct liste_widgetclass *next = liste_widgetclass->next;
        free(liste_widgetclass->first_widgetclass);
        free(liste_widgetclass);
        liste_widgetclass = next;
    }

    /* On libère la liste chaînée des geometry manager */
    while (liste_geometrymanager != NULL)
    {
        struct liste_geometrymanager *next = liste_geometrymanager->next;
        free(liste_geometrymanager->geometrymanager_cell);
        free(liste_geometrymanager);
        liste_geometrymanager = next;
    }

    /* On libère la liste chaînée des event types */
    free_liste_eventtypes(liste_events_widgets);

    while (rect_to_update != NULL)
    {
        ei_linked_rect_t *next = rect_to_update->next;
        free(rect_to_update);
        rect_to_update = next;
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
    ei_linked_rect_t *sent = rect_to_update;
    while (sent->next != NULL)
        sent = sent->next;
    sent->next = calloc(1, sizeof(ei_linked_rect_t));
    sent->next->rect = *rect;
}

void ei_app_quit_request(void)
{
    arret_final = EI_TRUE;
}

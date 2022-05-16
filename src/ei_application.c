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
ei_linked_rect_t *surface_a_update;

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
    ei_widget_t *pointed_widget;
    ei_widget_t *pressed_widget = NULL;
    ei_widget_t *released_widget;

    //update_surface(rect_to_update);
    
    while (arret == EI_FALSE) // Comment faire pour annoncer qu'on quit
    {
        hw_event_wait_next(event);
        if (event->type < 5)
        {
            recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);
            update_surface(rect_to_update);
        }

            /* Cas où on appuie avec le clic gauche */
        else if (event->type == ei_ev_mouse_buttondown && event->param.mouse.button == ei_mouse_button_left)
        {
            pressed_widget = ei_widget_pick(&event->param.mouse.where);
            if (!strcmp(pressed_widget->wclass->name, "button"))
            {
                *((ei_button_t *)pressed_widget)->relief = ei_relief_sunken;
                update_surface(rect_to_update);
            }
            recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);
            update_surface(rect_to_update);
        }

            /* Cas où on relache le clic gauche */
        else if (event->type == ei_ev_mouse_buttonup && event->param.mouse.button == ei_mouse_button_left)
        {
            /* Maintenant on test si on relache le clic sur le même widget que sur celui que l'on vient d'appuyer */
            released_widget = ei_widget_pick(&event->param.mouse.where);
            if (!strcmp(released_widget->wclass->name, "button") && !strcmp(pressed_widget->wclass->name, "button"))
            {
                /* Si c'est le même on appelle le callback et on redessine le relief*/
                *((ei_button_t *)pressed_widget)->relief = ei_relief_raised;
                (pressed_widget == released_widget) ? (((ei_button_t *)released_widget)->callback != NULL)
                                                      ? (*((ei_button_t *)released_widget)->callback)(released_widget, event, NULL)
                                                      : 0
                                                    : 0;
            }
            pressed_widget = NULL;
            recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);
            if (deplacement == EI_TRUE || re_size == EI_TRUE) {
                update_surface(rect_to_update);
                free(surface_a_update->next);
                free(surface_a_update);
            }
        }

            /* Si on ressort du bouton avec le clic appuyé, on redonne la forme normale du potentiel bouton cliqué et inversement */
        else if (event->type == ei_ev_mouse_move)
        {
            if (pressed_widget != NULL && !strcmp(pressed_widget->wclass->name, "button"))
            {
                pointed_widget = ei_widget_pick(&event->param.mouse.where);
                *((ei_button_t *)pressed_widget)->relief = (pointed_widget != pressed_widget) ? ei_relief_raised
                                                                                              : ei_relief_sunken;
                update_surface(rect_to_update);
            }
            if (deplacement == EI_TRUE || re_size == EI_TRUE) {
                recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);
                update_surface(surface_a_update);
                free(surface_a_update->next);
                free(surface_a_update);
            }
        }
    }
    free(event);
}

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
    arret = EI_TRUE;
}

#include "ei_autre_event.h"
#include "ei_autre_fonctions.h"

/*********** Paramètres généraux de l'appli ****************/

liste_widgetclass_t *liste_widgetclass;
liste_geometrymanager_t *liste_geometrymanager;
liste_eventtypes_t *liste_events_widgets;

static ei_widget_t *widget_racine;
static ei_surface_t racine_surface;
ei_surface_t pick_surface;

int widget_id = 0;

ei_bool_t is_moving = EI_FALSE;
ei_bool_t is_resizing = EI_FALSE;
ei_bool_t arret_final = EI_FALSE;

ei_linked_rect_t *rect_to_update;

/************************************************************/

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();

    liste_widgetclass = calloc(1, sizeof(liste_widgetclass_t));
    liste_geometrymanager = calloc(1, sizeof(liste_geometrymanager_t));
    liste_events_widgets = calloc(1, sizeof(liste_eventtypes_t));
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
ei_widget_t *ei_app_root_widget();
void ei_app_run()
{
    (ei_app_root_widget())->wclass->geomnotifyfunc(ei_app_root_widget());
    update_surface(rect_to_update, EI_TRUE);

    ei_event_t *event = calloc(1, sizeof(ei_event_t));
    ei_widget_t *pressed_widget = NULL;
    ei_widget_t *released_widget;
    ei_bool_t retour = EI_FALSE;

    while (arret_final == EI_FALSE)
    {
        hw_event_wait_next(event);
        if (event->type < 5)
            retour = recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL, NULL);

        /* Cas où on appuie avec le clic gauche */
        else if (event->type == ei_ev_mouse_buttondown)
        {
            pressed_widget = ei_widget_pick(&event->param.mouse.where);
            retour = recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL);
        }

        /* Cas où on relache le clic gauche */
        else if (event->type == ei_ev_mouse_buttonup && event->param.mouse.button == ei_mouse_button_left)
        {
            released_widget = ei_widget_pick(&event->param.mouse.where);
            retour = recherche_traitants_event(liste_events_widgets, event, EI_TRUE, released_widget, NULL);
            pressed_widget = NULL;
            released_widget = NULL;
        }

        /* Si on ressort du bouton avec le clic appuyé, on redonne la forme normale du potentiel bouton cliqué et inversement */
        else if (event->type == ei_ev_mouse_move)
        {
            if (is_moving || is_resizing)
            {
                recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL);
                rect_to_update = update_surface(rect_to_update, EI_FALSE);
            }
            else if (pressed_widget != NULL && !strcmp(pressed_widget->wclass->name, "button"))
            {
                if (recherche_traitants_event(liste_events_widgets, event, EI_TRUE, pressed_widget, NULL))
                    rect_to_update = update_surface(rect_to_update, EI_FALSE);
            }
        }
        if (retour)
        {
            rect_to_update = update_surface(rect_to_update, EI_TRUE);
            retour = EI_FALSE;
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
        liste_widgetclass_t *next = liste_widgetclass->next;
        free(liste_widgetclass->first_widgetclass);
        free(liste_widgetclass);
        liste_widgetclass = next;
    }

    /* On libère la liste chaînée des geometry manager */
    while (liste_geometrymanager != NULL)
    {
        liste_geometrymanager_t *next = liste_geometrymanager->next;
        free(liste_geometrymanager->geometrymanager_cell);
        free(liste_geometrymanager);
        liste_geometrymanager = next;
    }

    /* On libère la liste chaînée des event types */
    free_liste_eventtypes(liste_events_widgets);

    /* On libère la liste des rectangles à mettre à jour */
    free_linked_rects(rect_to_update);

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

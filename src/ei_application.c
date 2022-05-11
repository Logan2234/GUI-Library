#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "ei_autre_struct.h"
#include "ei_autre_event.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"

struct liste_widgetclass *liste_widgetclass;
struct liste_geometrymanager *liste_geometrymanager;
struct liste_eventtypes_t *liste_events_widgets;
ei_surface_t racine_surface;
ei_surface_t pick_surface;
ei_widget_t *widget_racine;
ei_bool_t arret = EI_FALSE;
uint32_t widget_id = 0;

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();

    liste_widgetclass = calloc(1, sizeof(struct liste_widgetclass));
    liste_geometrymanager = calloc(1, sizeof(struct liste_geometrymanager));
    liste_events_widgets = calloc(1, sizeof(liste_eventtypes_t));

    /* Enregistrement des différentes classes de widget */
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();

    /* Enregistrement des différents gestionnaires de géométrie */
    ei_geometrymanager_register(return_geometry_manager_placer());

    /* Création du widget root et de ses surfaces */
    widget_racine = ei_widget_create("frame", NULL, NULL, NULL);
    racine_surface = hw_create_window(main_window_size, fullscreen);
    pick_surface = hw_surface_create(racine_surface, main_window_size, EI_TRUE);
    ei_frame_configure(widget_racine, NULL, &ei_default_background_color, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void create_close_button_for_each_toplevel(ei_widget_t * widget)
{
    if (!strcmp(widget->wclass->name, "toplevel"))
    {
        ei_widget_t *button = ei_widget_create("button", widget, NULL, NULL);
        ei_button_configure(button, NULL, &close_button_color, &close_button_border_width, &close_button_corner_radius, &close_button_relief, &close_button_text, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        ei_place(button, &close_button_anchor, NULL, NULL, &close_button_width, &close_button_height, &close_button_rel_x, &close_button_rel_y, NULL, NULL);
    }
    if (widget->next_sibling != NULL)
        return create_close_button_for_each_toplevel(widget->next_sibling);
    if (widget->children_head != NULL)
        return create_close_button_for_each_toplevel(widget->children_head);
}

void ei_app_run()
{
    create_close_button_for_each_toplevel(widget_racine);
    hw_surface_lock(racine_surface);
    // hw_surface_lock(pick_surface);
    draw_widgets_and_family(widget_racine);
    hw_surface_unlock(racine_surface);
    // hw_surface_unlock(pick_surface);
    hw_surface_update_rects(racine_surface, NULL);

    struct ei_event_t *event = calloc(1, sizeof(ei_event_t));
    ei_widget_t *pointed_widget;
    ei_widget_t *pressed_widget = NULL;
    ei_widget_t *released_widget;
    while (arret == EI_FALSE) // Comment faire pour annoncer qu'on quit
    {
        hw_event_wait_next(event);
        if (event->type < 5)
            recherche_traitants_event(liste_events_widgets, event, EI_FALSE, NULL);

        /* Cas où on appuie avec le clic gauche */
        else if (event->type == ei_ev_mouse_buttondown && event->param.mouse.button == ei_mouse_button_left)
        {
            pressed_widget = search_widget_by_click(event);
            if (!strcmp(pressed_widget->wclass->name, "button")){
                *((ei_button_t *)pressed_widget)->relief = ei_relief_sunken;
                hw_surface_lock(racine_surface);
                draw_widgets_and_family(widget_racine);
                hw_surface_unlock(racine_surface);
                hw_surface_update_rects(racine_surface, NULL);
            }
            // recherche_traitants_event(liste_widget, event, EI_TRUE, TROUVER LE WIDGETS)
        }
        /* Cas où on relache le clic gauche */
        else if (event->type == ei_ev_mouse_buttonup && event->param.mouse.button == ei_mouse_button_left)
        {
            /* Maintenant on test si on relache le clic sur le même widget que sur celui que l'on vient d'appuyer */
            released_widget = search_widget_by_click(event);
            if (!strcmp(released_widget->wclass->name, "button") && !strcmp(pressed_widget->wclass->name, "button")) 
            {
                /* Si c'est le même on appelle le callback et on redessine le relief*/
                (pressed_widget == released_widget) ? (*((ei_button_t *)released_widget)->callback)(released_widget, event, NULL) : NULL;
                *((ei_button_t *)pressed_widget)->relief = ei_relief_raised;
                hw_surface_lock(racine_surface);
                draw_widgets_and_family(widget_racine);
                hw_surface_unlock(racine_surface);
                hw_surface_update_rects(racine_surface, NULL);
            } 
            pressed_widget = NULL;

        }
        /* Si on ressort du bouton avec le clic appuyé, on redonne la forme normale du potentiel bouton cliqué et inversement */
        else if (pressed_widget != NULL && !strcmp(pressed_widget->wclass->name, "button") && event->type == ei_ev_mouse_move)
        {
            pointed_widget = search_widget_by_click(event);
            *((ei_button_t *)pressed_widget)->relief = (pointed_widget != pressed_widget) ? ei_relief_raised : ei_relief_sunken;
            hw_surface_lock(racine_surface);
            draw_widgets_and_family(widget_racine);
            hw_surface_unlock(racine_surface);
            hw_surface_update_rects(racine_surface, NULL);
        }
    }
    // On doit faire ça ?
    free(event);
    free_liste_eventtypes(liste_events_widgets);
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

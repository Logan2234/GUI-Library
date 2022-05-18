#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"

/* Variables globales à ces tests */

extern liste_widgetclass_t *liste_widgetclass;
extern liste_geometrymanager_t *liste_geometrymanager;
static ei_size_t screen_size;
static ei_color_t root_bgcol;

ei_bool_t process_key(ei_widget_t *widget, ei_event_t *event, void *user_param)
{
    if (event->param.key.key_code == SDLK_ESCAPE)
    {
        ei_app_quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

/*
    Fonction qui va nous servir pour les tests
    Elle permet d'afficher toutes les relations père-fils des widgets
*/
void print_widget_and_family(ei_widget_t *widget)
{
    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        print_widget_and_family(current_widget->next_sibling);

    if (current_widget->children_head != NULL)
        print_widget_and_family(current_widget->children_head);

    if (widget->parent != NULL)
        printf("%s -> %s\n", widget->parent->wclass->name, widget->wclass->name);
    else
        printf("Racine: %s\n", widget->wclass->name);
}

/*
    Test des fonctions:
    - ei_app_create
    - ei_app_run
*/
int test_1()
{
    ei_app_create(screen_size, EI_FALSE);
    ei_app_run();
    ei_app_create(screen_size, EI_TRUE);
    ei_app_run();
    return (EXIT_SUCCESS);
}

/*
    Test des fonctions:
    - ei_widgetclass_register
    - ei_geometrymanager_register
    - ei_app_free pour les listes chaînées des geometry managers et des widget class
*/
int test_2()
{
    printf("\n===== BEFORE CALLING ei_app_create =====\n");
    printf("Widget class: ");
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
    {
        printf("%s -> ", liste_widgetclass->first_widgetclass->name);
        liste_widgetclass = liste_widgetclass->next;
    }
    printf("END\n");
    printf("Geometry manager: ");
    while (liste_geometrymanager != NULL && liste_geometrymanager->geometrymanager_cell != NULL)
    {
        printf("%s -> ", liste_geometrymanager->geometrymanager_cell->name);
        liste_geometrymanager = liste_geometrymanager->next;
    }

    printf("END\n========================================\n\nPress RETURN to continue and register widget class and geometry manager\n");
    getchar();

    ei_app_create(screen_size, EI_FALSE);

    printf("===== AFTER CALLING ei_app_create =====\n");
    printf("Widget class: ");
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
    {
        printf("%s -> ", liste_widgetclass->first_widgetclass->name);
        liste_widgetclass = liste_widgetclass->next;
    }
    printf("END\n");
    printf("Geometry manager: ");
    while (liste_geometrymanager != NULL && liste_geometrymanager->geometrymanager_cell != NULL)
    {
        printf("%s -> ", liste_geometrymanager->geometrymanager_cell->name);
        liste_geometrymanager = liste_geometrymanager->next;
    }

    printf("END\n=======================================\n\nPress RETURN to continue and free widget class and geometry manager\n");
    getchar();

    ei_app_free();

    printf("===== AFTER CALLING ei_app_free =====\n");
    printf("Widget class: ");
    while (liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
    {
        printf("%s -> ", liste_widgetclass->first_widgetclass->name);
        liste_widgetclass = liste_widgetclass->next;
    }
    printf("END\n");
    printf("Geometry manager: ");
    while (liste_geometrymanager != NULL && liste_geometrymanager->geometrymanager_cell != NULL)
    {
        printf("%s -> ", liste_geometrymanager->geometrymanager_cell->name);
        liste_geometrymanager = liste_geometrymanager->next;
    }
    printf("END\n=====================================\n");

    return (EXIT_SUCCESS);
}

/*
    Test des fonctions:
    - ei_app_root_widget
    - ei_frame_configure
    - Affichage du text et d'images dans des frames
*/
int test_3()
{
    root_bgcol = (ei_color_t){0x42, 0xA4, 0xA4, 0xff};

    ei_widget_t *frame, *frame2;
    ei_size_t frame_size = {0, 0};
    ei_size_t frame_size2 = {600, 600};
    int frame_x = 150;
    int frame_y = 200;
    int frame_x2 = 200;
    int frame_y2 = 400;
    ei_color_t frame_color = {0x88, 0x88, 0x88, 0xff};
    ei_relief_t frame_relief = ei_relief_raised;
    int frame_border_width = 6;
    ei_anchor_t ancre = ei_anc_southeast;

    ei_app_create(screen_size, EI_FALSE);

    ei_surface_t *image = hw_image_load("misc/klimt.jpg", ei_app_root_surface());
    char *text = "Coucou Nillllllllllllllllllllllllllllllllllls";
    ei_widget_t *root = ei_app_root_widget();
    
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame, &frame_size, &frame_color, &frame_border_width, &frame_relief, &text, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL);

    frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame2, &frame_size2, &frame_color, &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL, &image, NULL, &ancre);
    printf("%d\n", frame2->requested_size.width);
    ei_place(frame2, NULL, &frame_x2, &frame_y2, NULL, NULL, NULL, NULL, NULL, NULL);
    printf("%d\n", frame2->screen_location.size.width);

    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}

int test_4()
{
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

    ei_widget_t *button, *button2;
    ei_size_t button_size = {200,200};
    ei_size_t button_size2 = {600, 600};
    int button_x = 100;
    int button_y = 100;
    int button_x2 = 300;
    int button_y2 = 300;
    ei_color_t button_color = {0x88, 0x88, 0x88, 0xff};
    char *button_title = "Click !";
    ei_color_t button_text_color = {0x00, 0x00, 0x00, 0xff};
    int button_corner_radius = 20;
    ei_relief_t button_relief = ei_relief_raised;
    int button_border_width = 6;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_surface_t *image = hw_image_load("misc/klimt.jpg", ei_app_root_surface());

    /* Create, configure and place the button on screen. */
    button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(button, &button_size, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
                        NULL, NULL, NULL, NULL, NULL);
    ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL);

    button2 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure(button2, &button_size2, &button_color,
                        &button_border_width, &button_corner_radius, &button_relief, NULL, NULL, NULL, NULL,
                        &image, NULL, NULL, NULL, NULL);
    ei_place(button2, NULL, &button_x2, &button_y2, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Hook the keypress callback to the event. */
    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}

/*
    Test des fonctions:
    - ei_widget_create pour les trois widgets définis
    - ei_app_free avec les widgets
*/
int test_5()
{
    ei_app_create(screen_size, EI_FALSE);

    printf("===== BEFORE CREATING WIDGETS =====\n\n");

    ei_widget_t *root = ei_app_root_widget();

    print_widget_and_family(root);

    printf("\n===== AFTER CREATING WIDGETS =====\n\n");
    /* On créé les widgets comme ci:
                    -> FRAME -> (BUTTON -> TOPLEVEL) & (FRAME)
        FRAME(root) -> BUTTON -> (FRAME) & (BUTTON -> TOPLEVEL)
                    -> TOPLEVEL -> (BUTTON) & (FRAME) & (BUTTON)
    */

    printf("On créé l'arbre de widgets suivant:\n(il faut lire du bas à droite vers le haut à gauche)\n\n\
    \t\t-> FRAME -> BUTTON -> TOPLEVEL\n\
    \t\t\t\t\b\b\b\b\b\b\b-> FRAME\n\n\
    FRAME(root) -> BUTTON -> FRAME\n\
    \t\t\t\t\b\b\b\b\b\b-> BUTTON -> TOPLEVEL\n\n\
    \t\t\t\t\b\b\b\b-> BUTTON\n\
    \t\t-> TOPLEVEL -> FRAME\n\
    \t\t\t\t\b\b\b\b-> BUTTON\n\n");

    printf("Press RETURN to continue create widgets\n");
    getchar();

    ei_widget_t *frame_root = ei_widget_create("frame", root, NULL, NULL);
    ei_widget_t *button_root = ei_widget_create("button", root, NULL, NULL);

    ei_widget_t *button = ei_widget_create("button", frame_root, NULL, NULL);
    ei_widget_t *frame = ei_widget_create("frame", frame_root, NULL, NULL);
    ei_widget_t *toplevel = ei_widget_create("toplevel", button, NULL, NULL);

    ei_widget_t *frame2 = ei_widget_create("frame", button_root, NULL, NULL);
    ei_widget_t *button2 = ei_widget_create("button", button_root, NULL, NULL);
    ei_widget_t *toplevel2 = ei_widget_create("toplevel", button2, NULL, NULL);

    ei_widget_t *toplevel_root = ei_widget_create("toplevel", root, NULL, NULL);
    ei_widget_t *button3 = ei_widget_create("button", toplevel_root, NULL, NULL);
    ei_widget_t *frame3 = ei_widget_create("frame", toplevel_root, NULL, NULL);
    ei_widget_t *button4 = ei_widget_create("button", toplevel_root, NULL, NULL);

    print_widget_and_family(root);

    ei_app_free();

    return (EXIT_SUCCESS);
}

int test_6()
{
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};
    ei_color_t root_bgcol2 = {0xff, 0xff, 0xb4, 0xff};

    ei_widget_t *frame;
    ei_widget_t *frame2;
    ei_size_t frame_size = {500, 500};
    ei_size_t frame_size2 = {100, 100};
    int frame_x = 750;
    int frame_y = 750;
    float frame_x_rel = 0.5;
    float frame_y_rel = 0.5;
    ei_color_t frame_color = {0x88, 0x88, 0x88, 0xff};
    ei_relief_t frame_relief = ei_relief_raised;
    int frame_border_width = 6;
    ei_anchor_t ancre = ei_anc_center;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create, configure and place the frame on screen. */
    frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_frame_configure(frame, &frame_size, &frame_color,
                       &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                       NULL, NULL, NULL);
    ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL);

    frame2 = ei_widget_create("frame", frame, NULL, NULL);
    ei_frame_configure(frame2, &frame_size2, &root_bgcol2,
                       &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                       NULL, NULL, NULL);
    ei_place(frame2, &ancre, NULL, NULL, NULL, NULL, &frame_x_rel, &frame_y_rel, NULL, NULL);
    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}

int test_7()
{
    ei_widget_t *toplevel;
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};
    ei_color_t root_bgcol2 = {0xff, 0xff, 0xb4, 0xff};

    ei_widget_t *frame;
    ei_widget_t *frame2;
    ei_size_t frame_size = {500, 500};
    ei_size_t frame_size2 = {100, 100};
    int frame_x = 250;
    int frame_y = 250;
    int toplevel_x = 200;
    int toplevel_y = 200;
    float frame_x_rel = 0.5;
    float frame_y_rel = 0.5;
    ei_color_t frame_color = {0x88, 0x88, 0x88, 0xff};
    ei_relief_t frame_relief = ei_relief_raised;
    int frame_border_width = 6;
    ei_anchor_t ancre = ei_anc_center;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    int border_width = 0;
    ei_color_t toplevel_bg = {0xff, 0xff, 0xff, 0x60};
    ei_size_t toplevel_size = {300, 300};
    char *title = "Test 7";

    ei_toplevel_configure(toplevel, &toplevel_size, &toplevel_bg, &border_width, &title, NULL, NULL, NULL);
    ei_place(toplevel, NULL, &toplevel_x, &toplevel_y, NULL, NULL, NULL, NULL, NULL, NULL);

    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

    /* Run the application's main loop. */
    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

    /* We just exited from the main loop. Terminate the application (cleanup). */
    ei_app_free();

    return (EXIT_SUCCESS);
}

int test_8()
{
    ei_size_t screen_size = {800, 800};
    ei_color_t root_bgcol = {0x52, 0x7f, 0xb4, 0xff};

    ei_size_t window_size = {400, 400};
    char *window_title = "Hello World";
    ei_color_t window_color = {0xA0, 0xA0, 0xA0, 0xff};
    int window_border_width = 4;
    ei_bool_t closable = EI_TRUE;
    ei_axis_set_t window_resizable = ei_axis_none;
    ei_point_t window_position = {30, 10};

    ei_size_t window_size2 = {200, 200};
    char *window_title2 = "Goodbye World";
    ei_color_t window_color2 = {0xA0, 0x50, 0x50, 0xaa};
    int window_border_width2 = 4;
    ei_bool_t closable2 = EI_TRUE;
    ei_axis_set_t window_resizable2 = ei_anc_none;
    float window_position2_x = 0.2;

    ei_widget_t *toplevel, *toplevel2;

    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    toplevel = ei_widget_create("toplevel", ei_app_root_widget(), NULL, NULL);
    toplevel2 = ei_widget_create("toplevel", toplevel, NULL, NULL);

    ei_toplevel_configure(toplevel, &window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
    ei_toplevel_configure(toplevel2, &window_size2, &window_color2, &window_border_width2, &window_title2, &closable2, &window_resizable2, NULL);

    /*printf("%p \n", toplevel);
    printf("%p \n", toplevel2);*/

    ei_place(toplevel, NULL, &(window_position.x), &(window_position.y), NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place(toplevel2, NULL, NULL, NULL, NULL, NULL, &window_position2_x, &window_position2_x, NULL, NULL);

    ei_bind(ei_ev_keydown, NULL, "all", process_key, NULL);

    ei_app_run();

    ei_unbind(ei_ev_keydown, NULL, "all", process_key, NULL);

    ei_app_free();

    return (EXIT_SUCCESS);
}

/*
 main --
 Main function of the tests.
 */
int main(int argc, char **argv)
{
    screen_size = (ei_size_t){1000, 1000};
    int retour;
    // argv[1] = "test8";
    if (!strcmp(argv[1], "test1"))
        retour = test_1();
    else if (!strcmp(argv[1], "test2"))
        retour = test_2();
    else if (!strcmp(argv[1], "test3"))
        retour = test_3();
    else if (!strcmp(argv[1], "test4"))
        retour = test_4();
    else if (!strcmp(argv[1], "test5"))
        retour = test_5();
    else if (!strcmp(argv[1], "test7"))
        retour = test_7();
    else if (!strcmp(argv[1], "test8"))
        retour = test_8();
    // else if (!strcmp(argv[1], "test9"))
    //     retour = test_9();
    // else if (!strcmp(argv[1], "test10"))
    //     retour = test_10();
    else
        printf("\n>>> ERROR! Correct format: ./cmake/nos_tests test[1-10]\n");
    return (EXIT_FAILURE);

    return retour;
}

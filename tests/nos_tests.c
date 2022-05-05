#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_autre_struct.h"


/*
 * main --
 *
 *	Main function of the tests.
 */
int main(int argc, char** argv)
{
    extern struct liste_widgetclass *liste_widgetclass;
    extern struct liste_geometrymanager *liste_geometrymanager;
    
    ei_size_t screen_size = {600, 600};
    
    /* 
    Test basique avec affichage d'une fenêtre vide de taille 600x600
    puis en plein écran
    */
    int test_1(){
        ei_app_create(screen_size, EI_FALSE);
    	ei_app_run();
        ei_app_create(screen_size, EI_TRUE);
    	ei_app_run();
        return (EXIT_SUCCESS);
    }

    /* Test pour savoir si tout les widgets et geometry manager sont enregistrés */
    int test_2(){
        printf("===== AVANT APPEL DE ei_app_create =====\n");
        printf("Widget class: ");
        while(liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
        {
            printf("%s -> ", liste_widgetclass->first_widgetclass->name);
            liste_widgetclass = liste_widgetclass->next;
        }
        printf("\n");
        printf("Geometry manager: ");
        while(liste_geometrymanager != NULL && liste_geometrymanager->geometrymanager_cell != NULL)
        {
            printf("%s -> ", liste_geometrymanager->geometrymanager_cell->name);
            liste_geometrymanager = liste_geometrymanager->next;
        }

        ei_app_create(screen_size, EI_FALSE);

        printf("\n\n===== APRÈS APPEL DE ei_app_create =====\n");
        printf("Widget class: ");
        while(liste_widgetclass != NULL && liste_widgetclass->first_widgetclass != NULL)
        {
            printf("%s -> ", liste_widgetclass->first_widgetclass->name);
            liste_widgetclass = liste_widgetclass->next;
        }
        printf("\n");
        printf("Geometry manager: ");
        while(liste_geometrymanager != NULL && liste_geometrymanager->geometrymanager_cell != NULL)
        {
            printf("%s -> ", liste_geometrymanager->geometrymanager_cell->name);
            liste_geometrymanager = liste_geometrymanager->next;
        }
        return (EXIT_SUCCESS);
    }

    int test_3(){
        ei_app_create(screen_size, EI_FALSE);
        // ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    	ei_app_run();
        return (EXIT_SUCCESS);
    }


    int test_10(){
        ei_size_t screen_size = {600, 600};
        ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

        ei_widget_t*	frame;
        ei_size_t	frame_size		= {300,200};
        int		frame_x			= 150;
        int		frame_y			= 200;
        ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
        ei_relief_t	frame_relief		= ei_relief_raised;
        int		frame_border_width	= 6;

        /* Create the application and change the color of the background. */
        ei_app_create(screen_size, EI_FALSE);
        ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        /* Create, configure and place the frame on screen. */
        frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
        ei_frame_configure(frame, &frame_size, &frame_color,
                    &frame_border_width, &frame_relief, NULL, NULL, NULL, NULL,
                    NULL, NULL, NULL);
        // ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );

        /* Run the application's main loop. */
        ei_app_run();

        /* We just exited from the main loop. Terminate the application (cleanup). */
        ei_app_free();
        return 1;
    }

    int retour;

    if (!strcmp(argv[1], "test1")) retour = test_1();
    else if (!strcmp(argv[1], "test2")) retour = test_2();
    // else if (!strcmp(argv[1], "test3")) retour = test_3();
    // else if (!strcmp(argv[1], "test4")) retour = test_4();
    // else if (!strcmp(argv[1], "test5")) retour = test_5();
    // else if (!strcmp(argv[1], "test6")) retour = test_6();
    // else if (!strcmp(argv[1], "test7")) retour = test_7();
    // else if (!strcmp(argv[1], "test8")) retour = test_8();
    // else if (!strcmp(argv[1], "test9")) retour = test_9();
    // else if (!strcmp(argv[1], "test10")) retour = test_10();
    else return (EXIT_FAILURE);

	return retour;
}

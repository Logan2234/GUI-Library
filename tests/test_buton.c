#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

int main(int argc, char** argv)
{
    ei_size_t	screen_size		= {600, 600};
    ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

    ei_widget_t*	button;
    ei_size_t	button_size		= {300,200};
    int		button_x		= 150;
    int		button_y		= 200;
    ei_color_t	button_color		= {0x88, 0x88, 0x88, 0xff};
    char*		button_title		= "Mon premier Bouton !";
    ei_color_t	button_text_color	= {0x00, 0x00, 0x00, 0xff};
    int		button_corner_radius	= 20;
    ei_relief_t	button_relief		= ei_relief_raised;
    int		button_border_width	= 6;

    /* Create the application and change the color of the background. */
    ei_app_create(screen_size, EI_FALSE);
    ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create, configure and place the button on screen. */
    button = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_button_configure	(button, &button_size, &button_color,
                            &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
                            NULL, NULL, NULL, NULL, NULL);
    ei_place(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    /* Hook the keypress callback to the event. */


    /* Run the application's main loop. */
    ei_app_run();

    /* We just exited from the main loop. Terminate the application (cleanup). */

    ei_app_free();

    return (EXIT_SUCCESS);
}
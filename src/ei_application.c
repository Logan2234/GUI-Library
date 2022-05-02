#include "ei_application.h"
#include "ei_geometrymanager.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();
    // ei_widgetclass_register(widget_class); A faire pour toutes les classes de widget
    // ei_geometrymanager_register();
    const ei_surface_t racine = hw_create_window(main_window_size, fullscreen);
    ei_surface_t offscreen = hw_surface_create(racine, main_window_size, EI_TRUE);
}
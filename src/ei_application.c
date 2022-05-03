#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_autre_frame.h"

void ei_app_create(ei_size_t main_window_size, ei_bool_t fullscreen)
{
    hw_init();
    ei_widgetclass_register(return_class_frame());
    // ei_geometrymanager_register();
    const ei_surface_t racine = hw_create_window(main_window_size, fullscreen);
    ei_surface_t offscreen = hw_surface_create(racine, main_window_size, EI_TRUE);
}

void ei_app_run()
{
    getchar();
}

void ei_app_free()
{
    
}

ei_widget_t* ei_app_root_widget(void)
{
    return NULL;
}
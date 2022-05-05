#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"

/* Everything about frame */
typedef struct ei_frame_t
{
    ei_widget_t widget;
    ei_size_t *requested_size;
    const ei_color_t *color;
    int *border_width;
    ei_relief_t *relief;
    char **text;
    ei_font_t *text_font;
    ei_color_t *text_color;
    ei_anchor_t *text_anchor;
    ei_surface_t *img;
    ei_rect_t **img_rect;
    ei_anchor_t *img_anchor;
} ei_frame_t;

ei_widgetclass_t *return_class_frame(); /* Fonction de retour de la class frame */

/* Everything about button */
typedef struct ei_button_t
{
    ei_widget_t *widget;
    ei_size_t *requested_size;
    const ei_color_t *color;
    int *border_width;
    int *corner_radius;
    ei_relief_t *relief;
    char **text;
    ei_font_t *text_font;
    ei_color_t *text_color;
    ei_anchor_t *text_anchor;
    ei_surface_t *img;
    ei_rect_t **img_rect;
    ei_anchor_t *img_anchor;
    ei_callback_t *callback;
    void **user_param;
} ei_button_t;

ei_widgetclass_t *return_class_button(); /* Fonction de retour de la class button */

/* Everything about widget in general */
struct liste_widgetclass
{
    ei_widgetclass_t *first_widgetclass;
    struct liste_widgetclass *next;
};

/* Everything about geometry manager in general */
struct liste_geometrymanager
{
    ei_geometrymanager_t *geometrymanager_cell;
    struct liste_geometrymanager *next;
};

ei_geometrymanager_t *return_geometry_manager_placer();

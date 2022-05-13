#include "ei_widget.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"

/* Everything about frame */
typedef struct ei_frame_t
{
    ei_widget_t widget;
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
    ei_widget_t widget;
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

/* Liste chainée liant widgets, events et callback */
typedef struct liste_events_widgets
{
    ei_widget_t *widget;
    ei_eventtype_t eventtype;
    ei_callback_t callback;
    void *user_param;
    ei_tag_t tag;
    struct liste_events_widgets* next;
} liste_events;

/* Liste chainée de eventype menant vers de liste_events_widgets */
typedef struct liste_eventtypes_t
{
    ei_eventtype_t eventtype;
    struct liste_events_widgets* liste;
    struct liste_eventtypes_t* next;
} liste_eventtypes_t;

/* Extension 1 : radiobutton */
typedef struct ei_radiobutton_t
{
    ei_widget_t widget;
    const ei_color_t *color;
    int *border_width;
    ei_relief_t *relief;
    struct liste_textes *textes;
    ei_font_t *text_font;
    ei_color_t *text_color;
    ei_anchor_t *text_anchor;
    ei_surface_t *img;
    ei_rect_t **img_rect;
    ei_anchor_t *img_anchor;
    ei_callback_t *callback;  // Il faudrait faire une liste de callbacks ?
    void **user_param;
} ei_radiobutton_t;

ei_widgetclass_t *return_class_radiobutton(); /* Fonction de retour de la class radiobutton */

/* Everything about toplevel */
typedef struct ei_toplevel_t
{
    ei_widget_t widget;
    ei_color_t *color;
    int *border_width;
    char **title;
    ei_bool_t *closable;
    ei_axis_set_t *resizable;
    ei_size_t **min_size;
} ei_toplevel_t;

ei_widgetclass_t *return_class_toplevel(); /* Fonction de retour de la class toplevel */

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

struct liste_textes
{
    char **text;
    struct liste_textes *next;
};

#include "ei_event.h"
#include "ei_geometrymanager.h"
/**
 * @file ei_autre_struct.h
 * @brief Définit les extensions des types des widgets pour ensuite faire du polymorphisme.
 * 
 *
 */

/**
 * @brief Définition du type frame.
 * 
*/
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

/**
 * @brief Définition du type button.
 * 
*/
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

/**
 * @brief Définition du type toplevel.
 * 
*/
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

/**
 * @brief Liste chaînée des widgetclass enregistrées.
 **/
typedef struct liste_widgetclass
{
    ei_widgetclass_t *first_widgetclass;
    struct liste_widgetclass *next;
} liste_widgetclass_t;

/**
 * @brief Liste chaînée des gestionaires de géométrie enregistrés.
 **/
typedef struct liste_geometrymanager
{
    ei_geometrymanager_t *geometrymanager_cell;
    struct liste_geometrymanager *next;
} liste_geometrymanager_t;

/**
 * @brief Deéfinition du struct du placer 
 **/
typedef struct ei_placer_t
{
    ei_geometry_param_t manager;
    ei_anchor_t *anchor;
    int x;
    int y;
    float rel_x;
    float rel_y;
    int width;
    int height;
    float rel_width;
    float rel_height;
} ei_placer_t;

/**
 * @brief Liste chainée liant widgets, tags, events et callback 
 **/
typedef struct liste_events_widgets_t
{
    ei_widget_t *widget;
    ei_eventtype_t eventtype;
    ei_callback_t callback;
    void *user_param;
    ei_tag_t tag;
    struct liste_events_widgets_t *next;
} liste_events;

/**
 * @brief Liste chainée des eventype menant vers une liste_events 
 *
 */
typedef struct liste_eventtypes_t
{
    ei_eventtype_t eventtype;
    struct liste_events_widgets_t *liste;
    struct liste_eventtypes_t *next;
} liste_eventtypes_t;

/**
 * @brief  Extension 1 : radiobutton 
 **/
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
    ei_callback_t *callback;
    void **user_param;
} ei_radiobutton_t;

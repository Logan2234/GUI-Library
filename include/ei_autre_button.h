#ifndef PROJET_C_EI_AUTRE_BUTTON_H
#define PROJET_C_EI_AUTRE_BUTTON_H

#endif //PROJET_C_EI_AUTRE_BUTTON_H

#include "ei_widgetclass.h"
#include "ei_widget.h"

typedef struct ei_button_t {
    ei_widget_t*		widget;
    ei_size_t*		requested_size;
    const ei_color_t*	color;
    int*			border_width;
    int*			corner_radius;
    ei_relief_t*		relief;
    char**			text;
    ei_font_t*		text_font;
    ei_color_t*		text_color;
    ei_anchor_t*		text_anchor;
    ei_surface_t*		img;
    ei_rect_t**		img_rect;
    ei_anchor_t*		img_anchor;
    ei_callback_t*		callback;
    void**			user_param;
} ei_button_t;

struct ei_widget_t *button_allocfunc(void);

void button_releasefunc(struct ei_widget_t *widget);

void button_drawfunc(struct ei_widget_t *widget,
                    ei_surface_t surface,
                    ei_surface_t pick_surface,
                    ei_rect_t *clipper);

void button_setdefaultsfunc(struct ei_widget_t *widget);

void button_geomnotifyfunc(struct ei_widget_t *widget);

ei_widgetclass_t *return_class_button();
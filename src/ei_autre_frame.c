#include "ei_autre_frame.h"


struct ei_widget_t widget_frame;

widget_frame.wclass = widgetclass_frame;
widget_frame.pick_id = 0;
widget_frame.ei_color_t = NULL;
widget_frame.user_data = NULL;
widget_frame.destructor = NULL; /* Il faut créer la fonction */
widget_frame.parent = NULL;
widget_frame.children_head = NULL;
widget_frame.children_tail = NULL;
widget_frame.next_sibling = NULL;
widget_frame.geom_params = NULL;
ei_size_t taille;
taille.width = 600;
taille.height = 600;
widget_frame.requested_size = taille;
ei_rect_t
widget_frame.screen_location = NULL;
widget_frame.content_rect = NULL;

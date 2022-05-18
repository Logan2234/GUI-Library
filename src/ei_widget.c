#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"

extern liste_widgetclass_t *liste_widgetclass;
extern liste_geometrymanager_t *liste_geometrymanager;
extern ei_surface_t pick_surface;
extern int widget_id;

static void ei_kill_widget(ei_widget_t *widget, ei_widget_t *origin)
{
	ei_widget_t *pere = origin->parent;

	if (widget->children_head != NULL)
		ei_kill_widget(widget->children_head, origin);

	if (widget != origin && widget->next_sibling != NULL)
		ei_kill_widget(widget->next_sibling, origin);
	if (widget == origin && widget != ei_app_root_widget())
	{
		ei_widget_t *sent = origin->parent->children_head;

		(sent->pick_id == origin->pick_id) ? origin->parent->children_head = origin->parent->children_head->next_sibling : NULL;
		while (sent->next_sibling != NULL && sent->next_sibling->pick_id != origin->pick_id)
			sent = sent->next_sibling;

		(sent->next_sibling != NULL) ? sent->next_sibling = sent->next_sibling->next_sibling : NULL;
	}

	(widget->destructor != NULL) ? widget->destructor(widget) : NULL;
	widget->wclass->releasefunc(widget);
}

/**
 * @brief	Destroys a widget.
 * 		Removes the widget from the screen if it is currently displayed.
 * 		Destroys all its descendants.
 *		Calls its destructor if it was provided.
 * 		Frees the memory used by the widget (e.g. the widget param).
 *
 * @param	widget		The widget that is to be destroyed.
 */
void ei_widget_destroy(ei_widget_t *widget)
{
	ei_kill_widget(widget, widget);
}

/**
 * @brief	Returns the widget that is at a given location on screen.
 *
 * @param	where		The location on screen, expressed in the root window coordinates.
 *
 * @return			The top-most widget at this location, or NULL if there is no widget
 *				at this location (except for the root widget).
 */
ei_widget_t *ei_widget_pick(ei_point_t *where)
{
	uint32_t *picking_color_entier = (uint32_t *)hw_surface_get_buffer(pick_surface);
	picking_color_entier += where->x + where->y * hw_surface_get_size(pick_surface).width;
	return search_widget_by_id(ei_app_root_widget(), *picking_color_entier);
}

/**
 * @brief	Creates a new instance of a widget of some particular class, as a descendant of
 *		an existing widget.
 *
 *		The widget is not displayed on screen until it is managed by a geometry manager.
 *		When no more needed, the widget must be released by calling \ref ei_widget_destroy.
 *
 * @param	class_name	The name of the class of the widget that is to be created.
 * @param	parent 		A pointer to the parent widget. Can not be NULL.
 * @param	user_data	A pointer provided by the programmer for private use. May be NULL.
 * @param	destructor	A pointer to a function to call before destroying a widget structure. May be NULL.
 *
 * @return			The newly created widget, or NULL if there was an error.
 */
ei_widget_t *ei_widget_create(ei_widgetclass_name_t class_name, ei_widget_t *parent, void *user_data, ei_widget_destructor_t destructor)
{
	ei_widgetclass_t *widgetclass = ei_widgetclass_from_name(class_name);

	if (widgetclass == NULL)
		return NULL;

	ei_widget_t *widget = widgetclass->allocfunc();

	widget_id++;

	/* Puis on rentre les paramètres fournis en paramètre de ei_widget_create */
	widget->wclass = widgetclass;
	widget->wclass->setdefaultsfunc(widget); /* Notre nouveau widget prend les paramètres par défaut */
	widget->parent = parent;
	widget->user_data = user_data;
	widget->destructor = destructor;

	/* Il ne faut pas oublier de dire au parent qu'il a un nouveau fils si jamais c'est pas la racine */
	(parent != NULL) ? ajout_relation_parent(parent, widget) : NULL;
	return widget;
}

/**
 * @brief	Configures the attributes of widgets of the class "frame".
 *
 *		Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
 *		been defined before, then a default value should be used (default values are
 *		specified for each parameter). If the parameter is "NULL" but was defined on a
 *		previous call, then its value must not be changed.
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The size requested for this widget, including the widget's borders.
 *				The geometry manager may override this size due to other constraints.
 *				Defaults to the "natural size" of the widget, ie. big enough to
 *				display the border and the text or the image. This may be (0, 0)
 *				if the widget has border_width=0, and no text and no image.
 * @param	color		The color of the background of the widget. Defaults to
 *				\ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border decoration of the widget. The final
 *				appearance depends on the "relief" parameter. Defaults to 0.
 * @param	relief		Appearance of the border of the widget. Defaults to
 *				\ref ei_relief_none.
 * @param	text		The text to display in the widget, or NULL. Only one of the
 *				parameter "text" and "img" should be used (i.e. non-NULL). Defaults
 *				to NULL.
 * @param	text_font	The font used to display the text. Defaults to \ref ei_default_font.
 * @param	text_color	The color used to display the text. Defaults to
 *				\ref ei_font_default_color.
 * @param	text_anchor	The anchor of the text, i.e. where it is placed within the widget.
 *				Defines both the anchoring point on the parent and on the widget.
 *				Defaults to \ref ei_anc_center.
 * @param	img		The image to display in the widget, or NULL. Any surface can be
 *				used, but usually a surface returned by \ref hw_image_load. Only one
 *				of the parameter "text" and "img" should be used (i.e. non-NULL).
				Defaults to NULL.
 * @param	img_rect	If not NULL, this rectangle defines a subpart of "img" to use as the
 *				image displayed in the widget. Defaults to NULL.
 * @param	img_anchor	The anchor of the image, i.e. where it is placed within the widget
 *				when the size of the widget is bigger than the size of the image.
 *				Defaults to \ref ei_anc_center.
 */
void ei_frame_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color, int *border_width,
						ei_relief_t *relief, char **text, ei_font_t *text_font, ei_color_t *text_color,
						ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect, ei_anchor_t *img_anchor)
{
	ei_frame_t *frame = (ei_frame_t *)widget;

	ei_color_t *color_frame = calloc(1, sizeof(ei_color_t));
	int *border_width_frame = calloc(1, sizeof(int));
	ei_relief_t *relief_frame = calloc(1, sizeof(ei_relief_t));
	char **text_frame = calloc(1, sizeof(char *));
	char *text_frame_in;
	ei_font_t *text_font_frame = calloc(1, sizeof(ei_font_t));
	ei_color_t *text_color_frame = calloc(1, sizeof(ei_color_t));
	ei_anchor_t *text_anchor_frame = calloc(1, sizeof(ei_anchor_t));
	ei_surface_t *img_frame = calloc(1, sizeof(ei_surface_t));
	ei_rect_t **img_rect_frame = calloc(1, sizeof(ei_rect_t *));
	ei_rect_t *img_rect_frame_in = calloc(1, sizeof(ei_rect_t));
	ei_anchor_t *img_anchor_frame = calloc(1, sizeof(ei_anchor_t));

	*color_frame = (color != NULL) ? *color : (frame->color == NULL) ? ei_default_background_color
																	 : *frame->color;
	*border_width_frame = (border_width != NULL) ? *border_width : (frame->border_width == NULL) ? 0
																								 : *frame->border_width;
	*relief_frame = (relief != NULL) ? *relief : (frame->relief == NULL) ? default_relief_frame
																		 : *frame->relief;
	text_frame_in = (text != NULL) ? *text : (frame->text != NULL) ? *frame->text
																   : NULL;
	*text_font_frame = (text_font != NULL) ? *text_font : (frame->text_font == NULL) ? ei_default_font
																					 : *frame->text_font;
	*text_color_frame = (text_color != NULL) ? *text_color : (frame->text_color == NULL) ? (ei_color_t)ei_font_default_color
																						 : *frame->text_color;
	*text_anchor_frame = (text_anchor != NULL) ? *text_anchor : (frame->text_anchor == NULL) ? default_anchor_frame
																							 : *frame->text_anchor;

	if (img != NULL)
		*img_frame = *img;
	else
		img_frame = NULL;

	if (img_rect != NULL)
	{
		*img_rect_frame_in = **img_rect;
		*img_rect_frame = img_rect_frame_in;
	}
	else
		*img_rect_frame = NULL;

	*img_anchor_frame = (img_anchor != NULL) ? *img_anchor : (frame->img_anchor == NULL) ? default_anchor_frame
																						 : *frame->img_anchor;
	*text_frame = text_frame_in;

	widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;

	frame->color = color_frame;
	frame->border_width = border_width_frame;
	frame->relief = relief_frame;
	frame->text = text_frame;
	frame->text_font = text_font_frame;
	frame->text_color = text_color_frame;
	frame->text_anchor = text_anchor_frame;
	frame->img = img_frame;
	frame->img_rect = img_rect_frame;
	frame->img_anchor = img_anchor_frame;
}

/**
 * @brief	Configures the attributes of widgets of the class "button".
 *
 * @param	widget, requested_size, color, border_width, relief,
 *		text, text_font, text_color, text_anchor,
 *		img, img_rect, img_anchor
 *				See the parameter definition of \ref ei_frame_configure. The only
 *				difference is that relief defaults to \ref ei_relief_raised
 *				and border_width defaults to \ref k_default_button_border_width.
 * @param	corner_radius	The radius (in pixels) of the rounded corners of the button.
 *				0 means straight corners. Defaults to \ref k_default_button_corner_radius.
 * @param	callback	The callback function to call when the user clicks on the button.
 *				Defaults to NULL (no callback).
 * @param	user_param	A programmer supplied parameter that will be passed to the callback
 *				when called. Defaults to NULL.
 */
void ei_button_configure(ei_widget_t *widget, ei_size_t *requested_size, const ei_color_t *color,
						 int *border_width, int *corner_radius, ei_relief_t *relief,
						 char **text, ei_font_t *text_font, ei_color_t *text_color,
						 ei_anchor_t *text_anchor, ei_surface_t *img, ei_rect_t **img_rect,
						 ei_anchor_t *img_anchor, ei_callback_t *callback, void **user_param)
{
	ei_button_t *bouton = (ei_button_t *)widget;

	ei_color_t *color_button = calloc(1, sizeof(ei_color_t));
	int *border_width_button = calloc(1, sizeof(int));
	int *corner_radius_button = calloc(1, sizeof(int));
	ei_relief_t *relief_button = calloc(1, sizeof(ei_relief_t));
	char **text_button = calloc(1, sizeof(char *));
	char *text_button_in = calloc(1, sizeof(char));
	ei_font_t *text_font_button = calloc(1, sizeof(ei_font_t));
	ei_color_t *text_color_button = calloc(1, sizeof(ei_color_t));
	ei_anchor_t *text_anchor_button = calloc(1, sizeof(ei_anchor_t));
	ei_surface_t *img_button = calloc(1, sizeof(ei_surface_t));

	ei_anchor_t *img_anchor_button = calloc(1, sizeof(ei_anchor_t));
	ei_rect_t **img_rect_button = calloc(1, sizeof(ei_rect_t *));
	ei_rect_t *img_rect_button_in = calloc(1, sizeof(ei_rect_t));
	ei_callback_t *callback_button = calloc(1, sizeof(ei_callback_t));
	void **user_param_button = calloc(1, sizeof(void *));
	void *user_param_button_in = calloc(1, sizeof(void));

	*color_button = (color != NULL) ? *color : (bouton->color == NULL) ? ei_default_background_color
																	   : *bouton->color;
	*relief_button = (relief != NULL) ? *relief : (bouton->relief == NULL) ? default_relief_button
																		   : *bouton->relief;
	*border_width_button = (border_width != NULL) ? *border_width : (bouton->border_width == NULL) ? (int)k_default_button_border_width
																								   : *bouton->border_width;
	*corner_radius_button = (corner_radius != NULL) ? *corner_radius : (bouton->corner_radius == NULL) ? (int)k_default_button_corner_radius
																									   : *bouton->corner_radius;

	text_button_in = (text != NULL) ? *text : (bouton->text != NULL) ? *bouton->text
																	 : NULL;
	*text_button = text_button_in;

	*text_font_button = (text_font != NULL) ? *text_font : (bouton->text_font == NULL) ? ei_default_font
																					   : *bouton->text_font;
	*text_color_button = (text_color != NULL) ? *text_color : (bouton->text_color == NULL) ? (ei_color_t)ei_font_default_color
																						   : *bouton->text_color;
	*text_anchor_button = (text_anchor != NULL) ? *text_anchor : (bouton->text_anchor == NULL) ? default_anchor_button
																							   : *bouton->text_anchor;

	if (img != NULL)
	{
		*img_button = *img;
	}
	else
		img_button = NULL;

	if (img_rect != NULL)
	{
		*img_rect_button_in = **img_rect;
		*img_rect_button = img_rect_button_in;
	}
	else
		*img_rect_button = NULL;

	*img_anchor_button = (img_anchor != NULL) ? *img_anchor : (bouton->img_anchor == NULL) ? default_anchor_button
																						   : *bouton->img_anchor;
	*callback_button = (callback != NULL) ? *callback : (bouton->callback != NULL) ? *bouton->callback
																				   : NULL;

	user_param_button_in = (user_param != NULL) ? *user_param : (bouton->user_param != NULL) ? *bouton->user_param
																							 : NULL;
	*user_param_button = user_param_button_in;

	widget->requested_size = (requested_size != NULL) ? (*requested_size) : widget->requested_size;

	bouton->color = color_button;
	bouton->border_width = border_width_button;
	bouton->corner_radius = corner_radius_button;
	bouton->relief = relief_button;
	bouton->text = text_button;
	bouton->text_font = text_font_button;
	bouton->text_color = text_color_button;
	bouton->text_anchor = text_anchor_button;
	bouton->img = img_button;
	bouton->img_rect = img_rect_button;
	bouton->img_anchor = img_anchor_button;
	bouton->callback = callback_button;
	bouton->user_param = user_param_button;
}

/**
 * @brief	Configures the attributes of widgets of the class "toplevel".
 *
 * @param	widget		The widget to configure.
 * @param	requested_size	The content size requested for this widget, this does not include
 *				the decorations	(border, title bar). The geometry manager may
 *				override this size due to other constraints.
 *				Defaults to (320x240).
 * @param	color		The color of the background of the content of the widget. Defaults
 *				to \ref ei_default_background_color.
 * @param	border_width	The width in pixel of the border of the widget. Defaults to 4.
 * @param	title		The string title displayed in the title bar. Defaults to "Toplevel".
 *				Uses the font \ref ei_default_font.
 * @param	closable	If true, the toplevel is closable by the user, the toplevel must
 *				show a close button in its title bar. Defaults to \ref EI_TRUE.
 * @param	resizable	Defines if the widget can be resized horizontally and/or vertically
 *				by the user. Defaults to \ref ei_axis_both.
 * @param	min_size	For resizable widgets, defines the minimum size of its content.
 *				The default minimum size of a toplevel is (160, 120).
 *				If *min_size is NULL, this requires the min_size to be configured to
 *				the default size.
 */
void ei_toplevel_configure(ei_widget_t *widget, ei_size_t *requested_size, ei_color_t *color, int *border_width,
						   char **title, ei_bool_t *closable, ei_axis_set_t *resizable, ei_size_t **min_size)
{
	ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

	ei_color_t *color_toplevel = calloc(1, sizeof(ei_color_t));
	int *border_width_toplevel = calloc(1, sizeof(int));
	ei_bool_t *closable_toplevel = calloc(1, sizeof(ei_bool_t));
	ei_axis_set_t *resizable_toplevel = calloc(1, sizeof(ei_axis_set_t));
	char **titre_toplevel = calloc(1, sizeof(char *));
	char *titre_toplevel_in = calloc(1, sizeof(char));
	ei_size_t **min_size_toplevel = calloc(1, sizeof(ei_size_t *));
	ei_size_t *min_size_toplevel_in = calloc(1, sizeof(ei_size_t *));

	*color_toplevel = (color != NULL) ? *color : (toplevel->color == NULL) ? ei_default_background_color
																		   : *toplevel->color;
	*border_width_toplevel = (border_width != NULL) ? *border_width : (toplevel->border_width == NULL) ? 4
																									   : *toplevel->border_width;
	titre_toplevel_in = (title != NULL) ? *title : (toplevel->title != NULL) ? *toplevel->title
																			 : NULL;
	*closable_toplevel = (closable != NULL) ? *closable : (toplevel->closable == NULL) ? EI_TRUE
																					   : *toplevel->closable;
	*resizable_toplevel = (resizable != NULL) ? *resizable : (toplevel->resizable == NULL) ? ei_axis_both
																						   : *toplevel->resizable;
	min_size_toplevel_in = (min_size != NULL) ? *min_size : (toplevel->min_size == NULL) ? default_toplevel_min_size
																						 : *toplevel->min_size;

	*titre_toplevel = titre_toplevel_in;
	*min_size_toplevel = min_size_toplevel_in;

	widget->requested_size = (requested_size != NULL) ? (*requested_size) : (&widget->requested_size == NULL) ? default_toplevel_size
																											  : widget->requested_size;
	toplevel->color = color_toplevel;
	toplevel->border_width = border_width_toplevel;
	toplevel->title = titre_toplevel;
	toplevel->closable = closable_toplevel;
	toplevel->resizable = resizable_toplevel;
	toplevel->min_size = min_size_toplevel;
}
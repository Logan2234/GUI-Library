#include "ei_autre_struct.h"
#include "ei_application.h"
#include "ei_autre_global_var.h"
#include "ei_autre_fonctions.h"

extern liste_geometrymanager_t *liste_geometrymanager;

/**
 * \brief	Registers a geometry manager to the program so that it can be called to manager
 *		widgets. This must be done only once in the application.
 *
 * @param	geometrymanager		The structure describing the geometry manager.
 */
void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
	if (liste_geometrymanager->geometrymanager_cell == NULL)
		liste_geometrymanager->geometrymanager_cell = geometrymanager;

	else
	{
		liste_geometrymanager_t *sent = liste_geometrymanager;
		while (sent->next != NULL)
			sent = sent->next;

		liste_geometrymanager_t *new_cell = calloc(1, sizeof(liste_geometrymanager_t));
		new_cell->geometrymanager_cell = geometrymanager;
		sent->next = new_cell;
	}
}

/**
 * \brief	Tell the geometry manager in charge of a widget to forget it. This removes the
 *		widget from the screen. If the widget is not currently managed, this function
 *		returns silently.
 *		Side effects:
 *		<ul>
 *			<li> the \ref ei_geometrymanager_releasefunc_t of the geometry manager in
 *				charge of this widget is called, </li>
 *			<li> the geom_param field of the widget is freed, </li>
 *			<li> the current screen_location of the widget is invalided (which will
 *				trigger a redraw), </li>
 *			<li> the screen_location of the widget is reset to 0. </li>
 *		</ul>
 *
 * @param	widget		The widget to unmap from the screen.
 */
void ei_geometrymanager_unmap(ei_widget_t *widget)
{
	widget->geom_params->manager->releasefunc(widget);
	widget->geom_params = NULL;
	ei_app_invalidate_rect(&widget->screen_location);
	widget->screen_location = ((!strcmp(widget->wclass->name, "frame"))	   ? (ei_rect_t){0, 0, default_frame_size}
							   : (!strcmp(widget->wclass->name, "button")) ? (ei_rect_t){0, 0, default_button_size}
																		   : (ei_rect_t){0, 0, default_toplevel_size});
}

/**
 * \brief	Returns a geometry manager structure from its name.
 *
 * @param	name		The name of the geometry manager.
 *
 * @return			The structure describing the geometry manager.
 */
ei_geometrymanager_t *ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{
	liste_geometrymanager_t *sent = liste_geometrymanager;
	while (strcmp(sent->geometrymanager_cell->name, name) && sent->next != NULL)
		sent = sent->next;

	if (!strcmp(sent->geometrymanager_cell->name, name))
		return sent->geometrymanager_cell;
}

/**
 * \brief	Registers the "placer" geometry manager in the program. This must be called only
 *		once before the \ref ei_place function can be called.
 */
void ei_register_placer_manager(void)
{
	ei_geometrymanager_t *placer = return_geometry_manager_placer();
	ei_geometrymanager_register(placer);
}

/**
 * \brief
 Configure la géométrie d'un widget à l'aide du gestionnaire de géométrie "placer".
 * Si le widget était déjà géré par un autre gestionnaire de géométrie, alors il est d'abord
 * supprimé du gestionnaire de géométrie précédent.
 * Si le widget était déjà géré par le "placer", alors cela appelle simplement les mises à jour
 * les paramètres placer : les arguments non NULL remplacent les valeurs précédentes.
 * Lorsque les arguments sont passés comme NULL, le placer utilise les valeurs par défaut (détaillées dans
 * les descriptions des arguments ci-dessous). Si aucune taille n'est fournie (absolue ou
 * relatif), alors la taille demandée du widget est utilisée, c'est-à-dire la taille minimale
 * requis pour afficher son contenu.
 *
 * @param	widget		The widget to place.
 * @param	anchor		How to anchor the widget to the position defined by the placer
 *				(defaults to ei_anc_northwest).
 * @param	x		The absolute x position of the widget (defaults to 0) in the
 *				parent's reference.
 * @param	y		The absolute y position of the widget (defaults to 0) in the
 *				parent's reference.
 * @param	width		The absolute width for the widget (defaults to the requested width
 *				of the widget if rel_width is NULL, or 0 otherwise).
 * @param	height		The absolute height for the widget (defaults to the requested height
 *				of the widget if rel_height is NULL, or 0 otherwise).
 * @param	rel_x		The relative x position of the widget: 0.0 corresponds to the left
 *				side of the parent, 1.0 to the right side (defaults to 0.0).
 * @param	rel_y		The relative y position of the widget: 0.0 corresponds to the top
 *				side of the parent, 1.0 to the bottom side (defaults to 0.0).
 * @param	rel_width	The relative width of the widget: 0.0 corresponds to a width of 0,
 *				1.0 to the width of the parent (defaults to 0.0).
 * @param	rel_height	The relative height of the widget: 0.0 corresponds to a height of 0,
 *				1.0 to the height of the parent (defaults to 0.0).
 */
void ei_place(ei_widget_t *widget, ei_anchor_t *anchor, int *x, int *y, int *width,
			  int *height, float *rel_x, float *rel_y, float *rel_width, float *rel_height)
{
	int width_parent = widget->requested_size.width;
	int height_parent = widget->requested_size.height;
	if (widget->parent != NULL)
	{
		width_parent = widget->parent->screen_location.size.width;
		height_parent = widget->parent->screen_location.size.height;
	}
	ei_geometry_param_t *placer = (ei_geometry_param_t *)calloc(1, sizeof(ei_placer_t));

	placer->manager = ei_geometrymanager_from_name("placer\0\0\0\0\0\0\0\0\0\0\0\0\0");

	((ei_placer_t *)placer)->manager = *placer;
	((ei_placer_t *)placer)->anchor = anchor;
	((ei_placer_t *)placer)->x = ((x == NULL) ? widget->content_rect->top_left.x : *x);
	((ei_placer_t *)placer)->y = ((y == NULL) ? widget->content_rect->top_left.y : *y);
	((ei_placer_t *)placer)->rel_x = ((rel_x == NULL) ? -1 : *rel_x);
	((ei_placer_t *)placer)->rel_y = ((rel_y == NULL) ? -1 : *rel_y);
	((ei_placer_t *)placer)->rel_width = ((rel_width == NULL) ? -1 : *rel_width);
	((ei_placer_t *)placer)->rel_height = ((rel_height == NULL) ? -1 : *rel_height);
	((ei_placer_t *)placer)->width = ((width == NULL) ? (rel_width == NULL) ? widget->requested_size.width : ((ei_placer_t *)placer)->rel_width * width_parent : *width);
	((ei_placer_t *)placer)->height = ((height == NULL) ? (rel_height == NULL) ? widget->requested_size.height : ((ei_placer_t *)placer)->rel_height * height_parent : *height);

	widget->geom_params = placer;

	widget->geom_params->manager->runfunc(widget);
}

#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"

extern liste_widgetclass_t *liste_widgetclass;

/**
 * @brief	Registers a class to the program so that widgets of this class can be created.
 *		This must be done only once per widget class in the application.
 *
 * @param	widgetclass	The structure describing the class.
 */
void ei_widgetclass_register(ei_widgetclass_t *widgetclass)
{
	if (liste_widgetclass->first_widgetclass == NULL)
		liste_widgetclass->first_widgetclass = widgetclass;
	else
	{
		liste_widgetclass_t *sent = liste_widgetclass;
		while (sent->next != NULL)
			sent = sent->next;

		liste_widgetclass_t *new = calloc(1, sizeof(liste_widgetclass_t));
		new->first_widgetclass = widgetclass;
		sent->next = new;
	}
}

/**
 * @brief	Returns the structure describing a class, from its name.
 *
 * @param	name		The name of the class of widget.
 *
 * @return			The structure describing the class.
 */
ei_widgetclass_t *ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
	liste_widgetclass_t *sent = liste_widgetclass;
	while (strcmp(sent->first_widgetclass->name, name) && sent->next != NULL)
		sent = sent->next;

	if (!strcmp(sent->first_widgetclass->name, name))
		return sent->first_widgetclass;
	return NULL;
}

/**
 * \brief	Registers the "frame" widget class in the program. This must be called only
 *		once before widgets of the class "frame" can be created and configured with
 *		\ref ei_frame_configure.
 */
void ei_frame_register_class(void)
{
	ei_widgetclass_t *frame = return_class_frame();
	ei_widgetclass_register(frame);
}

/**
 * \brief	Registers the "button" widget class in the program. This must be called only
 *		once before widgets of the class "button" can be created and configured with
 *		\ref ei_button_configure.
 */
void ei_button_register_class(void)
{
	ei_widgetclass_t *button = return_class_button();
	ei_widgetclass_register(button);
}

/**
 * \brief	Registers the "toplevel" widget class in the program. This must be called only
 *		once before widgets of the class "toplevel" can be created and configured with
 *		\ref ei_toplevel_configure.
 */
void ei_toplevel_register_class(void)
{
	ei_widgetclass_t *toplevel = return_class_toplevel();
	ei_widgetclass_register(toplevel);
}
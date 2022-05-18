#include <string.h>

#include "ei_autre_event.h"

extern liste_eventtypes_t *liste_events_widgets;

/* ON PEUT OPTIMISER AVEC UN DICTIONNAIRE. VOIR APRÈS SI ÇA FONCTIONNE */

/**
 * \brief	Binds a callback to an event type and a widget or a tag.
 *
 * @param	eventtype	The type of the event.
 * @param	widget		The callback is only called if the event is related to this widget.
 *				This parameter must be NULL if the "tag" parameter is not NULL.
 * @param	tag		The callback is only called if the event is related to a widget that
 *				has this tag. A tag can be a widget class name, or the tag "all".
 *				This parameter must be NULL is the "widget" parameter is not NULL.
 * @param	callback	The callback (i.e. the function to call).
 * @param	user_param	A user parameter that will be passed to the callback when it is called.
 */
void ei_bind(ei_eventtype_t eventtype, ei_widget_t *widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
	liste_eventtypes_t *precedant = liste_events_widgets;
	liste_eventtypes_t *sentinel = liste_events_widgets;

	while (sentinel != NULL && sentinel->eventtype != eventtype)
	{
		precedant = sentinel;
		sentinel = sentinel->next;
	}

	if (sentinel == NULL)
	{
		liste_eventtypes_t *nouveau_eventtype = calloc(1, sizeof(liste_eventtypes_t));
		liste_events *nouveau_widget = calloc(1, sizeof(liste_events));

		nouveau_eventtype->eventtype = eventtype;
		nouveau_widget->widget = widget;
		nouveau_widget->eventtype = eventtype;
		nouveau_widget->user_param = user_param;
		nouveau_widget->callback = callback;
		nouveau_widget->tag = tag;
		nouveau_eventtype->liste = nouveau_widget;
		if (liste_events_widgets == NULL)
			liste_events_widgets = nouveau_eventtype;
		else
			precedant->next = nouveau_eventtype;
	}
	else
	{
		liste_events *suivant = sentinel->liste;
		while (suivant->next != NULL)
		{
			if ((suivant->widget == NULL && widget == NULL && !strcmp((char *)tag, (char *)(suivant->tag))) || (suivant->widget->pick_id == widget->pick_id))
			{
				suivant->callback = callback;
				suivant->user_param = user_param;
				return;
			}
			suivant = suivant->next;
		}
		if ((tag == NULL && suivant->widget != NULL && suivant->widget->pick_id == widget->pick_id) || (tag != NULL && !strcmp((char *)tag, (char *)(suivant->tag))))
		{
			suivant->callback = callback;
			suivant->user_param = user_param;
			return;
		}
		else
		{
			liste_events *nouveau = calloc(1, sizeof(liste_events));
			nouveau->widget = widget;
			nouveau->eventtype = eventtype;
			nouveau->user_param = user_param;
			nouveau->callback = callback;
			nouveau->tag = tag;
			suivant->next = nouveau;
		}
	}
}

/**
 * \brief	Unbinds a callback from an event type and widget or tag.
 *
 * @param	eventtype, widget, tag, callback, user_param
 *				All parameters must have the same value as when \ref ei_bind was
 *				called to create the binding.
 */
void ei_unbind(ei_eventtype_t eventtype, ei_widget_t *widget, ei_tag_t tag, ei_callback_t callback, void *user_param)
{
	liste_eventtypes_t *sentinel = liste_events_widgets;
	while (sentinel != NULL && sentinel->eventtype != eventtype)
		sentinel = sentinel->next;

	if (sentinel != NULL)
	{
		liste_events *liste_widget = sentinel->liste;
		if (liste_widget != NULL)
		{
			liste_events *suivant = liste_widget;
			liste_events *ancien = liste_widget;
			if (((suivant->widget == NULL && widget == NULL && !strcmp((char *)tag, (char *)(suivant->tag))) || suivant->widget->pick_id == widget->pick_id) && suivant->callback == callback &&
				suivant->user_param == user_param && suivant->eventtype == eventtype)
			{
				liste_widget = liste_widget->next;
				return;
			}
			while (suivant != NULL && suivant->next != NULL)
			{
				if (((suivant->next->widget == NULL && widget == NULL && !strcmp((char *)tag, (char *)(suivant->next->tag))) || suivant->next->widget->pick_id == widget->pick_id) && suivant->callback == callback &&
					suivant->user_param == user_param && suivant->eventtype == eventtype)
				{
					ancien = suivant->next;
					suivant->next = suivant->next->next;
					return;
				}
				else
				{
					ancien = suivant;
					suivant = suivant->next;
				}
			}
			if (((suivant != NULL && suivant->widget == NULL && widget == NULL && !strcmp((char *)tag, (char *)(suivant->tag))) || (suivant != NULL && suivant->widget != NULL && widget != NULL && suivant->widget->pick_id == widget->pick_id)) && suivant->callback == callback &&
				suivant->user_param == user_param && suivant->eventtype == eventtype)
			{
				suivant = ancien->next;
				ancien->next = NULL;
			}
		}
	}
}
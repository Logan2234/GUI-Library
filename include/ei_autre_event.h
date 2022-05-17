#include "ei_event.h"
#include "ei_autre_struct.h"

/*
 * Fonction de recherche des callbacks pour un événement donné.
 * Entrée : la liste chainée de liste chainées (*liste), l'eventement en question (*event), un booleen (specifique), un widget et un tag
 * Sortie : Un booleen indiquant si la fonction a trouvé un callback associé à cet événement.
 * La liste chainée est une liste_eventtypes (cf. ei_autre_struct.h)
 * Lorsque l'événement n'est pas spécifique à un widget en particulier, la fonction lance tous les callback de la liste chainée associé à un événement.
 * Spécifique vaut EI_FALSE
 * Si l'événement est spécifique à un événement ou tag, la fonction prend en compte le tag et le widget pour chercher les callbacks des widgets ou tags spécifiques à un tel événement.
 * Spécifique vaut EI_TRUE
 */
ei_bool_t recherche_traitants_event(struct liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag);

/*
 * Fonction de libération de la mémoire associée à une liste_eventtypes_t (cf. ei_autre_struct.h)
 */
void free_liste_eventtypes(struct liste_eventtypes_t *liste);

/*
 * Fonction générale utilisée comme callback lors de l'intéraction avec des boutons.
 * Cette fonction permet de gérer entre autres le changement de relief lors de l'intéraction
 * et l'appel de la fonction callback associée au bouton après le clic.
 */
ei_bool_t relief_toggle(ei_widget_t *widget, ei_event_t *event, void *user_param);

/*
 * Fonction callback appelée lors du relachement d'un close_button des toplevel
 * Elle a pour effet de détruire tout les éléments s'y trouvant à l'intérieur.
 */
ei_bool_t close_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

// TODO Jorge
ei_bool_t deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

// TODO Jorge
ei_bool_t deplacement_actif(ei_widget_t *widget, struct ei_event_t *event, void *user_param);

// TODO Jorge
ei_bool_t fin_deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event, void *user_param);
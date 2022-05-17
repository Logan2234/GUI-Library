/**
 * @file ei_autre_event.h
 * @brief Permet le déplacement et la fermeturee de toplevel.
 * 
 *
 */

#include "ei_event.h"
#include "ei_autre_struct.h"

/**
 * @brief Fonction de recherche des callbacks pour un événement donné.
 * 
 * @param liste Une liste chainée de liste chainées (*liste).
 * 
 * @param event L'évènement dont on doit trouver les callback associés (*event)
 * 
 * @param specifique booleen pour savoir si l'évènement est spécifique à un widget ou à un tag
 * 
 * @param widget Valable uniquement si specifique vaut true.
 * 
 * @param tag Valable uniquement si specifique vaut true.
 * 
 * @return Un booleen indiquant si la fonction a trouvé un callback associé à cet événement.
 * 
 */
ei_bool_t recherche_traitants_event(liste_eventtypes_t *liste, ei_event_t *event, ei_bool_t specifique, ei_widget_t *widget, ei_tag_t tag);

/**
 * @brief Fonction de libération de la mémoire associée à une liste_eventtypes_t (cf. ei_autre_struct.h).
 * 
 * @param liste Correspond à la liste qu'il faut libérer en mémoire.
 * 
 */
void free_liste_eventtypes(liste_eventtypes_t *liste);

/**
 * @brief Fonction générale utilisée comme callback lors de l'intéraction avec des boutons.
 * Cette fonction permet de gérer entre autres le changement de relief lors de l'intéraction
 * et l'appel de la fonction callback associée au bouton après le clic.
 * 
 * @param widget
 * 
 * @param event
 * 
 * @return
 */
ei_bool_t relief_toggle(ei_widget_t *widget, ei_event_t *event);

/** 
 * @brief
 * Fonction callback appelée lors du relachement d'un close_button des toplevel
 * Elle a pour effet de détruire tout les éléments s'y trouvant à l'intérieur.
 * 
 * @param widget Le widget (en l'occurance un toplebvel) qu'il faut fermer.
 * 
 * @return TRUE lorsqu'il est bien fermé.
 * 
 */
ei_bool_t close_toplevel(ei_widget_t *widget);

/**
 * @brief Initialisation du déplacement du toplevel
 * 
 * @param widget Le widget (toplevel en l'occurance) que l'on déplace.
 *
 * @param event L'évènement associé (resize ou déplacement).
 *
 * @return Booléen retournant s'il y a une action à associer au widget.
 * 
 */ 
ei_bool_t deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event);

/**
 * @brief Permet de déplacer le widget et de l'afficher se déplaçant.
 * 
 * @param widget Le widget (toplevel en l'occurance) que l'on déplace.
 * 
 * @param event L'évènement associé (resize ou déplacement).
 * 
 * @return Booléen retournant s'il y a une action à associer au widget.
 * 
 */ 
ei_bool_t deplacement_actif(ei_widget_t *widget, struct ei_event_t *event);

/**
 * @brief Déclare la fin du déplacement du toplevel.
 * 
 * @param widget le toplevel auquel l'event s'arrete.
 * 
 * @param event L'évènement qui agit sur le toplevel.
 * 
 * @return False s'il n'y avait pas de resize ni de mouvement, et True si le changement est finie.
 * 
 */ 
ei_bool_t fin_deplacement_toplevel(ei_widget_t *widget, struct ei_event_t *event);
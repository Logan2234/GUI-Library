#include "ei_widget.h"

/*
 * Permet de tracer tout les widgets. La fonction est tout le temps
 * appelée avec widget = racine de sorte à parcourir tout les widgets
 */
void draw_widgets_and_family(ei_widget_t *widget);

/*
 * Permet de free tout les points d'une liste chaînée de points.
 */
void free_linked_point_pointeur(ei_linked_point_t *liste);

/*
 * Permet d'ajouter tout les liens de parentés nécessaires
 * entre le père et le fils.
 */
void ajout_relation_parent(ei_widget_t *pere, ei_widget_t *fils);

/*
 * Transforme un entier en une couleur, pour les id des widgets
 */
ei_color_t int_to_color(uint32_t entier);

/*
 * Cherche parmi l'arbre des widgets celui correspondant à l'id fournit
 */
ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id);

/*
 * Fonction qui est lancée une et une seule fois en début de ei_app_run.
 * Elle permet d'attribuer à chaque widget toplevel un bouton de fermeture
 * et un évènement de drag pour le déplacement.
 */
void init_toplevel(ei_widget_t *widget);

/* Fonction simplifiant la mise à jour des surfaces pour le dessins */
void update_surface(ei_linked_rect_t *rectangles_list);

/*
 * Permet d'éclaircir une couleur
 */
void lighten_color(ei_color_t *couleur);

/*
 * Permet d'assombrir une couleur
 */
void darken_color(ei_color_t *couleur);

/*
 * Donne un widget et renvoie là où le texte est censé s'afficher 
 */
ei_point_t compute_location(ei_widget_t *widget, ei_anchor_t *ancre);
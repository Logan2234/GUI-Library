#include "ei_widget.h"
#include "ei_event.h"

/*
 * Permet de tracer tout les widgets. La fonction est tout le temps
 * appelée avec widget = racine de sorte à parcourir tout les widgets
 */
void draw_widgets_and_family(ei_widget_t *widget);

/*
 * Permet de free tout les widgets. La fonction est tout le temps
 * appelée avec widget = racine de sorte à parcourir tout les widgets
 */
void free_widgets_and_family(ei_widget_t *widget);

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
static ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id);

/*
 * Utilise la fonction précédente pour chercher avec quel widget
 * l'utilisateur a intéragit. On prend l'event de clic, et on renvoie
 * le widget en question.
 */
ei_widget_t *search_widget_by_click(ei_event_t *event);

/*
 * Fonction qui est lancée une et une seule fois en début de ei_app_run.
 * Elle permet d'attribuer à chaque widget toplevel un bouton de fermeture.
 */
void create_close_button_for_each_toplevel(ei_widget_t *widget);

/* Fonction simplifiant la mise à jour des surfaces pour le dessins */
void update_surface(ei_linked_rect_t *rectangles_list);
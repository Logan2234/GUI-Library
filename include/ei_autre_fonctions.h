#include "ei_widget.h"

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
ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id);

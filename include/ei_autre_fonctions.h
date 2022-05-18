/**
 * @file ei_autre_fonctions.h
 * @brief Regroupe toutes les fonctions utiles pour la création des widgets.
 *
 *
 */

#include "ei_widget.h"
#include "ei_geometrymanager.h"

/* Fonction pour mettre un toplevel en premier plan */
void put_on_head(ei_widget_t *widget);

/* Fonction de retour de la widgetclass frame */
ei_widgetclass_t *return_class_frame();

/* Fonction de retour de la widgetclass button */
ei_widgetclass_t *return_class_button();

/* Fonction de retour de la widgetclass toplevel */
ei_widgetclass_t *return_class_toplevel();

/* Fonction de retour de la widgetclass radiobutton */
ei_widgetclass_t *return_class_radiobutton();

/* Fonction de retour du gestionnaire de géométrie placer */
ei_geometrymanager_t *return_geometry_manager_placer();

/**
 * @brief Permet de free tout les points d'une liste chaînée de points.
 *
 * @param liste Liste de points à free. Cette liste provient de draw_arc et de round_frame.
 *
 */
void free_linked_point_pointeur(ei_linked_point_t *liste);

/*
 * Permet de free tout les rects d'une liste chaînée de rectangles sauf le premier si entier = EI_TRUE
 */
void free_linked_rects(ei_linked_rect_t *liste_rect);

/**
 * @brief Permet d'ajouter tout les liens de parentés nécessaires
 * entre le père et le fils.
 *
 * @param pere Le widget père.
 *
 * @param fils Le widget fils.
 *
 */
void ajout_relation_parent(ei_widget_t *pere, ei_widget_t *fils);

/**
 * @brief Transforme un entier en une couleur, pour les id des widgets.
 *
 * @param entier Entier à transformer en couleur.
 *
 * @return couleur renvoyée en RGBA.
 */
ei_color_t int_to_color(uint32_t entier);

/**
 * @brief Cherche parmi l'arbre des widgets celui correspondant à l'id fournit
 *
 * @param widget A appeler avec la racine pour parcourir tous les widgets
 *
 * @param id Entier qui correspond à l'ID du widget à trouver
 *
 * @return Le widget correspondant à l'ID recherché dans l'arbre du widget d'entrée.
 *
 */
ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id);

/**
 * @brief Fonction qui est lancée une et une seule fois en début de ei_app_run.
 * Elle permet d'attribuer à chaque widget toplevel un bouton de fermeture
 * et un évènement de drag pour le déplacement.
 *
 * @param widget correspond au widget racine.
 *
 */
void init_toplevel(ei_widget_t *widget);

/**
 * @brief Fonction simplifiant la mise à jour des surfaces pour le dessins
 *
 * @param rectangles_list liste des surfaces à mettre à jour.
 *
 * @param ponctuel Booléen s'il vaut EI_TRUE il permet de forcerla mise à jour, sinon la mise à jour ne peut se faire que fps fois par seconde.
 *
 */
ei_linked_rect_t *update_surface(ei_linked_rect_t *rectangles_list, ei_bool_t ponctuel);

/**
 * @brief Permet d'éclaircir une couleur
 *
 * @param couleur La couleur à eclaircir de 50 par composante de couleur dans la limite de 255.
 *
 */
void lighten_color(ei_color_t *couleur);

/**
 * @brief Permet d'assombrir une couleur
 *
 * @param couleur La couleur à assombrir de 50 par composante de couleur dans la limite de 0.
 *
 */
void darken_color(ei_color_t *couleur);

/**
 * @brief Donne un widget et renvoie là où le texte est censé s'afficher
 *
 * @param widget Le widget à placer
 *
 * @param ancre L'ancre de ce qu'il faut afficher
 *
 * @param about_texte Booléen pour savoir s'il y a du texte ou non
 *
 * @return le point where duquel on doit placer le widget.
 *
 */
ei_point_t compute_location(ei_widget_t *widget, ei_anchor_t *ancre, ei_bool_t about_text);
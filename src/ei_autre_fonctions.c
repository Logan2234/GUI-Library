#include "ei_application.h"
#include "ei_autre_global_var.h"
#include "ei_autre_event.h"

extern ei_surface_t pick_surface;
extern ei_linked_rect_t *rect_to_update;

static double last_update;

/**
 * @brief Permet de tracer tout les widgets. 
 * 
 * @param widget Le widget appelé doit être la racine de sorte à parcourir tout les widgets
 * 
 */
static void draw_widgets_and_family(ei_widget_t *widget)
{
    ei_surface_t racine_surface = ei_app_root_surface();

    if (widget->parent == NULL)
        widget->wclass->drawfunc(widget, racine_surface, pick_surface, NULL);

    else if (widget->geom_params != NULL)
    {
        (widget->parent->pick_id == 1)
            ? widget->wclass->drawfunc(widget, racine_surface, pick_surface, widget->content_rect)
            : widget->wclass->drawfunc(widget, racine_surface, pick_surface, widget->parent->content_rect);
    }

    else
        return;

    ei_widget_t *current_widget = widget;
    if (current_widget->next_sibling != NULL)
        draw_widgets_and_family(current_widget->next_sibling);
    if (current_widget->children_head != NULL)
        draw_widgets_and_family(current_widget->children_head);
}

/**
 * @brief Permet de free tout les points d'une liste chaînée de points.
 * 
 * @param liste Liste de points à free. Cette liste provient de draw_arc et de round_frame.
 * 
 */
void free_linked_point_pointeur(ei_linked_point_t *liste)
{
    ei_linked_point_t *courant = liste;
    ei_linked_point_t *suivant = liste->next;
    while (suivant != NULL)
    {
        free(courant);
        courant = suivant;
        suivant = suivant->next;
    }
    free(courant);
}

void free_linked_rects(ei_linked_rect_t *liste_rect)
{
    ei_linked_rect_t *sent = liste_rect->next;
    ei_linked_rect_t *next;

    while (sent != NULL)
    {
        next = sent->next;
        free(sent);
        sent = next;
    }
}

/**
 * @brief Permet d'ajouter tout les liens de parentés nécessaires
 * entre le père et le fils.
 * 
 * @param pere Le widget père.
 * 
 * @param fils Le widget fils.
 * 
 */
void ajout_relation_parent(ei_widget_t *pere, ei_widget_t *fils)
{
    if (pere->children_head == NULL)
    {
        pere->children_head = fils;
        pere->children_tail = fils;
    }
    else
    {
        pere->children_tail->next_sibling = fils;
        pere->children_tail = fils;
    }
}

/**
 * @brief Transforme un entier en une couleur, pour les id des widgets.
 * 
 * @param entier Entier à transformer en couleur.
 * 
 * @return couleur renvoyée en RGBA.
 */
ei_color_t int_to_color(uint32_t entier)
{
    uint8_t red, green, blue, alpha;
    red = entier / (256 * 256);
    green = (entier - red * 256 * 256) / 256;
    blue = (entier - red * 256 * 256 - green * 256);
    return (ei_color_t){red, green, blue, 0xff};
}

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
ei_widget_t *search_widget_by_id(ei_widget_t *widget, uint32_t id)
{
    ei_widget_t *current_widget = widget;
    ei_widget_t *res = NULL;

    if (current_widget->pick_id == id)
        return current_widget;

    if (current_widget->children_head != NULL)
        res = search_widget_by_id(current_widget->children_head, id);

    if (res != NULL)
        return res;

    if (current_widget->next_sibling != NULL)
        res = search_widget_by_id(current_widget->next_sibling, id);

    if (res != NULL)
        return res;
    return NULL;
}

/**
 * @brief Fonction simplifiant la mise à jour des surfaces pour le dessins
 * 
 * @param rectangles_list liste des surfaces à mettre à jour.
 * 
 * @param ponctuel Booléen s'il vaut EI_TRUE il permet de forcerla mise à jour, sinon la mise à jour ne peut se faire que fps fois par seconde.
 * 
 */
ei_linked_rect_t *update_surface(ei_linked_rect_t *rectangles_list, ei_bool_t ponctuel)
{
    if (hw_now() - last_update > (double)1 / fps || ponctuel)
    {
        hw_surface_lock(ei_app_root_surface());
        draw_widgets_and_family(ei_app_root_widget());

        last_update = hw_now();

        hw_surface_unlock(ei_app_root_surface());
        
        if (rectangles_list->next != NULL && rectangles_list->rect.size.height != 0)
            hw_surface_update_rects(ei_app_root_surface(), rectangles_list);
        else
            hw_surface_update_rects(ei_app_root_surface(), NULL);
    }

    free_linked_rects(rectangles_list);

    rect_to_update->next = NULL;

    return rectangles_list;
}

/**
 * @brief Fonction qui est lancée une et une seule fois en début de ei_app_run.
 * Elle permet d'attribuer à chaque widget toplevel un bouton de fermeture
 * et un évènement de drag pour le déplacement.
 * 
 * @param widget correspond au widget racine.
 * 
 */
void init_toplevel(ei_widget_t *widget)
{
    ei_callback_t deplacement_callback = deplacement_toplevel;
    ei_callback_t fin_deplacement_callback = fin_deplacement_toplevel;
    ei_callback_t deplacement_actif_callback = deplacement_actif;
    ei_callback_t close_toplevel_widget = close_toplevel;
    ei_widget_t *button = NULL;

    if (*((ei_toplevel_t *)widget)->closable)
    {
        button = ei_widget_create("button\0\0\0\0\0\0\0\0\0\0\0\0\0", widget, NULL, NULL);
        ei_button_configure(button, NULL, &close_button_color, &close_button_border_width, &close_button_corner_radius, &close_button_relief, &close_button_text, NULL, NULL, NULL, NULL, NULL, NULL, &close_toplevel_widget, NULL);
        ei_place(button, &close_button_anchor, &close_button_x, &close_button_y, &close_button_width, &close_button_height, &close_button_rel_x, &close_button_rel_y, NULL, NULL);
    }
    ei_bind(ei_ev_mouse_buttondown, widget, NULL, deplacement_callback, NULL);
    ei_bind(ei_ev_mouse_buttonup, widget, NULL, fin_deplacement_callback, NULL);
    ei_bind(ei_ev_mouse_move, widget, NULL, deplacement_actif_callback, NULL);
}

/**
 * @brief Permet d'éclaircir une couleur
 * 
 * @param couleur La couleur à eclaircir de 50 par composante de couleur dans la limite de 255.
 * 
 */
void lighten_color(ei_color_t *couleur)
{
    couleur->red = (couleur->red <= 205) ? couleur->red + 50 : 255;
    couleur->green = (couleur->green <= 205) ? couleur->green + 50 : 255;
    couleur->blue = (couleur->blue <= 205) ? couleur->blue + 50 : 255;
}

/**
 * @brief Permet d'assombrir une couleur
 * 
 * @param couleur La couleur à assombrir de 50 par composante de couleur dans la limite de 0.
 * 
 */
void darken_color(ei_color_t *couleur)
{
    couleur->red = (couleur->red >= 50) ? couleur->red - 50 : 0;
    couleur->green = (couleur->green >= 50) ? couleur->green - 50 : 0;
    couleur->blue = (couleur->blue >= 50) ? couleur->blue - 50 : 0;
}

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
ei_point_t compute_location(ei_widget_t *widget, ei_anchor_t *ancre, ei_bool_t about_text)
{
    int largeur_contenu;
    int hauteur_contenu;

    if (about_text == EI_TRUE)
    {
        ei_surface_t text_surface;

        if (!strcmp(widget->wclass->name, "frame"))
            text_surface = hw_text_create_surface(*((ei_frame_t *)widget)->text, *((ei_frame_t *)widget)->text_font, *((ei_frame_t *)widget)->text_color);

        else
            text_surface = hw_text_create_surface(*((ei_button_t *)widget)->text, *((ei_button_t *)widget)->text_font, *((ei_button_t *)widget)->text_color);

        largeur_contenu = hw_surface_get_size(text_surface).width;
        hauteur_contenu = hw_surface_get_size(text_surface).height;
        hw_surface_free(text_surface);
    }
    else
    {
        if (!strcmp(widget->wclass->name, "frame"))
        {
            largeur_contenu = hw_surface_get_size(((ei_frame_t *)widget)->img).width;
            hauteur_contenu = hw_surface_get_size(((ei_frame_t *)widget)->img).height;
        }
        else
        {
            largeur_contenu = hw_surface_get_size(((ei_button_t *)widget)->img).width;
            hauteur_contenu = hw_surface_get_size(((ei_button_t *)widget)->img).height;
        }
    }

    ei_point_t point = widget->content_rect->top_left;
    int largeur_parent = widget->content_rect->size.width;
    int hauteur_parent = widget->content_rect->size.height;

    if (ancre == NULL)
    {
        point.x += (largeur_parent - largeur_contenu) / 2;
        point.y += (hauteur_parent - hauteur_contenu) / 2;
    }
    else
    {
        switch (*ancre)
        {
        case ei_anc_none:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_northwest:
            break;
        case ei_anc_north:
            point.x += (largeur_parent - largeur_contenu) / 2;
            break;
        case ei_anc_northeast:
            point.x += (largeur_parent - largeur_contenu);
            break;
        case ei_anc_west:
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_center:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_east:
            point.x += (largeur_parent - largeur_contenu);
            point.y += (hauteur_parent - hauteur_contenu) / 2;
            break;
        case ei_anc_southwest:
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        case ei_anc_south:
            point.x += (largeur_parent - largeur_contenu) / 2;
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        case ei_anc_southeast:
            point.x += (largeur_parent - largeur_contenu);
            point.y += (hauteur_parent - hauteur_contenu);
            break;
        }
    }
    return point;
}

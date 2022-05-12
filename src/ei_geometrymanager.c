#include "ei_geometrymanager.h"
#include "ei_autre_struct.h"

extern struct liste_geometrymanager *liste_geometrymanager;

void ei_geometrymanager_register(ei_geometrymanager_t *geometrymanager)
{
    if (liste_geometrymanager->geometrymanager_cell == NULL)
        liste_geometrymanager->geometrymanager_cell = geometrymanager;

    else
    {
        struct liste_geometrymanager *sent = liste_geometrymanager;
        while (sent->next != NULL)
            sent = sent->next;

        struct liste_geometrymanager *new_cell = calloc(1, sizeof(struct liste_geometrymanager));
        new_cell->geometrymanager_cell = geometrymanager;
        sent->next = new_cell;
    }
}

void ei_geometrymanager_unmap(ei_widget_t *widget)
{
    // TODO
}

ei_geometrymanager_t *ei_geometrymanager_from_name(ei_geometrymanager_name_t name)
{
    struct liste_geometrymanager *sent = liste_geometrymanager;
    while (strcmp(sent->geometrymanager_cell->name, name) && sent->next != NULL)
        sent = sent->next;

    if (!strcmp(sent->geometrymanager_cell->name, name))
        return sent->geometrymanager_cell;
}

void ei_register_placer_manager(void)
{
    ei_geometrymanager_t *placer = return_geometry_manager_placer();
    ei_geometrymanager_register(placer);
}

/*
 Configure la géométrie d'un widget à l'aide du gestionnaire de géométrie "placer".
 * Si le widget était déjà géré par un autre gestionnaire de géométrie, alors il est d'abord
   supprimé du gestionnaire de géométrie précédent. // TODO
 * Si le widget était déjà géré par le "placer", alors cela appelle simplement les mises à jour
   les paramètres placer : les arguments non NULL remplacent les valeurs précédentes.
 * Lorsque les arguments sont passés comme NULL, le placer utilise les valeurs par défaut (détaillées dans
   les descriptions des arguments ci-dessous). Si aucune taille n'est fournie (absolue ou
   relatif), alors la taille demandée du widget est utilisée, c'est-à-dire la taille minimale
   requis pour afficher son contenu. */
void ei_place(ei_widget_t *widget, ei_anchor_t *anchor, int *x, int *y, int *width, 
              int *height, float *rel_x, float *rel_y, float *rel_width, float *rel_height)
{
    
    /* Gestion du paramètre geom_params du widget */
    widget->geom_params = ei_geometrymanager_from_name("placer");
    
    /* Initialisation des variables pour contrer les NULL */
    ei_point_t *top_left = calloc(1, sizeof(ei_point_t));

    int width_parent = widget->requested_size.width;
    int height_parent = widget->requested_size.height;

    int x2, y2;
    int width2, height2;
    float rel_x2, rel_y2;
    int rel_height2, rel_width2;

    x2 = ((x == NULL) ? widget->screen_location.top_left.x : *x);
    y2 = ((y == NULL) ? widget->screen_location.top_left.y : *y);
    rel_x2 = ((rel_x == NULL) ? 0 : *rel_x);
    rel_y2 = ((rel_y == NULL) ? 0 : *rel_y);

    if (widget->parent != NULL)
    {
        top_left->x = ((rel_x != NULL) ? widget->parent->screen_location.top_left.x : 0);
        top_left->y = ((rel_y != NULL) ? widget->parent->screen_location.top_left.y : 0);
        width_parent = widget->parent->screen_location.size.width;
        height_parent = widget->parent->screen_location.size.height;
    }

    rel_width2 = ((rel_width == NULL) ? 0 : *rel_width);
    rel_height2 = ((rel_height == NULL) ? 0 : *rel_height);

    height2 = ((height == NULL) ? widget->requested_size.height : *height);
    height2 += rel_height2 * height_parent;

    width2 = ((width == NULL) ? widget->requested_size.width : *width);
    width2 += rel_width2 * width_parent;

    /* Maintenant on remplace dans les données de widgets */
    widget->screen_location.size.width = width2;
    widget->screen_location.size.height = height2;
    if (anchor == NULL)
    {
        top_left->x += x2 + rel_x2 * width_parent;
        top_left->y += y2 + rel_y2 * height_parent;
    }
    else
    {
        switch (*anchor)
        {
        case ei_anc_none:
            top_left->x += x2 + rel_x2 * width_parent;
            top_left->y += y2 + rel_y2 * height_parent;
            break;
        case ei_anc_center:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2) / 2;
            top_left->y += y2 + rel_y2 * height_parent - height2 / 2;
            break;
        case ei_anc_north:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2) / 2;
            top_left->y += y2 + rel_y2 * height_parent;
            break;
        case ei_anc_south:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2) / 2;
            top_left->y += y2 + rel_y2 * height_parent - (height2 + rel_height2);
            break;
        case ei_anc_east:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2);
            top_left->y += y2 + rel_y2 * height_parent - (height2 + rel_height2) / 2;
            break;
        case ei_anc_west:
            top_left->x += x2 + rel_x2 * width_parent;
            top_left->y += y2 + rel_y2 * height_parent - (height2 + rel_height2) / 2;
            break;
        case ei_anc_northeast:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2);
            top_left->y += y2 + rel_y2 * height_parent;
            break;
        case ei_anc_northwest:
            top_left->x += x2 + rel_x2 * width_parent;
            top_left->y += y2 + rel_y2 * height_parent;
            break;
        case ei_anc_southeast:
            top_left->x += x2 + rel_x2 * width_parent - (width2 + rel_width2);
            top_left->y += y2 + rel_y2 * height_parent - (height2 + rel_height2);
            break;
        case ei_anc_southwest:
            top_left->x += x2 + rel_x2 * width_parent;
            top_left->y += y2 + rel_y2 * height_parent - (height2 + rel_height2);
            break;
        }
    }
    widget->screen_location.top_left = *top_left;
    free(top_left);
}

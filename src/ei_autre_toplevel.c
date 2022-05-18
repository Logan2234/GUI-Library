#include "ei_autre_struct.h"
#include "ei_autre_fonctions.h"
#include "ei_autre_global_var.h"
#include "ei_autre_draw.h"
#include "ei_application.h"

extern int widget_id;
extern ei_surface_t pick_surface;

static ei_widget_t *toplevel_allocfunc(void)
{
    ei_toplevel_t *widget_toplevel = calloc(1, sizeof(ei_toplevel_t));
    return (ei_widget_t *)widget_toplevel;
}

static void toplevel_releasefunc(ei_widget_t *widget)
{
    free(widget->pick_color);
    free(widget->geom_params);
    free(widget->content_rect);
    free(((ei_toplevel_t *)widget)->border_width);
    free(((ei_toplevel_t *)widget)->closable);
    free(((ei_toplevel_t *)widget)->color);
    free(((ei_toplevel_t *)widget)->resizable);
    // free(*((ei_toplevel_t *)widget)->title);
    free(((ei_toplevel_t *)widget)->title);
    // free(*((ei_toplevel_t *)widget)->min_size);
    free(((ei_toplevel_t *)widget)->min_size);
    free((ei_toplevel_t *)widget);
}

/**
 * @brief Fonction permettant de dessiner un toplevel
 * 
 * @param widget Widget (en l'occurance un toplevel) à dessiner.
 * 
 * @param surface La surface sur lequel on le dessine.
 * 
 * @param pick_surface Surface permettant de savoir sur quel objet on clique.
 * 
 * @param clipper Zone délimitant le dessin par son parent.
 * 
 */
static void toplevel_drawfunc(ei_widget_t *widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t *clipper)
{
    ei_toplevel_t *toplevel = (ei_toplevel_t *)widget;

    /* On trace le fond */
    ei_linked_point_t *premier_point = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel = premier_point;
    premier_point->point.x = widget->screen_location.top_left.x;
    premier_point->point.y = widget->screen_location.top_left.y + taille_header;

    ei_linked_point_t *nouveau = calloc(1, sizeof(ei_linked_point_t));
    nouveau->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau->point.y = widget->screen_location.top_left.y + taille_header;
    sentinel->next = nouveau;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau1 = calloc(1, sizeof(ei_linked_point_t));
    nouveau1->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau1->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + taille_header;
    sentinel->next = nouveau1;
    sentinel = sentinel->next;

    ei_linked_point_t *nouveau2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau2->point.x = widget->screen_location.top_left.x;
    nouveau2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + taille_header;
    sentinel->next = nouveau2;

    ei_draw_polygon(surface, premier_point, *toplevel->color, clipper);

    free_linked_point_pointeur(premier_point);

    /* On trace les bordures */
    ei_linked_point_t *contour = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sent = contour;

    contour->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    contour->point.y = widget->screen_location.top_left.y + taille_header;

    ei_linked_point_t *nouveau_contour = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    nouveau_contour->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width + taille_header;
    sent->next = nouveau_contour;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour2 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour2->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
    nouveau_contour2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width + taille_header;
    sent->next = nouveau_contour2;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour3 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour3->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
    nouveau_contour3->point.y = widget->screen_location.top_left.y + taille_header;
    sent->next = nouveau_contour3;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour4 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour4->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau_contour4->point.y = widget->screen_location.top_left.y + taille_header;
    sent->next = nouveau_contour4;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour5 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour5->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width;
    nouveau_contour5->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + taille_header;
    sent->next = nouveau_contour5;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour6 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour6->point.x = widget->screen_location.top_left.x;
    nouveau_contour6->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + taille_header;
    sent->next = nouveau_contour6;
    sent = sent->next;

    ei_linked_point_t *nouveau_contour7 = calloc(1, sizeof(ei_linked_point_t));
    nouveau_contour7->point.x = widget->screen_location.top_left.x;
    nouveau_contour7->point.y = widget->screen_location.top_left.y + taille_header;
    sent->next = nouveau_contour7;

    ei_rect_t new_clipper_toplevel = *clipper;

    if (widget->parent->pick_id == 1)
    {
        new_clipper_toplevel.top_left.x -= *toplevel->border_width;
        new_clipper_toplevel.top_left.y -= taille_header;
        new_clipper_toplevel.size.width += 2 * *toplevel->border_width;
        new_clipper_toplevel.size.height += *toplevel->border_width + taille_header;
    }

    ei_draw_polygon(surface, contour, (ei_color_t){0x63, 0x69, 0x70, 0xff}, &new_clipper_toplevel);

    free_linked_point_pointeur(contour);

    /* On trace le header */
    ei_linked_point_t *header = calloc(1, sizeof(ei_linked_point_t));
    ei_linked_point_t *sentinel2 = header;
    header->point.x = widget->screen_location.top_left.x - *toplevel->border_width;
    header->point.y = widget->screen_location.top_left.y + taille_header;

    ei_linked_point_t *nouveau_header = calloc(1, sizeof(ei_linked_point_t));
    nouveau_header->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width;
    nouveau_header->point.y = widget->screen_location.top_left.y + taille_header;
    sentinel2->next = nouveau_header;
    sentinel2 = sentinel2->next;

    ei_point_t centre_droit = {widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width - 15, widget->screen_location.top_left.y + 15};
    ei_linked_point_t *nouveau_header2 = ei_draw_arc(centre_droit, 15, 0, 1.57075);
    sentinel2->next = nouveau_header2;
    while (sentinel2->next != NULL)
        sentinel2 = sentinel2->next;

    ei_point_t centre_gauche = {widget->screen_location.top_left.x - *toplevel->border_width + 15, widget->screen_location.top_left.y + 15};
    ei_linked_point_t *nouveau_header3 = ei_draw_arc(centre_gauche, 15, 1.57075, 3.1415);
    sentinel2->next = nouveau_header3;

    ei_draw_polygon(surface, header, (ei_color_t){0x63, 0x69, 0x70, 0xff}, &new_clipper_toplevel);

    free_linked_point_pointeur(header);

    /* On trace un petit carré qui indique qu'on peut resize la fenêtre */
    if (*toplevel->resizable != ei_axis_none)
    {
        ei_linked_point_t *resize_indicator = calloc(1, sizeof(ei_linked_point_t));
        ei_linked_point_t *sentinel3 = resize_indicator;
        resize_indicator->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width - 1;
        resize_indicator->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height - 15 + taille_header - 1;

        ei_linked_point_t *resize_indicator2 = calloc(1, sizeof(ei_linked_point_t));
        resize_indicator2->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width + *toplevel->border_width - 1;
        resize_indicator2->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width + taille_header - 1;
        sentinel3->next = resize_indicator2;
        sentinel3 = sentinel3->next;

        ei_linked_point_t *resize_indicator3 = calloc(1, sizeof(ei_linked_point_t));
        resize_indicator3->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width - 15 + *toplevel->border_width - 1;
        resize_indicator3->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + *toplevel->border_width + taille_header - 1;
        sentinel3->next = resize_indicator3;
        sentinel3 = sentinel3->next;

        ei_linked_point_t *resize_indicator4 = calloc(1, sizeof(ei_linked_point_t));
        resize_indicator4->point.x = widget->screen_location.top_left.x + widget->screen_location.size.width - 15 + *toplevel->border_width - 1;
        resize_indicator4->point.y = widget->screen_location.top_left.y + widget->screen_location.size.height + taille_header - 15;
        sentinel3->next = resize_indicator4;

        ei_draw_polygon(surface, resize_indicator, (ei_color_t){0x63, 0x69, 0x70, 0xff}, &new_clipper_toplevel);

        free_linked_point_pointeur(resize_indicator);
    }

    /* Et enfin, on trace le titre */
    ei_point_t titre_pos = widget->screen_location.top_left;
    titre_pos.x += taille_header;
    titre_pos.y += 4;
    ei_draw_text(surface, &titre_pos, *toplevel->title, ei_default_font, (ei_color_t){0xFF, 0xFF, 0xFF, 0xFF}, &new_clipper_toplevel);

    /* La pick_color n'est dessinée que si l'on reste dans le clipper bien sûr */
    ei_rect_t new_clipper_including_header = *widget->parent->content_rect;
    if (widget->screen_location.top_left.x <= new_clipper_including_header.top_left.x + new_clipper_including_header.size.width &&
        widget->screen_location.top_left.x + widget->screen_location.size.width >= new_clipper_including_header.top_left.x &&
        widget->screen_location.top_left.y <= new_clipper_including_header.top_left.y + new_clipper_including_header.size.height &&
        widget->screen_location.top_left.y + widget->screen_location.size.height >= new_clipper_including_header.top_left.y)
        ei_fill(pick_surface, widget->pick_color, &new_clipper_including_header);
}

/**
 * @brief Initialise un toplevel avec les paramètres par défaut.
 * 
 * @param widget Widget (toplevel) que l'on doit initialiser.
 * 
 */
static void toplevel_setdefaultsfunc(ei_widget_t *widget)
{
    /* Gestion du pick_id et de la couleur associée au pick_id */
    widget->pick_id = widget_id;
    ei_color_t *pick_color = malloc(sizeof(ei_color_t));
    *pick_color = int_to_color(widget_id);
    widget->pick_color = pick_color;

    /* Nécessité d'avoir un calloc pour le content_rect ? */
    ei_rect_t *content_rect = calloc(1, sizeof(ei_rect_t));
    *content_rect = widget->screen_location;
    widget->content_rect = content_rect;

    /* Et enfin, on lui donne une configuration de base */
    ei_toplevel_configure(widget, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    init_toplevel(widget);
}

/**
 * @brief Permet de prévenir les fils lorsque qu'un changement apparait sur un widget comme un resize ou un déplacement
 * 
 * @param widget Widget (toplevel) sur lequel un event apparait.
 * 
 * 
 */
static void toplevel_geomnotifyfunc(ei_widget_t *widget)
{
    widget->geom_params->manager->runfunc(widget);

    ei_widget_t *sent = widget->children_head;
    while (sent != NULL)
    {
        sent->wclass->geomnotifyfunc(sent);
        sent = sent->next_sibling;
    }
}

/**
 * @brief Créé un widgetclass toplevel avec les initialisations lors de la création de la fenêtre principale pour pouvoir construire un toplevel
 * 
 * @return La classe du toplevel avec les fonctions de bases du toplevel.
 * 
 */
ei_widgetclass_t *return_class_toplevel(void)
{
    ei_widgetclass_t *widgetclass_toplevel = calloc(1, sizeof(ei_widgetclass_t));

    strcpy(widgetclass_toplevel->name, "toplevel");
    widgetclass_toplevel->allocfunc = &toplevel_allocfunc;
    widgetclass_toplevel->releasefunc = &toplevel_releasefunc;
    widgetclass_toplevel->drawfunc = &toplevel_drawfunc;
    widgetclass_toplevel->setdefaultsfunc = &toplevel_setdefaultsfunc;
    widgetclass_toplevel->geomnotifyfunc = &toplevel_geomnotifyfunc;
    widgetclass_toplevel->next = NULL;

    return widgetclass_toplevel;
}

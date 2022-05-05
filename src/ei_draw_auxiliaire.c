#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_autre_draw.h"
#include <math.h>
#include <stdlib.h>


ei_linked_point_t* ei_draw_arc (ei_point_t center,
                                uint32_t rayon,
                                float angle_debut,
                                float angle_fin)
{
    float longueur_arc = abs((float) rayon * (angle_fin - angle_debut));
    float pas = (angle_fin - angle_debut) / longueur_arc;
    ei_linked_point_t *liste_point;
    liste_point->point.x = (int) rayon * cos(angle_debut);
    liste_point->point.y = (int) rayon * sin(angle_debut);
    ei_linked_point_t *suivant;
    suivant = liste_point;
    for (i = 1; i <= longueur_arc; i++){
        int x = (int) rayon * cos(angle_debut + i * pas);
        int y = (int) rayon * sin(angle_debut + i * pas);
        while (suivant->next != NULL){
            suivant = suivant->next;
        }
        if (suivant->next->point.x != x || suivant->next->point.y != y){
            ei_linked_point_t *nouveau;
            nouveau->point.x = x;
            nouveau->point.y = y;
            nouveau->next = NULL;
            suivant->next = nouveau;
        }
    }
    return liste_point;
}
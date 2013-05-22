/**
* \file edit_affichage.c
* \version 1.0
* \date 2013
* \brief Permet l'affichage du terrain dessin�.
* \details Ce fichier permet d'afficher le terrain et les �tapes interm�diaires.
*/

#include <stdlib.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "main.h"
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"
#include "terrains.h"

#include "edit_main.h"
#include "edit_affichage.h"

/**
* \brief Permet l'affichage du terrain.
* \details Cette fonction affiche les objets d�j� dessin�s. Ele permet �galement de dessiner une version interm�diaire de l'objet en cours de dessin.
* \param mode Mode courant. Pour l'instant, seul le mode Dessin a �t� impl�ment�.
* \param debut Dans le cas o� un objet est en train d'�tre dessin�, debut permet de pr�ciser qu'il faut fournir une image interm�diaire pour l'objet en cours de dessin,
* bas� sur la position courante de la souris.
* \param fragments Pointeur vers le premier objet devant �tre dessin�. Celui-ci contient un pointeur vers l'objet suivant, s'il n'est pas nul.
* \param ecran L'�cran sur lequel il faut dessiner.
* \param posecran Position de l'�cran, ajustable avec les fl�ches gauche et droite.
* \param surfaceFond Surface Cairo sur laquelle on va dessiner.
* \param surfLigne Surface SDL sr laquelle on va copier la surface Cairo pour l'affichage.
* \param positionecran Permet de d�terminer la position de la surface � dessiner.
* \param droite Droite Cairo que l'on va dessiner.
* \param tailleecran Taille de la surface qu'il faut dessiner.

*/
void edit_affichage(Mode mode, Bool debut, Fragment* fragments, SDL_Surface* ecran, int posecran,
                cairo_surface_t *surfaceFond, SDL_Surface *surfLigne, SDL_Rect positionecran, cairo_t *droite, SDL_Rect tailleecran) {

    int n, x, y;
    SDL_GetMouseState(&x, &y);

    Fragment* neuf = NULL;
    SDL_FillRect(surfLigne, &tailleecran, 0xFFFFFFFF);
    droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite, eEPAISSEUR_TRAIT);
    Point* nvp = NULL;

    neuf = fragments;
    while(neuf!=NULL) {
        n=0;
        if (neuf->couleur==Rouge)
            cairo_set_source_rgba (droite, 1, 0, 0, 1);
        else
            cairo_set_source_rgba (droite, 0, 0, 0, 1);
        nvp = neuf->chaine;

        cairo_move_to(droite, nvp->x, nvp->y);
        nvp = nvp->next;
        while (neuf->lench>=4+3*n) {
            cairo_curve_to(droite, nvp->x, nvp->y, nvp->next->next->x, nvp->next->next->y, nvp->next->x, nvp->next->y);
            n++;
            nvp = nvp->next->next->next;
        }
        if (mode==Dessin && debut==Faux && neuf->next==NULL) {
            if (neuf->lench==4+3*n-1) {
                cairo_curve_to(droite, nvp->x, nvp->y, x+posecran, y, nvp->next->x, nvp->next->y);
            }
            if (neuf->lench==4+3*n-2) {
                cairo_curve_to(droite, nvp->x, nvp->y, x+posecran, y, x+posecran, y);
            }
        }
        cairo_stroke(droite);
        neuf = neuf->next;
    }


    SDL_BlitSurface(surfLigne, &tailleecran, ecran, &positionecran);
    SDL_Flip(ecran);

}

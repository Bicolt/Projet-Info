/**
 * \file      	terrains.c
 * \version   	1.0
 * \date      	2013
 * \brief		Création des terrains
 *
 * \details   	Permet de créer le terrain appelé par la fonction niveau de niveau.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"
#include "main.h"
#include "terrains.h"

#include "edit_main.h"
#include "edit_affichage.h"

/**
 * \brief il s'agit du tutoriel
 *
 * \param surfaceFond Pour ce terrain comme pour tout les autres, il s'agit de la surface sur laquelle le terrain est dessiné
 */
void tterrain0(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_select_font_face (droite, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (droite, 0.1*eh);

    cairo_move_to (droite, 0.3*ew, 0.2*eh);
    cairo_show_text (droite, "Bienvenue !");

    cairo_set_font_size (droite, 0.045*eh);
    cairo_move_to (droite, 0.3*ew, 0.3*eh);
    cairo_show_text (droite, "le but est d arriver a la fin");
    cairo_move_to (droite, 0.3*ew , 0.375*eh);
    cairo_show_text (droite, "du niveau le plus vite possible");
    cairo_move_to (droite, 0.3*ew , 0.45*eh);
    cairo_show_text (droite, "en evitant les obstacles");

    cairo_move_to (droite, 0.8*ew, 0.8*eh);
    cairo_show_text (droite, "pour cela il est possible de");
    cairo_move_to (droite, 0.8*ew, 0.875*eh);
    cairo_show_text (droite, "selectionner une zone de l ecran");
    cairo_move_to (droite, 0.8*ew, 0.95*eh);
    cairo_show_text (droite, "a l aide du clique gauche puis");

    cairo_move_to (droite, 1.3*ew, 0.3*eh);
    cairo_show_text (droite, "de la deplacer en maintenant le clique droit");
    cairo_move_to (droite, 1.3*ew, 0.375*eh);
    cairo_show_text (droite, "enfonce et de la coller en le relachant");
    cairo_move_to (droite, 1.3*ew, 0.45*eh);
    cairo_show_text (droite, "");

    cairo_move_to (droite, 1.8*ew, 0.8*eh);
    cairo_show_text (droite, "Il est aussi possible de tourner une piece en ");
    cairo_move_to (droite, 1.8*ew, 0.875*eh);
    cairo_show_text (droite, "cours de translation, en maintenant la touche r");
    cairo_move_to (droite, 1.8*ew, 0.95*eh);
    cairo_show_text (droite, "et en bougeant la souris pour determiner l angle");

	cairo_move_to (droite, 2.3*ew, 0.2*eh);
    cairo_show_text (droite, "Et de la redimensionner en utilisant ");
    cairo_move_to (droite, 2.3*ew, 0.275*eh);
    cairo_show_text (droite, "la molette de la souris");

    cairo_move_to(droite, 0, 0.7*eh);
    cairo_line_to(droite, 2.3*ew, 0.7*eh);
    cairo_curve_to(droite, 2.4*ew, 0.5*eh, 2.5*ew, 0.4*eh, 2.6*ew, 0.7*eh);
    cairo_move_to(droite, 2.8*ew,0.7*eh);
    cairo_line_to(droite, 2.8*ew, 0.6*eh);
    cairo_move_to(droite, 2.8*ew, 0.7*eh);
    cairo_line_to(droite, 3.1*ew, 0.7*eh);
    cairo_stroke(droite);

    cairo_set_source_rgba(droite, 1, 0, 0, 1);
    cairo_move_to(droite, 3.1*ew, 0.7*eh);
    cairo_line_to(droite, 3.2*ew, 0.7*eh);
    cairo_stroke(droite);

    cairo_set_source_rgba(droite, 0, 0, 0, 1);
    cairo_move_to(droite, 3.2*ew, 0.7*eh);
    cairo_line_to(droite, X_FIN+200.5, 0.7*eh);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

/**
 * \brief de 1 à 5, les terrains sont des terrains générés manuellement
 */
void tterrain1(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 0.7*eh);
    cairo_line_to(droite, 0.5*ew, 0.7*eh);
    cairo_curve_to(droite, 0.65*ew, 0.4*eh, 0.8*ew, 0.4*eh, 0.85*ew, 0.7*eh);
    cairo_line_to(droite, ew, 0.7*eh);
    cairo_line_to(droite, 1.5*ew, 0.25*eh);
    cairo_line_to(droite, 1.7*ew, 0.25*eh);
    cairo_move_to(droite, 1.7*ew, 0.75*eh);
    cairo_line_to(droite, 2*ew, 0.75*eh);
    cairo_curve_to(droite, 2.1*ew, 0.6*eh, 2.3*ew, 0.4*ew, 2.5*ew, 0.25*eh);
    cairo_line_to(droite, 2.7*ew, 0.25*eh);
    cairo_move_to(droite, 2.8*ew, 0.7*eh);
    cairo_curve_to(droite, 2.9*ew, 0.6*eh, 3.1*ew, 0.4*eh, 3.3*ew, 0.3*eh);
    cairo_line_to(droite, 3.5*ew, 0.75*eh);
    cairo_line_to(droite, X_FIN + ew/5, 0.75*eh);
    cairo_move_to(droite, 3.7*ew, 0.3*eh);
    cairo_line_to(droite, 3.8*ew, 0.3*eh);
    cairo_move_to(droite, 4.7*ew, 0.3*eh);
    cairo_line_to(droite, 4.8*ew, 0.3*eh);
    cairo_move_to(droite, 5.7*ew, 0.3*eh);
    cairo_line_to(droite, 5.8*ew, 0.3*eh);
    cairo_move_to(droite, 6.7*ew, 0.3*eh);
    cairo_line_to(droite, 6.8*ew, 0.3*eh);
    cairo_move_to(droite, 7.7*ew, 0.3*eh);
    cairo_line_to(droite, 7.8*ew, 0.3*eh);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 2.7*ew, 0.7*eh);
    cairo_line_to(droite, 2.8*ew, 0.7*eh);
    cairo_move_to(droite, 3.4*ew, 0.5*eh);
    cairo_line_to(droite, 3.45*ew, 0.6*eh);
    cairo_stroke(droite);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain2(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 0.7*eh);
    cairo_line_to(droite, 0.3*ew, 0.7*eh);
    cairo_curve_to(droite, 0.45*ew, 0.8*eh, 0.6*ew, 0.05*eh, 0.75*ew, 0.2*eh);
    cairo_line_to(droite, 0.9*ew, 0.2*eh);
    cairo_move_to(droite, 0.95*ew, 0.4*eh);
    cairo_line_to(droite, 1.15*ew, 0.6*eh);
    cairo_curve_to(droite, 1.25*ew, 0.3*eh, 1.35*ew, 0.35*eh, 1.5*ew, 0.6*eh);
    cairo_curve_to(droite, 1.65*ew, 0.6*eh, 1.8*ew, 0.3*eh, 2*ew, 0.2*eh);
    cairo_line_to(droite, 2.5*ew, 0.2*eh);
    cairo_move_to(droite, 2.45*ew, 0.6*eh);
    cairo_line_to(droite, X_FIN + 200.5, 0.6*eh);
    cairo_move_to(droite, 1.8*ew, 0.36*eh);
    cairo_line_to(droite, 1.8*ew, 0.30*eh);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 0.8*ew, 0.4*eh);
    cairo_line_to(droite, 0.95*ew, 0.4*eh);
    cairo_move_to(droite, 1.05*ew, 0.5*eh);
    cairo_line_to(droite, 1.15*ew, 0.6*eh);
    cairo_move_to(droite, 3.2*ew, 0.6*eh);
    cairo_line_to(droite, 3.5*ew, 0.6*eh);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain3(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 0.7*eh);
    cairo_line_to(droite, 0.5*ew, 0.7*eh);
    cairo_curve_to(droite, 0.65*ew, 0.4*eh, 0.8*ew, 0.4*eh, 0.85*ew, 0.7*eh);
    cairo_line_to(droite, ew, 0.7*eh);
    cairo_line_to(droite, 1.5*ew, 0.25*eh);
    cairo_line_to(droite, 1.7*ew, 0.25*eh);
    cairo_move_to(droite, 1.7*ew, 0.75*eh);
    cairo_line_to(droite, 2*ew, 0.75*eh);
    cairo_curve_to(droite, 2.1*ew, 0.6*eh, 2.3*ew, 0.4*ew, 2.5*ew, 0.25*eh);
    cairo_line_to(droite, 2.7*ew, 0.25*eh);
    cairo_move_to(droite, 2.8*ew, 0.7*eh);
    cairo_curve_to(droite, 2.9*ew, 0.6*eh, 3.1*ew, 0.4*eh, 3.3*ew, 0.3*eh);
    cairo_line_to(droite, 3.5*ew, 0.75*eh);
    cairo_line_to(droite, X_FIN + ew/5, 0.75*eh);
    cairo_move_to(droite, 3.7*ew, 0.3*eh);
    cairo_line_to(droite, 3.8*ew, 0.3*eh);
    cairo_move_to(droite, 4.7*ew, 0.3*eh);
    cairo_line_to(droite, 4.8*ew, 0.3*eh);
    cairo_move_to(droite, 5.7*ew, 0.3*eh);
    cairo_line_to(droite, 5.8*ew, 0.3*eh);
    cairo_move_to(droite, 6.7*ew, 0.3*eh);
    cairo_line_to(droite, 6.8*ew, 0.3*eh);
    cairo_move_to(droite, 7.7*ew, 0.3*eh);
    cairo_line_to(droite, 7.8*ew, 0.3*eh);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 2.7*ew, 0.7*eh);
    cairo_line_to(droite, 2.8*ew, 0.7*eh);
    cairo_move_to(droite, 3.4*ew, 0.5*eh);
    cairo_line_to(droite, 3.45*ew, 0.6*eh);
    cairo_stroke(droite);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain4(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 0.7*eh);
    cairo_line_to(droite, 0.5*ew, 0.7*eh);
    cairo_curve_to(droite, 0.65*ew, 0.4*eh, 0.8*ew, 0.4*eh, 0.85*ew, 0.7*eh);
    cairo_line_to(droite, ew, 0.7*eh);
    cairo_line_to(droite, 1.5*ew, 0.25*eh);
    cairo_line_to(droite, 1.7*ew, 0.25*eh);
    cairo_move_to(droite, 1.7*ew, 0.75*eh);
    cairo_line_to(droite, 2*ew, 0.75*eh);
    cairo_curve_to(droite, 2.1*ew, 0.6*eh, 2.3*ew, 0.4*ew, 2.5*ew, 0.25*eh);
    cairo_line_to(droite, 2.7*ew, 0.25*eh);
    cairo_move_to(droite, 2.8*ew, 0.7*eh);
    cairo_curve_to(droite, 2.9*ew, 0.6*eh, 3.1*ew, 0.4*eh, 3.3*ew, 0.3*eh);
    cairo_line_to(droite, 3.5*ew, 0.75*eh);
    cairo_line_to(droite, X_FIN + ew/5, 0.75*eh);
    cairo_move_to(droite, 3.7*ew, 0.3*eh);
    cairo_line_to(droite, 3.8*ew, 0.3*eh);
    cairo_move_to(droite, 4.7*ew, 0.3*eh);
    cairo_line_to(droite, 4.8*ew, 0.3*eh);
    cairo_move_to(droite, 5.7*ew, 0.3*eh);
    cairo_line_to(droite, 5.8*ew, 0.3*eh);
    cairo_move_to(droite, 6.7*ew, 0.3*eh);
    cairo_line_to(droite, 6.8*ew, 0.3*eh);
    cairo_move_to(droite, 7.7*ew, 0.3*eh);
    cairo_line_to(droite, 7.8*ew, 0.3*eh);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 2.7*ew, 0.7*eh);
    cairo_line_to(droite, 2.8*ew, 0.7*eh);
    cairo_move_to(droite, 3.4*ew, 0.5*eh);
    cairo_line_to(droite, 3.45*ew, 0.6*eh);
    cairo_stroke(droite);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain5(cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 0.7*eh);
    cairo_line_to(droite, 0.5*ew, 0.7*eh);
    cairo_curve_to(droite, 0.65*ew, 0.4*eh, 0.8*ew, 0.4*eh, 0.85*ew, 0.7*eh);
    cairo_line_to(droite, ew, 0.7*eh);
    cairo_line_to(droite, 1.5*ew, 0.25*eh);
    cairo_line_to(droite, 1.7*ew, 0.25*eh);
    cairo_move_to(droite, 1.7*ew, 0.75*eh);
    cairo_line_to(droite, 2*ew, 0.75*eh);
    cairo_curve_to(droite, 2.1*ew, 0.6*eh, 2.3*ew, 0.4*ew, 2.5*ew, 0.25*eh);
    cairo_line_to(droite, 2.7*ew, 0.25*eh);
    cairo_move_to(droite, 2.8*ew, 0.7*eh);
    cairo_curve_to(droite, 2.9*ew, 0.6*eh, 3.1*ew, 0.4*eh, 3.3*ew, 0.3*eh);
    cairo_line_to(droite, 3.5*ew, 0.75*eh);
    cairo_line_to(droite, X_FIN + ew/5, 0.75*eh);
    cairo_move_to(droite, 3.7*ew, 0.3*eh);
    cairo_line_to(droite, 3.8*ew, 0.3*eh);
    cairo_move_to(droite, 4.7*ew, 0.3*eh);
    cairo_line_to(droite, 4.8*ew, 0.3*eh);
    cairo_move_to(droite, 5.7*ew, 0.3*eh);
    cairo_line_to(droite, 5.8*ew, 0.3*eh);
    cairo_move_to(droite, 6.7*ew, 0.3*eh);
    cairo_line_to(droite, 6.8*ew, 0.3*eh);
    cairo_move_to(droite, 7.7*ew, 0.3*eh);
    cairo_line_to(droite, 7.8*ew, 0.3*eh);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 2.7*ew, 0.7*eh);
    cairo_line_to(droite, 2.8*ew, 0.7*eh);
    cairo_move_to(droite, 3.4*ew, 0.5*eh);
    cairo_line_to(droite, 3.45*ew, 0.6*eh);
    cairo_stroke(droite);
    //cairo_curve_to(droite, ew, 400., ew, 400., ew, 400.);
    //cairo_line_to(droite, ew-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

/**
 * \brief Scanne le fichier donné en entrée
 *
 * Le fichier scanné est le fichier de sauvegarde de l'éditeur de terrain
 * \param fichier Le fichier de sauvegarde à scanner
 * \param x1,x2,y1,y2,x2,y3 Les différents points des structures vectorielles à tracer
 */
int scanner(FILE *fichier, int* x1, int* y1, int* x2, int* y2, int* x3, int* y3) {
    int result;
    fscanf(fichier, "%d %d ", x1, y1);
    if (*x1>=0) {
        fscanf(fichier, "%d %d ", x2, y2);
        if (*x2>=0) {
            fscanf(fichier, "%d %d ", x3, y3);
            if (*x3>=0)
                return 1;
            else    result = *x3;
        }
        else    result = *x2;
    }
    else    result = *x1;
    return result;
}

/**
 * \brief le terrain 6 correspond au terrain créé via l'éditeur de terrain qui est ici chargé
 */
void tterrain6(cairo_surface_t * surfaceFond) {
	FILE *fichier = fopen("save.txt", "r");
    if (fichier != NULL){
        int x1, y1, x2, y2, x3, y3;

        int result;
        int continuer = 1;

        cairo_t *droite = cairo_create(surfaceFond);
        cairo_set_line_width(droite,EPAISSEUR_TRAIT);
        cairo_set_source_rgba (droite, 0, 0, 0, 1);

        fscanf(fichier, "%d %d %d %d ", &x1, &y1, &x2, &y2);
        if((x1 == -2)&&(y1 == -2)){ //on vérifie que le fichier ouvert est bien un fichier de sauvegarde
            fscanf(fichier, "%d %d", &x1, &y1);
            cairo_move_to(droite, x1, y1);
            while (continuer==1) {
                result = scanner(fichier, &x1, &y1, &x2, &y2, &x3, &y3);
                switch(result) {
                    case 1:
                        cairo_curve_to(droite, x1, y1, x3, y3, x2, y2);
                        break;
                    case -1:
                        fscanf(fichier, "%d %d ", &x2, &y2);
                        cairo_move_to(droite, x2, y2);
                        break;
                    case -2:
                        cairo_stroke(droite);
                        cairo_set_source_rgba (droite, 1, 0, 0, 1);
                        break;
                    case -3:
                        cairo_stroke(droite);
                        cairo_destroy(droite);
                        continuer = 0;
                        break;
                }
            }
        }
    }
}

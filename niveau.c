/**
 * \file      	niveau.c
 * \version   	1.0
 * \date      	2013
 * \brief		Gestion des niveaux
 *
 * \details   	Permet de charger un niveau en particulier et g�re son d�filement
 * \details		ainsi que les diff�rentes int�ractions entre le joueur et le terrain
*/

#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
 * \param H_PERSO d�finit la hauteur du personnage � l'�cran
 * \param L_PERSO d�finit la largeur du personnage � l'�cran
 * \param EPAISSEUR_TRAIT d�finit l'�paisseur par d�faut des traits trac�s � l'�cran
 * \param X_FIN d�finit l'abscisse de fin de niveau
 * \param NOMBRE_ECRANS d�finit la largeur, en nombre d'�crans, de chaque niveau
 */
int H_PERSO, L_PERSO, EPAISSEUR_TRAIT, X_FIN, NOMBRE_ECRANS;

/**
 * \brief	Gestion globale d'un niveau
 *
 *	Cette fonction g�re l'affichage d'un niveau
 *	ainsi que les int�ractions utilisateurs avec ce dernier
 *	\param ecran La surface visible � l'�cran sur laquelle est affich�e la partie courante du niveau
 *	\param choixTerrain Le niveau s�lectionn� dans le menu de s�lection du niveau
 *	\return Un \e int correspondant aux diff�rentes fins possibles du niveaux (victoire, mort, ...)
 */
int niveau(SDL_Surface *ecran, int choixTerrain){

    Uint32 Blanc = 0xFFFFFFFF;

    int continuer = 1, select = 0;
    int enSelection = 0, dejaSelectionne = 0, collagePossible = 0, enDrag = 0, chute = 0, enRotation = 0; //d�finition des diff�rentes constantes indiquant un �tat particulier (en s�lection ,en d�placement, ...)

	Uint32 temps_precedent = SDL_GetTicks(), temps_actuel = SDL_GetTicks(); //constantes temporelles, permettant de g�rer le rafraichissement

	int xSouris = 0, ySouris = 0;
    int xSourisButton = 0, ySourisButton = 0;
    int xRinit = 0, yRinit = 0; //d�finitions de diff�rentes positions utilis�es pour les calcul d'angle, de d�placement, ...

    double longueur = 0., longueur_proj = 0., angle = 0., angleTotal = 0., scale = 1.; // initialisation de diff�rents param�tres g�om�triques

    H_PERSO = 110*eh/768;
    L_PERSO = 55*eh/768;
    EPAISSEUR_TRAIT = 7*eh/768;
    NOMBRE_ECRANS = 8;
    X_FIN = (NOMBRE_ECRANS - 4)*ew - ew/5; //initialisations des constantes globales d�finit en d�but de fichier

    int k = 0, k1 = 0; //variables permettant la gestion de l'animation du personnage


    SDL_Surface *surfPerso = NULL, *surfPause = NULL, *surfLigne = NULL, *rect=NULL, *surfSelec=NULL;
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, NOMBRE_ECRANS*ew, eh, 32, 0, 0, 0, 0);
    surfSelec = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, L_PERSO, H_PERSO, 32, 0, 0, 0, 0);
    surfPause = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0); //initialisation des diff�rentes surface utilis�es pour afficher un niveau

    SDL_Rect posPointille, pos, posligne, pospersoNiveau, pospause, posrec, selecNiveau, posSelection, posDestination, posAppercu, posPersoEcran;
    pos.x = selecNiveau.x = posligne.x = posSelection.x = posDestination.x = posAppercu.x = 0;
    pos.y = selecNiveau.y = posligne.y = posSelection.y = posDestination.y = posAppercu.y = 0;
    selecNiveau.h = eh;
    selecNiveau.w = ew;
    pospause.x = ew - 70*eh/768;
    pospause.y = 0;
    pospersoNiveau.x = ew/6;
    pospersoNiveau.y = 0.7*eh - 2*H_PERSO;
    posPersoEcran.x = pospersoNiveau.x - selecNiveau.x;
    posPersoEcran.y = pospersoNiveau.y; //d�finition et initialisation des diff�rentes positions de collage des diff�rentes surfaces

    cairo_surface_t *surfaceFond;
    rect = selection(60*eh/768, 60*eh/768, surfLigne->format);
    SDL_Event event;
    SDL_FillRect(ecran, NULL, Blanc);
    SDL_FillRect(surfSelec, NULL, Blanc);
    SDL_FillRect(surfPause, NULL, Blanc);
    SDL_FillRect(surfPerso, NULL, Blanc);
    SDL_FillRect(surfLigne, NULL, Blanc); //remplissage des surfaces

    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch); // Cr�ation d'une surface cairo (permettant les trac�s vectorils) ayant pour format d'affichage celui d'une surface SDL

    chargerTerrain(surfaceFond, choixTerrain); //Chargement du terrain s�lectionn�
    pperso(surfPerso, k); //Initialisation du personnage
    afficherTexte(surfPause, "ariblk.ttf", 60*eh/768, "II", 0, 0); //affichage du boutton pause
    SDL_SetAlpha(rect, SDL_SRCALPHA, 0); //gestion de la transparence
    SDL_EnableKeyRepeat(0,0); //d�sactivation de la persistance automatique des touches
    SDL_Flip(ecran); //rafraichissement de l'�cran visible
    while(continuer!=0){
        SDL_FillRect(surfSelec, NULL, Blanc);
        continuer = avancer(&pospersoNiveau, surfLigne, selecNiveau); //appel de la fonction g�rant le progression du personnage (prise en compte des obstacle, mont�e, descente, chutte)
		if(continuer == 0) //Le personnage est bloqu� et est au bord de l'�cran
			return (-1); //gameOver
        if (continuer == -1) //Le personnage a atteint X_FIN
            return 0; //Victoire
        if(continuer!=2){ //Le personnage avance normalement (il monte ou descend)
            if (chute >= 2*H_PERSO){
				return -1;
            }
				else {chute = 0;}
			k1++;
			k1 = k1%4;
			if (k1==3){
            k++;
            k = k%9;}
            selecNiveau.x = selecNiveau.x + 4;
			}
            else {chute +=5;} //Le personnage est en train de tomber

        if(enSelection){
            posPointille = pointilleSelection(surfSelec, selecNiveau, posSelection, xSouris, ySouris);
        }
        else if(dejaSelectionne){
            posPointille = pointilleSelection(surfSelec, selecNiveau, posSelection, -1, -1);
        }
        SDL_BlitSurface(surfLigne, &selecNiveau, ecran, NULL);
        if(posPointille.x >= 0){
            insererSurface(surfSelec, NULL, ecran, NULL);
        }
        SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
        if(enDrag){
            if(!enRotation){
                posAppercu.x =  xSouris - xRinit;
                posAppercu.y =  ySouris - yRinit;
            }
            else{
                longueur_proj = (posAppercu.y + posAppercu.h) - ySouris;
                longueur = sqrt((longueur_proj*longueur_proj) + ((xSouris-posAppercu.x)*(xSouris-posAppercu.x)));
                if(xSouris > posAppercu.x)
                    angle = mod_d(angleTotal + acos(longueur_proj/longueur), M_PI);
                else angle = mod_d(angleTotal - acos(longueur_proj/longueur), M_PI);
            }
            decouperColler(1, ecran, surfaceFond, posSelection, posAppercu, angle, scale);
        }
        if(collagePossible){
            decouperColler(0, surfLigne, surfaceFond, posSelection, posDestination, angleTotal, scale);
            collagePossible = 0;
            enSelection = 0;
            dejaSelectionne = 0;
            enDrag = 0;
            angle = 0;
            angleTotal = 0;
            scale = 1;
        } // Gestion des diff�rents cas d'interaction (l'utilisateur a selectionn� une zone du terrain , la d�place, la colle, la redimensionne ou la fait tourner

        posPersoEcran.x = pospersoNiveau.x - selecNiveau.x;
        posPersoEcran.y = pospersoNiveau.y;
        pperso(surfPerso, k);
        insererSurface(surfPerso, NULL, ecran, &posPersoEcran);
        SDL_BlitSurface(rect, NULL, ecran, &posrec);
        SDL_Flip(ecran); //actualisation des positions relatives du personnage et du niveau par rapot � l'�cran et rafraichissement
        temps_actuel = SDL_GetTicks();
        SDL_Delay(max(40 - (temps_actuel-temps_precedent),0)); //gestion du nombre d'images affich�es par seconde
        temps_precedent = temps_actuel;
        while (SDL_PollEvent(&event)) { //gestion des diff�rentes actions de l'utilisateur
            switch(event.type){
                case SDL_MOUSEMOTION: //mouvement de la souris
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if(((ew - 77*eh/768) <= xSouris) && (ySouris < 76*eh/768)){
                        SDL_SetAlpha(rect, SDL_SRCALPHA, 120);
                        select = 0;
                        posrec.x=ew - 77*eh/768;
                        posrec.y=14*eh/768;
                    }
                    else { SDL_SetAlpha(rect, SDL_SRCALPHA, 0);}
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    xSourisButton = event.button.x;
                    ySourisButton = event.button.y;
                    if(event.button.button == SDL_BUTTON_LEFT){ //clic gauche enfonc�, qui permet la selection d'une zone de l'�cran
                        xSourisButton = xSourisButton + selecNiveau.x;
                        ySourisButton = ySourisButton + selecNiveau.y;
                        if(enSelection == 0){
                            enDrag = 0;
                            dejaSelectionne = 0;
                            enSelection = 1;
                            posSelection.x = xSourisButton;
                            posSelection.y = ySourisButton;

                        }
                    }
                    else if(event.button.button == SDL_BUTTON_RIGHT){ //clic droit enfonc�, qui permet de d�placer une zone s�lectionn�e
                        if(!enDrag){
                            xRinit = xSourisButton - xSouris;
                            yRinit = ySourisButton - ySouris;
                            if(dejaSelectionne){
                                enDrag=1;
                                posAppercu = posSelection;
                                posAppercu.x = posSelection.x - selecNiveau.x;
                                posAppercu.y = posSelection.y - selecNiveau.y;
                            }
                        }
                    }
                    else if(event.button.button == SDL_BUTTON_WHEELDOWN){ //utilisation de la molette qui permet de redimensionner un objet en cours de selection
                        if(enDrag){
                            scale = scale - 0.05;
                            scale = max_d(scale, 0.5);
                        }
                    }
                    else if(event.button.button == SDL_BUTTON_WHEELUP){
                        if(enDrag){
                            scale = scale + 0.05;
                            scale = min_d(scale, 1.8);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    xSourisButton = event.button.x;
                    ySourisButton = event.button.y;
                    if(event.button.button == SDL_BUTTON_LEFT){ //clic gauche relach�, qui permet de mettre le jeu en pause si la souris est sur le boutton ou de terminer la selection d'une zone de l'�cran
                        if(((ew - 77*eh/768) <= xSourisButton) && (ySourisButton < 76*eh/768)){
                            switch (jeupause(ecran)){
                                case SORTIE:
                                    return SORTIE;
                                    break;
                                case MENU:
                                    return MENU;
                                    break;
                                case 0:
                                    event.button.button=SDL_BUTTON_MIDDLE;
                                    SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
                                    collagePossible = 0;
                                    enSelection = 0;
                                    dejaSelectionne = 0;
                                    enDrag = 0;
                                    angle = 0;
                                    angleTotal = 0;
                                    scale = 1;
                                    break;
                                default:
                                    break;
                                }
                        }
                        else{
                            xSourisButton = xSourisButton + selecNiveau.x;
                            ySourisButton = ySourisButton + selecNiveau.y;
                            if(collagePossible == 0){
                                if(enSelection){
                                    posSelection.w = abs(posSelection.x-xSourisButton);
                                    posSelection.h = abs(posSelection.y-ySourisButton);
                                    posSelection.x = min(posSelection.x, xSourisButton);
                                    posSelection.y = min(posSelection.y, ySourisButton);
                                    enSelection = 0;
                                    dejaSelectionne = 1;
                                }
                            }
                        }
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT){ //clic droit relach�, qui permet de coller � l'�cran un objet en cours de d�placement
                        enRotation  = 0;
                        xSourisButton = xSourisButton + selecNiveau.x;
                        ySourisButton = ySourisButton + selecNiveau.y;
                        if(enDrag){
                            enDrag=0;
                            if(dejaSelectionne){
                                collagePossible = 1;
                                posDestination = posAppercu;
                                posDestination.x = posAppercu.x + selecNiveau.x;
                                posDestination.y = posAppercu.y + selecNiveau.y;
                            }
                        }
                    }
                    break;
                case SDL_KEYDOWN: //une touche est press�e
                    switch(event.key.keysym.sym){
                        case SDLK_ESCAPE: //appuis sur la touche echap, qui met le jeu en pause
                            switch (jeupause(ecran)){ //gestion des retours de la pause
                                case SORTIE: //sortie de jeu, cela ferme l'application
                                    return SORTIE;
                                    break;
                                case MENU: //retour au menu
                                    return MENU;
                                    break;
                                case 0: //retour au jeu, on r�initialise les param�tres de selction
                                    collagePossible = 0;
                                    enSelection = 0;
                                    dejaSelectionne = 0;
                                    enDrag = 0;
                                    angle = 0;
                                    angleTotal = 0;
                                    scale = 1;
                                    event.key.keysym.sym = SDLK_a;
                                    break;
                                default:
                                    break;
                                }
                            break;
                            case SDLK_r: //appuis sur r, qui permet d'initier une rotation
                            if(enDrag)
                                enRotation = 1;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP: //une touche est relach�e
                    switch(event.key.keysym.sym){
                        case SDLK_r: //r, termine la d�finition de l'angle de la rotation
                            if(enRotation){
                                angleTotal = angle;
                            }
                            enRotation = 0;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT: //le programme est ferm� manuellement (via la croix ou le clic droit -> ferm�)
                    return SORTIE;
                    break;
                default:
                    break;
            }
        }
    }
    return MENU;
}

/**
 * \brief G�re l'affichage et l'animation du personnage
 *
 * \param surfPerso Surface SDL sur laquelle est coll� le perso avant d'�tre affich� � l'�cran
 * \param k Variable d�finissant l'animation courante du personnage � afficher
 * \return void
 */
void pperso(SDL_Surface *surfPerso, int k)
{
    cairo_surface_t *surface = NULL;
    surface = cairo_image_surface_create_for_data (surfPerso->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfPerso->w,
                                                      surfPerso->h,
                                                      surfPerso->pitch);

      cairo_t *perso = cairo_create(surface);
    cairo_set_line_width(perso, EPAISSEUR_TRAIT);
    cairo_set_source_rgb(perso, 1, 1, 1);
    cairo_rectangle(perso, 0, 0, L_PERSO, H_PERSO);
    cairo_fill(perso);
    cairo_set_source_rgba (perso, 0, 0, 0, 1);
    switch (k){
        case 0:
            cairo_arc(perso, 31*eh/768, 19*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, 28*eh/768, 28*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, 28*eh/768, 30*eh/768);
            cairo_line_to(perso, 15*eh/768, 45*eh/768);
            cairo_line_to(perso, 12*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, 32*eh/768, 32*eh/768);
            cairo_line_to(perso, 36*eh/768, 46*eh/768);
            cairo_line_to(perso, 48*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, 25*eh/768, 70*eh/768);
            cairo_line_to(perso, 17*eh/768, 90*eh/768);
            cairo_line_to(perso, 3*eh/768, 133*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, 27*eh/768, 72*eh/768);
            cairo_line_to(perso, 38*eh/768, 92*eh/768);
            cairo_line_to(perso, 55*eh/768, 135*eh/768);
            cairo_stroke_preserve(perso);
        break;
        case 1:
           cairo_arc(perso, (31+1)*eh/768, 19*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, 28*eh/768, 28*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, 28*eh/768, 30*eh/768);
            cairo_line_to(perso, (15+2)*eh/768, (45+1)*eh/768);
            cairo_line_to(perso, (12+7)*eh/768, (60+1)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, 32*eh/768, 32*eh/768);
            cairo_line_to(perso, (36-2)*eh/768, (46-1)*eh/768);
            cairo_line_to(perso, (48-3)*eh/768, (60-1)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+1)*eh/768, (70+1)*eh/768);
            cairo_line_to(perso, (17+3)*eh/768, (90+1)*eh/768);
            cairo_line_to(perso, (3+5)*eh/768, (133-8)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-1)*eh/768, (72-1)*eh/768);
            cairo_line_to(perso, (38-3)*eh/768, (92-1)*eh/768);
            cairo_line_to(perso, (55-7)*eh/768, (135-2)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 2:
           cairo_arc(perso, (31+1)*eh/768, (19-1)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, (28+1)*eh/768, 28*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+1)*eh/768, 30*eh/768);
            cairo_line_to(perso, (15+4)*eh/768, (45+2)*eh/768);
            cairo_line_to(perso, (12+14)*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-1)*eh/768, 32*eh/768);
            cairo_line_to(perso, (36-4)*eh/768, (46-2)*eh/768);
            cairo_line_to(perso, (48-5)*eh/768, (60-2)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+1)*eh/768, (70+1)*eh/768);
            cairo_line_to(perso, (17+8)*eh/768, (90+1)*eh/768);
            cairo_line_to(perso, (3+17)*eh/768, (133-12)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-1)*eh/768, (72-1)*eh/768);
            cairo_line_to(perso, (38-8)*eh/768, (92-1)*eh/768);
            cairo_line_to(perso, (55-17)*eh/768, (135+2)*eh/768);
            cairo_stroke_preserve(perso);
    break;
     case 3:
           cairo_arc(perso, (31+2)*eh/768, (19-1)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, (28+1)*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+1)*eh/768, (30+1)*eh/768);
            cairo_line_to(perso, (15+6)*eh/768, (45+3)*eh/768);
            cairo_line_to(perso, (12+20)*eh/768, (60-1)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-1)*eh/768, (32-1)*eh/768);
            cairo_line_to(perso, (36-6)*eh/768, (46-3)*eh/768);
            cairo_line_to(perso, (48-11)*eh/768, (60-1)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+2)*eh/768, (70+2)*eh/768);
            cairo_line_to(perso, (17+15)*eh/768, (90+2)*eh/768);
            cairo_line_to(perso, (3+26)*eh/768, (133-12)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-2)*eh/768, (72-2)*eh/768);
            cairo_line_to(perso, (38-15)*eh/768, (92-2)*eh/768);
            cairo_line_to(perso, (55-29)*eh/768, (135+4)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 4:
           cairo_arc(perso, (31+3)*eh/768, (19-1)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, (28+2)*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+2)*eh/768, (30+1)*eh/768);
            cairo_line_to(perso, (15+8)*eh/768, (45+4)*eh/768);
            cairo_line_to(perso, (12+25)*eh/768, (60-2)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-2)*eh/768, (32-1)*eh/768);
            cairo_line_to(perso, (36-8)*eh/768, (46-4)*eh/768);
            cairo_line_to(perso, (48-18)*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+2)*eh/768, (70+2)*eh/768);
            cairo_line_to(perso, (17+17)*eh/768, (90+2)*eh/768);
            cairo_line_to(perso, (3+42)*eh/768, (133-6)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-2)*eh/768, (72-2)*eh/768);
            cairo_line_to(perso, (38-17)*eh/768, (92-2)*eh/768);
            cairo_line_to(perso, (55-42)*eh/768, (135+6)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 5:
           cairo_arc(perso, (31+4)*eh/768, (19-2)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, (28+2)*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+3)*eh/768, (30+2)*eh/768);
            cairo_line_to(perso, (15+10)*eh/768, (45+5)*eh/768);
            cairo_line_to(perso, (12+18)*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-3)*eh/768, (32-2)*eh/768);
            cairo_line_to(perso, (36-10)*eh/768, (46-5)*eh/768);
            cairo_line_to(perso, (48-25)*eh/768, (60+2)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+2)*eh/768, (70+2)*eh/768);
            cairo_line_to(perso, (17+20)*eh/768, (90+2)*eh/768);
            cairo_line_to(perso, (3+38)*eh/768, (133-4)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-2)*eh/768, (72-2)*eh/768);
            cairo_line_to(perso, (38-20)*eh/768, (92-2)*eh/768);
            cairo_line_to(perso, (55-52)*eh/768, (135+12)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 6:
           cairo_arc(perso, (31+3)*eh/768, (19-2)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, (28+1)*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+2)*eh/768, (30+1)*eh/768);
            cairo_line_to(perso, (15+8)*eh/768, (45+4)*eh/768);
            cairo_line_to(perso, (12+11)*eh/768, (60+1)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-2)*eh/768, (32-1)*eh/768);
            cairo_line_to(perso, (36-8)*eh/768, (46-4)*eh/768);
            cairo_line_to(perso, (48-20)*eh/768, (60+1)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+2)*eh/768, (70+2)*eh/768);
            cairo_line_to(perso, (17+15)*eh/768, (90+2)*eh/768);
            cairo_line_to(perso, (3+27)*eh/768, (133-2)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-2)*eh/768, (72-2)*eh/768);
            cairo_line_to(perso, (38-15)*eh/768, (92-2)*eh/768);
            cairo_line_to(perso, (55-32)*eh/768, (135+12)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 7:
           cairo_arc(perso, (31+2)*eh/768, (19-2)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, 28*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+1)*eh/768, (30+1)*eh/768);
            cairo_line_to(perso, (15+6)*eh/768, (45+3)*eh/768);
            cairo_line_to(perso, (12+5)*eh/768, (60+2)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-1)*eh/768, (32-1)*eh/768);
            cairo_line_to(perso, (36-6)*eh/768, (46-3)*eh/768);
            cairo_line_to(perso, (48-14)*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+1)*eh/768, (70+1)*eh/768);
            cairo_line_to(perso, (17+8)*eh/768, (90+1)*eh/768);
            cairo_line_to(perso, (3+17)*eh/768, 133*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-1)*eh/768, (72-1)*eh/768);
            cairo_line_to(perso, (38-8)*eh/768, (92-1)*eh/768);
            cairo_line_to(perso, (55-17)*eh/768, (135+8)*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 8:
           cairo_arc(perso, (31+2)*eh/768, (19-1)*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, 28*eh/768, (28+1)*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, (28+1)*eh/768, 30*eh/768);
            cairo_line_to(perso, (15+4)*eh/768, (45+2)*eh/768);
            cairo_line_to(perso, (12+3)*eh/768, (60+1)*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, (32-1)*eh/768, 32*eh/768);
            cairo_line_to(perso, (36-4)*eh/768, (46-2)*eh/768);
            cairo_line_to(perso, (48-7)*eh/768, (60-1)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, (25+1)*eh/768, (70+1)*eh/768);
            cairo_line_to(perso, (17+3)*eh/768, (90+1)*eh/768);
            cairo_line_to(perso, (3+7)*eh/768, (133+2)*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, (27-1)*eh/768, (72-1)*eh/768);
            cairo_line_to(perso, (38-3)*eh/768, (92-1)*eh/768);
            cairo_line_to(perso, (55-5)*eh/768, 135*eh/768);
            cairo_stroke_preserve(perso);
    break;
    case 9:
           cairo_arc(perso, (31+1)*eh/768, 19*eh/768, 10*eh/768, 0, 2*M_PI);
            cairo_fill_preserve(perso);
            cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
            //Corps
            cairo_move_to(perso, 28*eh/768, 28*eh/768);
            cairo_line_to(perso, 26*eh/768, 68*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 1
            cairo_move_to(perso, 28*eh/768, 30*eh/768);
            cairo_line_to(perso, (15+2)*eh/768, (45+1)*eh/768);
            cairo_line_to(perso, 12*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Bras 2
            cairo_move_to(perso, 32*eh/768, 32*eh/768);
            cairo_line_to(perso, (36-2)*eh/768, (46-1)*eh/768);
            cairo_line_to(perso, 48*eh/768, 60*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 1
            cairo_move_to(perso, 25*eh/768, 70*eh/768);
            cairo_line_to(perso, 17*eh/768, 90*eh/768);
            cairo_line_to(perso, 3*eh/768, 133*eh/768);
            cairo_stroke_preserve(perso);
            //Jambe 2
            cairo_move_to(perso, 27*eh/768, 72*eh/768);
            cairo_line_to(perso, 38*eh/768, 92*eh/768);
            cairo_line_to(perso, 55*eh/768, 135*eh/768);
            cairo_stroke_preserve(perso);
    break;
    }
}

/**
 * \brief retourne le minimum de deux entiers a et b
 */
int min(int a, int b){
    if(a <= b)
        return a;
    return b;
}

/**
 * \brief retourne le maximum de deux entiers a et b
 */
int max(int a, int b){
    if(a <= b)
        return b;
    return a;
}

/**
 * \brief retourne le minimum de deux r�els a et b
 */
double min_d(double a, double b){
    if(a <= b)
        return a;
    return b;
}

/**
 * \brief retourne le maximum de deux r�els a et b
 */
double max_d(double a, double b){
    if(a <= b)
        return b;
    return a;
}

/**
 * \brief retourne le modulo de deux r�els a et b
 */
double mod_d(double a, double b){

    int q = (int)(a/b);

    return (a - q*b);
}

/**
 * \brief Cette fonction g�re l'insertion d'une zone d'une surface dans une autre
 *
 * Elle permet d'afficher une zone du terrain en un autre endroit du terrain, en la tournant et l'agrandissant et peut aussi l'y coller
 * \param enAppercu d�termine si la zone s�lectionn�e doit �tre seulement affich�e apr�s avoir �t� modifi�e ou aussi coll�e sur le terrain
 * \param surfLigne l'�cran ou la surface du niveau selon qu'il s'agit d'un apper�u ou non
 * \param surfaceFond le calque vectoriel de surfLigne dans lequel le dessin est r�alis�
 * \param posSelection la zone de terrain selectionn�e
 * \param posDestination la zone de terrain o� l'apper�u doit �tre affich�/coll�
 * \param angle l'angle de la rotation � effectuer
 * \param scale le rapport de l'agrandssement � effectuer
 * \return void Les modifiations sont faites directement sur les objets gr�ces aux pointeurs
 */
void decouperColler(int enAppercu, SDL_Surface *surfLigne, cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination, double angle, double scale){


    Uint32 Blanc = 0xFFFFFFFF;
    SDL_Surface *collage = NULL;
    SDL_Rect posRelative;
    posRelative.x = 0;
    posRelative.y = 0;
    posRelative.w = posSelection.w;
    posRelative.h = posSelection.h; //d�finitions des positions relatives

    double diagonale = scale*sqrt((posSelection.w*(posSelection.w))+(posSelection.h*(posSelection.h)));
    collage = SDL_CreateRGBSurface(SDL_HWSURFACE , diagonale, diagonale, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0); //cr�ation d'une surface de travail interm�diaire
    SDL_FillRect(collage, NULL, Blanc);

    cairo_surface_t *surfCollage = cairo_image_surface_create_for_data (collage->pixels,
                                                      CAIRO_FORMAT_RGB24,
                                                      collage->w,
                                                      collage->h,
                                                      collage->pitch); //cr�ation d'un calque vectoriel

    if((angle == 0) && (scale == 1) &&(!enAppercu))
        recollementContinu(surfLigne, posSelection, &posDestination); //gestion du recolement continue dans le cas d'une simple translation

	cairo_t *enSelect = cairo_create(surfCollage); //cr�ation du chemin de dessin vectoriel

    if((angle <= M_PI_2)&&(angle >= -M_PI_2))
        cairo_translate(enSelect, scale*max_d((posSelection.h)*sin(angle), 0.), -scale*min_d((posSelection.w)*sin(angle), 0));
    else if(angle > M_PI_2)
        cairo_translate(enSelect, scale*(posSelection.w)*cos(M_PI-angle)+scale*(posSelection.h)*cos(angle-M_PI_2), scale*(posSelection.h)*sin(angle - M_PI_2));
    else
        cairo_translate(enSelect,scale*(posSelection.w)*(cos(angle-M_PI)), scale*(posSelection.h)*cos(angle + M_PI) + scale*(posSelection.w)*cos(angle + M_PI_2));
    cairo_scale(enSelect, scale, scale);
    cairo_rotate(enSelect,angle); //gestion de la rotation et du redimensionnement

    cairo_set_source_surface (enSelect, surfaceFond,  0 - posSelection.x, 0 - posSelection.y);
    cairo_rectangle (enSelect, 0, 0, min(posDestination.w,posSelection.w), min(posDestination.h,posSelection.h));
	cairo_fill(enSelect); //remplit le chemin vectoriel avec celui, transform�, de la zone � copier

    posDestination.w = posDestination.h = diagonale;
    insererSurface(collage, NULL, surfLigne, &posDestination); //affiche le r�sultat dans la surface voulue

    cairo_destroy(enSelect);
    cairo_surface_destroy(surfCollage);
    SDL_FreeSurface(collage);
}

/**
 * \brief G�re le recollement continu
 */
void recollementContinu(SDL_Surface *surfLigne, SDL_Rect posSelection, SDL_Rect *posDestination){
    int decallage = 0, trouve = 1, compteur = 0, rel_compteur, continuer = 1;
    while(continuer && (compteur < (min(posSelection.h, posDestination->h) - 16)) && (posDestination->y + compteur < eh)){
        unsigned int pix = getpixel(surfLigne,posSelection.x, posSelection.y + compteur );
        if((pix == 0LL)||(pix == 4278190080LL)){
            rel_compteur = max((compteur - 15),0);
            while((rel_compteur < compteur + 15)&&trouve&&(posDestination->y + rel_compteur < eh)){
                unsigned int pix_int = getpixel(surfLigne, posDestination->x-1, posDestination->y + rel_compteur);
                if((pix_int == 0LL) || (pix_int == 4278190080LL)){
                    continuer = 0;
                    trouve = 0;
                    decallage = (rel_compteur - compteur);
                }
                rel_compteur++;
            }
        }
        compteur++;
    }
    posDestination->y=max(posDestination->y+decallage, 0);
}

/**
 * \brief Affiche des pointill�s de s�lection sur la zone s�lectionn�e
 * \param surfSelec La surface sur laquelle sont affich�s les pointill�s
 * \param selecNiveau D�finit la position courante du niveau visibe � l'�cran
 * \param pos D�finit la position d'origine (ou totale lorsque xSouris et ySouris valent -1), dans le niveau, des pointill�s
 * \param xSouris D�finit l'abscisse de destination des pointill�s
 * \param ySouris D�finit l'ordonn�e de destination des pointill�s
 * \return Un \e SDL_Rect d�finissant le cadra contenant les pontill�s � l'�cran
 */
SDL_Rect pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris){

    cairo_surface_t *surfaceFond;
    SDL_Rect retour, retour_erreur;
    retour_erreur.x = -1;
    retour_erreur.y = -1;
    int x_rel = 0, coinHG_x = 0, coinHG_y = 0, coinHD_x = 0, coinBD_y = 0, coinBG_x = 0, largeur = 0;
    x_rel = pos.x - selecNiveau.x;
    if((xSouris == -1) && (ySouris == -1)){
        coinHG_x = coinBG_x = x_rel;
        coinHG_y = pos.y;
        coinHD_x = coinHG_x + pos.w;
        coinBD_y = pos.y + pos.h;
        largeur = pos.w;
    }
    else{
        coinHG_x = coinBG_x = min(x_rel,xSouris);
        coinHG_y = min(pos.y, ySouris);
        coinHD_x = max(x_rel, xSouris);
        coinBD_y = max(pos.y, ySouris);
        largeur = coinHD_x - coinHG_x;
    }
    if(largeur > 0) {
        retour.x = max(coinHG_x - 5, 0);
        retour.y = max(coinHG_y -5, 0);
        retour.h = min(coinBD_y - coinHG_y + 5, selecNiveau.h - coinHG_y);
        retour.w = min(largeur + 10, selecNiveau.w/NOMBRE_ECRANS - coinHG_x);
        surfaceFond = cairo_image_surface_create_for_data (surfSelec->pixels,
                                                              CAIRO_FORMAT_ARGB32,
                                                              surfSelec->w,
                                                              surfSelec->h,
                                                              surfSelec->pitch);

        cairo_t *ligne = cairo_create(surfaceFond);
        double dashes[] = {10.0,  /* ink */
                           10.0  /* skip*/
                          };
        int    ndash  = sizeof (dashes)/sizeof(dashes[0]);
        double offset = 0.0;
        cairo_set_dash (ligne, dashes, ndash, offset);
        cairo_set_line_width (ligne, 5.0);
        cairo_move_to(ligne, coinHG_x, coinHG_y);
        cairo_line_to(ligne, coinHD_x, coinHG_y);
        cairo_rel_line_to(ligne, 0, coinBD_y-coinHG_y);
        cairo_rel_line_to(ligne, coinBG_x-coinHD_x, 0);
        cairo_rel_line_to(ligne, 0, coinHG_y-coinBD_y);
        cairo_stroke(ligne);
        cairo_destroy(ligne);
        cairo_surface_destroy(surfaceFond);
        return retour;
    }
    return retour_erreur;
}

/**
 * \brief Colle un surface SDL sur une autre en appliquant un transparence au blanc sur la surface � coller
 * \param collage Surface � coller
 * \param posSelection Le rectangle de selection
 * \param surfLigne La surface de destination
 * \param posDestination Le rectangle de destination
 * \return void
 */
void insererSurface(SDL_Surface *collage, SDL_Rect *posSelection, SDL_Surface *surfLigne, SDL_Rect *posDestination){

    int i, j;
    int h, w, x_init_S, y_init_S, x_init_D, y_init_D;
    unsigned int pixel, p_ligne = surfLigne->pitch/4;
    if(posDestination == NULL){
        x_init_D = y_init_D = 0;
    }
    else{
        x_init_D = posDestination->x;
        y_init_D = posDestination->y;
    }
    if(posSelection == NULL){
            x_init_S = y_init_S = 0;
            h = min(collage -> h, surfLigne -> h - y_init_D);
            w = min(collage -> w, surfLigne -> w - x_init_D);
    }
    else {
        x_init_S = posSelection->x;
        y_init_S = posSelection->y;
        h = min(posSelection->h, surfLigne -> h - y_init_D);
        w = min(posSelection->w, surfLigne -> w - x_init_D);
    }
    for(j=0; j<h; j++){
        for(i=0; i<w; i++){
            pixel = getpixel(collage, i + x_init_S, j + y_init_S);
            if(pixel != 0xFFFFFFFF){
                ((unsigned int*)surfLigne->pixels)[(j + y_init_D)*p_ligne + i + x_init_D] = pixel;
            }
        }
    }
}

/**
 * \brief G�re le chargement du terrain en fonction du choix de l'utilisateur
 * \param surfaceFond Calque vectoriel sur lequel le terrain est trac�
 * \param choixTerrain terrain choisit par l'utilisateur
 * \return void
 */
void chargerTerrain(cairo_surface_t *surfaceFond, int choixTerrain){
    switch(choixTerrain){
        case 0:
            tterrain0(surfaceFond);
            break;
        case 1:
            tterrain1(surfaceFond);
            break;
        case 2:
            tterrain2(surfaceFond);
            break;
        case 3:
            tterrain3(surfaceFond);
            break;
        case 4:
            tterrain4(surfaceFond);
            break;
        case 5:
            tterrain5(surfaceFond);
            break;
        case 6:
            tterrain6(surfaceFond);
            break;
        default:
            break;
    }
}


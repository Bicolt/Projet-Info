#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"


int niveau(SDL_Surface *ecran){


    int continuer = 1, select = 0;
    int enSelection = 0, dejaSelectionne = 0, collagePossible = 0, chute = 0;
    int xSouris = 0, ySouris = 0;
    int xSourisButton = 0, ySourisButton = 0;


    SDL_Surface *surfNiveau, *surfPerso = NULL, *surfPause = NULL, *surfLigne = NULL, *rect=NULL, *surfSelec=NULL;
    surfNiveau = SDL_CreateRGBSurface(SDL_HWSURFACE, NOMBRE_ECRANS*ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfSelec = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, L_PERSO, H_PERSO, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, surfNiveau->w, surfNiveau->h, 32, 0, 0, 0, 0);
    surfPause = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0);

    SDL_Rect pos, posligne, pospersoNiveau, pospause, posrec, selecNiveau, posSelection, posDestination;
    pos.x = selecNiveau.x = posligne.x = posSelection.x = posDestination.x = 0;
    pos.y = selecNiveau.y = posligne.y = posSelection.y = posDestination.y = 0;
    selecNiveau.h = ecran -> h;
    selecNiveau.w = ecran -> w;
    pospause.x = ecran->w - 70;
    pospause.y = 0;
    pospersoNiveau.x = 200;
    pospersoNiveau.y = 200;
    cairo_surface_t *surface, *surfaceFond;
    rect = selection(60, 60, surfNiveau->format);
    SDL_Event event;
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfSelec, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfPause, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfPerso, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    // Création d'une surface cairo pour le personnage ayant pour format d'affichage celui d'une surface SDL

    // Création d'une surface cairo ayant pour format d'affichage celui d'une surface SDL
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch);



    cairo_t *droite1 = tterrain(ecran, surfaceFond);
	cairo_t *personnage = pperso(surfNiveau, surface, surfPerso);

    SDL_SetColorKey(surfPerso, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_SetColorKey(surfSelec, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_BlitSurface(surfPerso, NULL, surfNiveau, &pos);
    SDL_BlitSurface(surfLigne, NULL, surfNiveau, NULL);
    afficherTexte(surfPause, "ariblk.ttf", 60, "II", 0, 0);
    SDL_BlitSurface(surfPause, NULL, surfNiveau, &pospause);
    SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
    SDL_BlitSurface(rect, NULL, surfNiveau, &posrec);
    SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
    SDL_BlitSurface(surfSelec, NULL, ecran, NULL);
    SDL_Flip(ecran);
    while(continuer!=0){
        SDL_Delay(30);
        SDL_FillRect(surfSelec, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
        if( (pospersoNiveau.x - surfNiveau->w) < (surfNiveau->w)/3 )
            continuer = avancer(&pospersoNiveau, surfNiveau, selecNiveau);
		if(continuer == 0)
			return (-1); //gameOver
        if (continuer == -1)
            return 0; //Victoire
        if(continuer!=2){
            if (chute >= 2*H_PERSO){
				return -1;
            }
				else {chute = 0;}
			selecNiveau.x = selecNiveau.x + 4;}
            else {chute +=5;}
        if(collagePossible){
            decouperColler(surfaceFond, posSelection, posDestination);
            collagePossible = 0;
            enSelection = 0;
            dejaSelectionne = 0;
        }
        if(enSelection){
            if(dejaSelectionne){
                pointilleSelection(surfSelec, selecNiveau, posSelection, -1, -1);
                pointilleSelection(surfSelec, selecNiveau, posDestination, xSouris, ySouris);
            }
            else{
                pointilleSelection(surfSelec, selecNiveau, posSelection, xSouris, ySouris);
            }
        }
        else if(dejaSelectionne){
            pointilleSelection(surfSelec, selecNiveau, posSelection, -1, -1);
        }
        SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
        SDL_SetColorKey(surfSelec, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
        SDL_BlitSurface(surfLigne, NULL, surfNiveau, NULL);
        SDL_BlitSurface(surfPerso, NULL, surfNiveau, &pospersoNiveau);
        SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
        SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
        SDL_BlitSurface(rect, NULL, ecran, &posrec);
        SDL_BlitSurface(surfSelec, NULL, ecran, NULL);
        SDL_Flip(ecran);
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_MOUSEMOTION:
                xSouris = event.button.x;
                ySouris = event.button.y;
                if(((ecran->w - 77) <= xSouris) && (ySouris < 76)){
                    SDL_SetAlpha(rect, SDL_SRCALPHA, 120);
                    select = 0;
                    posrec.x=ecran->w - 77;
                    posrec.y=14;
                }
                else { SDL_SetAlpha(rect, SDL_SRCALPHA, 0);}
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSourisButton = event.button.x;
                    ySourisButton = event.button.y;
                    xSourisButton = xSourisButton + selecNiveau.x;
                    ySourisButton = ySourisButton + selecNiveau.y;
                    if(collagePossible == 0){
                        if(enSelection == 0){
                            enSelection = 1;
                            if(dejaSelectionne){
                                posDestination.x = xSourisButton;
                                posDestination.y = ySourisButton;
                            }
                            else{
                                posSelection.x = xSourisButton;
                                posSelection.y = ySourisButton;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSourisButton = event.button.x;
                    ySourisButton = event.button.y;
                    if(((ecran->w - 77) <= xSourisButton) && (ySourisButton < 76)){
                        switch (pause(ecran)){
                            case SORTIE:
                                return SORTIE;
                                break;
                            case 1:
                                return 1;
                                break;
                            case 0:
                                event.button.button=SDL_BUTTON_RIGHT;
                                SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
                                collagePossible = 0;
                                enSelection = 0;
                                dejaSelectionne = 0;
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
                                if(dejaSelectionne){
                                    posDestination.w = abs(posDestination.x-xSourisButton);
                                    posDestination.h = abs(posDestination.y-ySourisButton);
                                    posDestination.x = min(posDestination.x, xSourisButton);
                                    posDestination.y = min(posDestination.y, ySourisButton);
                                    event.type = SDL_MOUSEMOTION;
                                    collagePossible = 1;
                                }
                                else {
                                    posSelection.w = abs(posSelection.x-xSourisButton);
                                    posSelection.h = abs(posSelection.y-ySourisButton);
                                    posSelection.x = min(posSelection.x, xSourisButton);
                                    posSelection.y = min(posSelection.y, ySourisButton);
                                    enSelection = 0;
                                    dejaSelectionne = 1;
                                    event.type = SDL_MOUSEMOTION;
                                }
                            }
                        }
                    }
                }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        switch (pause(ecran)){
                            case SORTIE:
                                return SORTIE;
                                break;
                            case 1:
                                return 1;
                                break;
                            case 0:
                            /*cairo_surface_destroy(surfaceFond);
                            SDL_FreeSurface(surfNiveau);*/
                                event.key.keysym.sym = SDLK_a;
                                break;
                            default:
                                break;
                            }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                return SORTIE;
                break;
            default:
                break;
        }
    }
    return 1;
}

cairo_t * tterrain(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_move_to(droite, 0., 500.);
    cairo_line_to(droite, 400., 500.);
    cairo_curve_to(droite, 700, 600, 800, 50, 1100., 200.);
    cairo_line_to(droite, 1300, 200.);
    cairo_move_to(droite, 1250., 400.);
    cairo_line_to(droite, 1500, 400.);
    cairo_line_to(droite, 1900, 600);
    cairo_curve_to(droite, 2200, 600, 2500, 300, 2800, 200);
    cairo_line_to(droite, 3000, 200);
    cairo_move_to(droite, 2900, 500);
    cairo_line_to(droite, X_FIN + 200, 500);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke_preserve(droite);
    return (droite);
}


cairo_t * pperso(SDL_Surface *surfNiveau, cairo_surface_t *surface, SDL_Surface *surfPerso)
{
    surface = cairo_image_surface_create_for_data (surfPerso->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfPerso->w,
                                                      surfPerso->h,
                                                      surfPerso->pitch);

    cairo_t *perso = cairo_create(surface);
    cairo_set_line_width(perso, EPAISSEUR_TRAIT);
    cairo_set_source_rgba (perso, 0, 0, 0, 1);
    cairo_arc(perso, 31, 19, 13, 0, 2*M_PI);
    cairo_fill_preserve(perso);
    cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
    //Corps
    cairo_move_to(perso, 28, 27);
    cairo_line_to(perso, 26, 72);
    cairo_stroke_preserve(perso);
    //Bras 1
    cairo_move_to(perso, 28, 29);
    cairo_line_to(perso, 15, 43);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 14, 44);
    cairo_line_to(perso, 11, 65);
    cairo_stroke_preserve(perso);
    //Bras 2
    cairo_move_to(perso, 32, 32);
    cairo_line_to(perso, 37, 48);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 38, 49);
    cairo_line_to(perso, 52, 63);
    cairo_stroke_preserve(perso);
    //Jambe 1
    cairo_move_to(perso, 25, 74);
    cairo_line_to(perso, 17, 94);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 17, 95);
    cairo_line_to(perso, 5, 133);
    cairo_stroke_preserve(perso);
    //Jambe 2
    cairo_move_to(perso, 27, 76);
    cairo_line_to(perso, 40, 96);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 41, 97);
    cairo_line_to(perso, 52, 128);
    cairo_stroke_preserve(perso);
    return perso;
}

int min(int a, int b){
    if(a <= b)
        return a;
    return b;
}

int max(int a, int b){
    if(a <= b)
        return b;
    return a;
}

void decouperColler(cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination){

	cairo_t *enSelect = cairo_create(surfaceFond);
	double scale_x = 1, scale_y = 1; //scale_x = (posDestination.w/posSelection.w), scale_y = (posDestination.h/posSelection.h);
    cairo_scale(enSelect, scale_x, scale_y);
    cairo_set_source_surface (enSelect, surfaceFond,  (posDestination.x/scale_x) - posSelection.x, (posDestination.y/scale_y) - posSelection.y); //remplit cr avec le chemin présent
    //dans surfaceFond en commancant en 0,0 dans cr => 0, 280 dans surfaceFond
    //cairo_set_source_surface (enSelect, source, x_dest*scale_x-x_source, y_dest*scale_y-y_source);
    //cairo_paint(enSelect); //recouvre tout surfaceFond avec cr
    cairo_rectangle (enSelect, posDestination.x/scale_x, posDestination.y/scale_y, posDestination.w/scale_x, posDestination.h/scale_y); //cairo_rectangle (enSelect, x_dest*scale_x, y_dest*scale_y, width, heigh);
    cairo_fill (enSelect); // ne recouvre surfaceFond que
    //dans le carrée dont le coin supérieur gauche est en 0,0 et de largeur 100)

}

void pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris){

    cairo_surface_t *surfaceFond;
    int x_rel = 0, coinHG_x = 0, coinHG_y = 0, coinHD_x = 0, coinBD_y = 0, coinBG_x = 0;
    x_rel = pos.x-selecNiveau.x;
    if((xSouris == -1) && (ySouris == -1)){
        coinHG_x = coinBG_x = x_rel;
        coinHG_y = pos.y;
        coinHD_x = coinHG_x + pos.w;
        coinBD_y = pos.y + pos.h;

    }
    else{
        coinHG_x = coinBG_x = min(x_rel,xSouris);
        coinHG_y = min(pos.y, ySouris);
        coinHD_x = max(x_rel, xSouris);
        coinBD_y = max(pos.y, ySouris);
    }
    if(x_rel > 0) {
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
    }
}

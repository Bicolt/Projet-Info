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


int niveau(SDL_Surface *ecran, int choixTerrain){


    int continuer = 1, select = 0;
    int enSelection = 0, dejaSelectionne = 0, collagePossible = 0, enDrag = 0, chute = 0;
    Uint32 temps_precedent = SDL_GetTicks(), temps_actuel = SDL_GetTicks();
    int xSouris = 0, ySouris = 0;
    int xSourisButton = 0, ySourisButton = 0;
    int xRinit = 0, yRinit = 0;


    SDL_Surface *surfNiveau, *surfPerso = NULL, *surfPause = NULL, *surfLigne = NULL, *rect=NULL, *surfSelec=NULL;
    surfNiveau = SDL_CreateRGBSurface(SDL_HWSURFACE, NOMBRE_ECRANS*ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfSelec = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, L_PERSO, H_PERSO, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, surfNiveau->w, surfNiveau->h, 32, 0, 0, 0, 0);
    surfPause = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0);

    SDL_Rect pos, posligne, pospersoNiveau, pospause, posrec, selecNiveau, posSelection, posDestination, posAppercu, posPersoEcran;
    pos.x = selecNiveau.x = posligne.x = posSelection.x = posDestination.x = posAppercu.x = 0;
    pos.y = selecNiveau.y = posligne.y = posSelection.y = posDestination.y = posAppercu.y = 0;
    selecNiveau.h = ecran -> h;
    selecNiveau.w = ecran -> w;
    pospause.x = ecran->w - 70;
    pospause.y = 0;
    pospersoNiveau.x = ecran->w/3;
    pospersoNiveau.y = 300;
    posPersoEcran.x = pospersoNiveau.x - selecNiveau.x;
    posPersoEcran.y = pospersoNiveau.y;
    cairo_surface_t *surfaceFond;
    rect = selection(60, 60, surfNiveau->format);
    SDL_Event event;
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfSelec, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfPause, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfPerso, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));

    // Création d'une surface cairo ayant pour format d'affichage celui d'une surface SDL
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch);



    chargerTerrain(ecran, surfaceFond, choixTerrain);
	cairo_t *personnage = pperso(surfNiveau, surfPerso);

    SDL_SetColorKey(surfPerso, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_SetColorKey(surfSelec, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_SetColorKey(surfLigne, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_BlitSurface(surfLigne, NULL, surfNiveau, NULL);
    afficherTexte(surfPause, "ariblk.ttf", 60, "II", 0, 0);
    SDL_BlitSurface(surfPause, NULL, surfNiveau, &pospause);
    SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
    SDL_BlitSurface(rect, NULL, surfNiveau, &posrec);
    SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
    SDL_BlitSurface(surfSelec, NULL, ecran, NULL);
    SDL_Flip(ecran);
    while(continuer!=0){
        temps_actuel = SDL_GetTicks();
        SDL_Delay(max(60 - (temps_actuel-temps_precedent),0));
        temps_precedent = temps_actuel;
        SDL_FillRect(surfSelec, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
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
			selecNiveau.x = selecNiveau.x + 4;
			}
            else {chute +=5;}
        if(collagePossible){
            decouperColler(surfLigne, surfaceFond, posSelection, posDestination);
            collagePossible = 0;
            enSelection = 0;
            dejaSelectionne = 0;
            enDrag = 0;
        }
        if(enSelection){
            pointilleSelection(surfSelec, selecNiveau, posSelection, xSouris, ySouris);
        }
        else if(dejaSelectionne){
            pointilleSelection(surfSelec, selecNiveau, posSelection, -1, -1);
        }
        SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, surfNiveau, NULL);
        SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
        SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
        if(enDrag){
            posAppercu.x =  min(xSouris - xRinit, ecran->w - posSelection.w);
            posAppercu.y =  min(ySouris - yRinit, ecran->h - posSelection.h);
            SDL_BlitSurface(surfLigne, &posSelection, ecran, &posAppercu);
        }
        posPersoEcran.x = pospersoNiveau.x - selecNiveau.x;
        posPersoEcran.y = pospersoNiveau.y;
        SDL_BlitSurface(surfPerso, NULL, ecran, &posPersoEcran);
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
                xSourisButton = event.button.x;
                ySourisButton = event.button.y;
                if(event.button.button == SDL_BUTTON_LEFT){
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
                else if(event.button.button == SDL_BUTTON_RIGHT){
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
                break;
            case SDL_MOUSEBUTTONUP:
                xSourisButton = event.button.x;
                ySourisButton = event.button.y;
                if(event.button.button == SDL_BUTTON_LEFT){
                    if(((ecran->w - 77) <= xSourisButton) && (ySourisButton < 76)){
                        switch (pause(ecran)){
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
                                enDrag = 0;
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
                if(event.button.button == SDL_BUTTON_RIGHT){
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
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        switch (pause(ecran)){
                            case SORTIE:
                                return SORTIE;
                                break;
                            case MENU:
                                return MENU;
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
    return MENU;
}

cairo_t * pperso(SDL_Surface *surfNiveau, SDL_Surface *surfPerso)
{
    cairo_surface_t *surface = NULL;
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

void decouperColler(SDL_Surface *surfLigne, cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination){

    SDL_Surface *collage = NULL;
    SDL_Rect posRelative;
    posRelative.x = 0;
    posRelative.y = 0;
    posRelative.w = posSelection.w;
    posRelative.h = posSelection.h;

    collage = SDL_CreateRGBSurface(SDL_HWSURFACE , posSelection.w, posSelection.h, 32, 0, 0, 0, 0);
    SDL_SetColorKey(collage, SDL_SRCCOLORKEY, SDL_MapRGB(collage->format,255,255,255));
    cairo_surface_t *surfCollage = cairo_image_surface_create_for_data (collage->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      collage->w,
                                                      collage->h,
                                                      collage->pitch);

    recollementContinu(surfLigne, posSelection, &posDestination);

	cairo_t *enSelect = cairo_create(surfCollage);
	double scale_x = 1, scale_y = 1; //scale_x = (posDestination.w/posSelection.w), scale_y = (posDestination.h/posSelection.h);
    cairo_scale(enSelect, scale_x, scale_y);
    cairo_set_source_surface (enSelect, surfaceFond,  (posRelative.x/scale_x) - posSelection.x, (posRelative.y/scale_y) - posSelection.y); //remplit cr avec le chemin présent
    //dans surfaceFond en commancant en 0,0 dans cr => 0, 280 dans surfaceFond
    //cairo_set_source_surface (enSelect, source, x_dest*scale_x-x_source, y_dest*scale_y-y_source);
    //cairo_paint(enSelect); //recouvre tout surfaceFond avec cr
    cairo_rectangle (enSelect, posRelative.x/scale_x, posRelative.y/scale_y, min(posDestination.w/scale_x,posSelection.w), min(posDestination.h/scale_y,posSelection.h)); //cairo_rectangle (enSelect, x_dest*scale_x, y_dest*scale_y, width, heigh);
    cairo_fill (enSelect); // ne recouvre surfaceFond que
    //dans le carrée dont le coin supérieur gauche est en 0,0 et de largeur 100
    SDL_BlitSurface(collage, NULL, surfLigne, &posDestination);

}

void recollementContinu(SDL_Surface *surfLigne, SDL_Rect posSelection, SDL_Rect *posDestination){
    int decallage = 0, trouve = 1, compteur = 0, rel_compteur, continuer = 1;
    while(continuer && (compteur < (min(posSelection.h, posDestination->h) - 16))){
        long unsigned int pix = getpixel(surfLigne,posSelection.x, posSelection.y + compteur );
        if((pix == 0LL)||(pix == 4278190080LL)){
            rel_compteur = max((compteur - 15),0);
            while((rel_compteur < compteur + 15)&&trouve){
                long unsigned int pix_int = getpixel(surfLigne, posDestination->x-1, posDestination->y + rel_compteur);
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

void pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris){

    cairo_surface_t *surfaceFond;
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

void chargerTerrain(SDL_Surface *ecran, cairo_surface_t *surfaceFond, int choixTerrain){
    switch(choixTerrain){
        case 0:
            tterrain0(ecran, surfaceFond);
            break;
        case 1:
            tterrain1(ecran, surfaceFond);
            break;
        case 2:
            tterrain2(ecran, surfaceFond);
            break;
        case 3:
            tterrain3(ecran, surfaceFond);
            break;
        case 4:
            tterrain4(ecran, surfaceFond);
            break;
        case 5:
            tterrain5(ecran, surfaceFond);
            break;
        case 6:
            tterrain6(ecran, surfaceFond);
            break;
        default:
            break;
    }
}

void tterrain0(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_select_font_face (droite, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (droite, 90.0);

    cairo_move_to (droite, 10.5, 135.5);
    cairo_show_text (droite, "Hello");

    cairo_move_to(droite, 0, 500.5);
    cairo_line_to(droite, X_FIN+200.5, 500.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain1(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain2(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 1450.5, 500.5);
    cairo_line_to(droite, 1600.5, 600.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain3(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain4(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain5(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain6(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

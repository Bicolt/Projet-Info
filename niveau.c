#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include "niveau.h"
#include "main.h"
#include "affichage.h"

int niveau(SDL_Surface *ecran);
cairo_t * pperso(SDL_Surface *surfNiveau, cairo_surface_t *surface);
cairo_t * tterrain(SDL_Surface *ecran, cairo_surface_t * surfaceFond);

int niveau(SDL_Surface *ecran){

    int continuer = 1, select = 0, vitesse = 0;
    int xSouris, ySouris;

    SDL_Surface *surfNiveau, *surfPerso = NULL, *surfPause = NULL, *surfLigne = NULL, *rect=NULL;
    surfNiveau = SDL_CreateRGBSurface(SDL_HWSURFACE, NOMBRE_ECRANS*ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 140, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, surfNiveau->w, surfNiveau->h, 32, 0, 0, 0, 0);
    surfPause = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0);

    SDL_Rect pos, posligne, pospersoNiveau, pospause, posrec, selecNiveau;
    pos.x = selecNiveau.x = posligne.x = 0;
    pos.y = selecNiveau.y = posligne.y = 0;
    selecNiveau.h = ecran -> h;
    selecNiveau.w = ecran -> w;
    pospause.x = ecran->w - 70;
    pospause.y = 0;
    pospersoNiveau.x = 200;
    pospersoNiveau.y = 60;
    cairo_surface_t *surface, *surfaceFond;
    rect = selection(60, 60, surfNiveau->format);
    SDL_Event event;
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
    SDL_FillRect(surfPause, NULL, SDL_MapRGB(surfPause->format, 255, 255, 255));
    SDL_FillRect(surfPerso, NULL, SDL_MapRGB(surfPerso->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(surfPerso->format, 255, 255, 255));
    SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(surfPerso->format, 255, 255, 255));

    // Création d'une surface cairo pour le personnage ayant pour format d'affichage celui d'une surface SDL
    surface = cairo_image_surface_create_for_data (surfPerso->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfPerso->w,
                                                      surfPerso->h,
                                                      surfPerso->pitch);
        // Création d'une surface cairo ayant pour format d'affichage celui d'une surface SDL
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch);
    cairo_t *droite1 = tterrain(ecran, surfaceFond); 
	cairo_t *personnage = pperso(surfNiveau, surface);
    /* cairo_t *droite2 = cairo_create(surfaceFond);
    cairo_move_to(droite1, 0., 300.); //debut de ligne
    cairo_line_to(droite1, 1500., 300.);
    /*cairo_move_to(droite, 250., 600.);
    cairo_line_to(droite, surfNiveau->w-200, 600.);
    cairo_curve_to(droite, surfNiveau->w, 600., surfNiveau->w, 400., surfNiveau->w, 400.);
    cairo_line_to(droite, surfNiveau->w-200., 550.); 
    //fin de ligne
    cairo_set_source_rgba (droite1, 0, 0, 0, 1);
    cairo_set_line_width(droite1,EPAISSEUR_TRAIT);
    cairo_stroke(droite1);
    cairo_move_to(droite2, 1500., 170.); //debut de ligne
    cairo_line_to(droite2, 1250., 170.);
    cairo_set_line_width(droite2,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite2, 0, 0, 0, 1);
    //cairo_line_to(droite, surfNiveau->w-100, 600.);
    cairo_stroke(droite2); */
    SDL_UnlockSurface(surfNiveau);
    SDL_SetColorKey(surfPerso, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_BlitSurface(surfPerso, NULL, surfNiveau, &pos);
    SDL_BlitSurface(surfLigne, NULL, surfNiveau, &pos);
    afficherTexte(surfPause, "ariblk.ttf", 60, "II", 0, 0);
    SDL_BlitSurface(surfPause, NULL, surfNiveau, &pospause);
    SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
    SDL_BlitSurface(rect, NULL, surfNiveau, &posrec);
    SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
    SDL_Flip(ecran);
    while(continuer){
        SDL_Delay(30);
        if( (pospersoNiveau.x - surfNiveau->w) < (surfNiveau->w)/3 )
            vitesse = avancer(&pospersoNiveau, surfNiveau);
        SDL_FillRect(surfNiveau, NULL, SDL_MapRGB(surfNiveau->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, surfNiveau, &posligne);
        SDL_BlitSurface(surfPerso, NULL, surfNiveau, &pospersoNiveau);
        SDL_BlitSurface(surfNiveau, &selecNiveau, ecran, NULL);
        SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
        SDL_BlitSurface(rect, NULL, ecran, &posrec);
        SDL_Flip(ecran);
        if(vitesse!=2){
            selecNiveau.x = selecNiveau.x + 4;}
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
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if(((ecran->w - 77) <= xSouris) && (ySouris < 76)){
                        if(pause(ecran))
                            continuer = 0;
                            else {event.button.button=SDL_BUTTON_RIGHT;
                            SDL_SetAlpha(rect, SDL_SRCALPHA, 0);}
                            }
                            }
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        if(pause(ecran)){
                            /*cairo_surface_destroy(surfaceFond);
                            SDL_FreeSurface(surfNiveau);*/
                            continuer = 0;
                        }
                            else event.key.keysym.sym = SDLK_a;
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
    }
    return 0;
}

cairo_t * tterrain(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_move_to(droite, 0., 300.); //debut de ligne
    cairo_curve_to(droite, 0., 300., 100., 400., 300., 400.);
    cairo_move_to(droite, 250., 600.);
    cairo_line_to(droite, ecran->w-200, 600.);
    cairo_curve_to(droite, ecran->w, 600., ecran->w, 400., ecran->w, 400.);
    cairo_line_to(droite, ecran->w-200., 550.);
    //fin de ligne
    cairo_stroke_preserve(droite);
    //cairo_line_to(droite, ecran->w-100, 600.);
    cairo_stroke(droite);
    return (surfaceFond);
}


cairo_t * pperso(SDL_Surface *surfNiveau, cairo_surface_t *surface)
{

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

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
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface);


int niveau(SDL_Surface *ecran){

    int continuer = 1, select = 0;
    int xSouris, ySouris;
    SDL_Rect pos, posligne, posperso, pospause, posrec;
    pos.x = posligne.x = 0;
    pos.y = posligne.y = 0;
    pospause.x = ecran->w - 70;
    pospause.y = 0;
    posperso.x = 0;
    posperso.y = 50;
    cairo_surface_t *surface, *surfaceFond;
    SDL_Surface *surfPerso = NULL, *surfPause = NULL, *surfLigne = NULL, *rect=NULL;
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 140, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    surfPause = SDL_CreateRGBSurface(SDL_HWSURFACE, 70, 70, 32, 0, 0, 0, 0);
    rect = selection(60, 60, ecran->format);
    SDL_Event event;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfPause, NULL, SDL_MapRGB(surfPause->format, 255, 255, 255));
    SDL_FillRect(surfPerso, NULL, SDL_MapRGB(surfPerso->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(surfPerso->format, 255, 255, 255));

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
    cairo_t *droite = cairo_create(surfaceFond);
    cairo_t *personnage = pperso(ecran, surface);
    cairo_move_to(droite, 0., 300.); //debut de ligne
    cairo_curve_to(droite, 0., 300., 100., 400., 300., 400.);
    /*cairo_move_to(droite, 250., 600.);
    cairo_line_to(droite, ecran->w-200, 600.);
    cairo_curve_to(droite, ecran->w, 600., ecran->w, 400., ecran->w, 400.);
    cairo_line_to(droite, ecran->w-200., 550.); */
    //fin de ligne
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_stroke_preserve(droite);
    //cairo_line_to(droite, ecran->w-100, 600.);
    cairo_stroke(droite);
    SDL_UnlockSurface(ecran);
    SDL_SetColorKey(surfPerso, SDL_SRCCOLORKEY, SDL_MapRGB(surfPerso->format,255,255,255));
    SDL_BlitSurface(surfPerso, NULL, ecran, &pos);
    SDL_BlitSurface(surfLigne, NULL, ecran, &pos);
    afficherTexte(surfPause, "ariblk.ttf", 60, "II", 0, 0);
    SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
    SDL_SetAlpha(rect, SDL_SRCALPHA, 0);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    while(continuer){
        SDL_Delay(30);
        continuer = avancer(&posperso, surfLigne, ecran);//posperso.x = (posperso.x + 2)%(ecran -> w);
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, ecran, &posligne);
        SDL_BlitSurface(surfPerso, NULL, ecran, &posperso);
        SDL_BlitSurface(surfPause, NULL, ecran, &pospause);
        SDL_BlitSurface(rect, NULL, ecran, &posrec);
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
                        if(pause(ecran))
                            continuer = 0;
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


cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface)
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


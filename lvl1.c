#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "lvl1.h"

int niveau(SDL_Surface *ecran);
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface);

int niveau(SDL_Surface *ecran){

    int continuer = 1;
    SDL_Rect pos, posligne, posperso;
    pos.x = posligne.x = 0;
    pos.y = posligne.y = 0;
    posperso.x = 0;
    posperso.y = 358;
    cairo_surface_t *surface, *surfaceFond;
    SDL_Surface *surfRect = NULL, *surfLigne = NULL;
    surfRect = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 140, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    SDL_Event event;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfRect, NULL, SDL_MapRGB(surfRect->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(surfRect->format, 255, 255, 255));

    // Cr�ation d'une surface cairo pour le rectangle ayant pour format d'affichage celui d'une surface SDL
    surface = cairo_image_surface_create_for_data (surfRect->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfRect->w,
                                                      surfRect->h,
                                                      surfRect->pitch);
        // Cr�ation d'une surface cairo ayant pour format d'affichage celui d'une surface SDL
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch);
    cairo_t *droite = cairo_create(surfaceFond);
    cairo_t *rectangle = pperso(ecran, surface); // cairo_create(surface);
    cairo_move_to(droite, 0., 500.); //debut de ligne
    cairo_line_to(droite, ecran->w, 500.); //fin de ligne
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_stroke_preserve(droite);
    cairo_line_to(droite, ecran->w-100, 600.);
    cairo_stroke(droite);
    /* cairo_rectangle(rectangle, 0., 440, 100, 50);
    cairo_set_line_width(rectangle,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (rectangle, 0, 0, 0, 1); // rgb, transparence tous compris entre 0 et 1
    cairo_fill_preserve(rectangle);
    cairo_stroke(rectangle);*/
    SDL_UnlockSurface(ecran);
    SDL_SetColorKey(surfRect, SDL_SRCCOLORKEY, SDL_MapRGB(surfRect->format,255,255,255));
    SDL_BlitSurface(surfRect, NULL, ecran, &pos);
    SDL_BlitSurface(surfLigne, NULL, ecran, &pos);
    SDL_Flip(ecran);
    while(continuer){
        SDL_Delay(30);
        posperso.x = (posperso.x + 2)%(ecran -> w);
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, ecran, &posligne);
        SDL_BlitSurface(surfRect, NULL, ecran, &posperso);
        SDL_Flip(ecran);
        SDL_PollEvent(&event);
        switch(event.type){
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        continuer = 0;
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


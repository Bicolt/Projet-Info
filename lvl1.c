#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "lvl1.h"

int niveau(SDL_Surface *ecran){

    int continuer = 1;
    SDL_Rect pos, posligne;
    pos.x = posligne.x = 0;
    pos.y = posligne.y = 0;
    cairo_surface_t *surface, *surfaceFond;
    SDL_Surface *surfRect = NULL, *surfLigne = NULL;
    surfRect = SDL_CreateRGBSurface(SDL_HWSURFACE, 150, 495, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    SDL_Event event;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_FillRect(surfRect, NULL, SDL_MapRGB(surfRect->format, 255, 255, 255));
    SDL_FillRect(surfLigne, NULL, SDL_MapRGB(surfRect->format, 255, 255, 255));

    // Création d'une surface cairo pour le rectangle ayant pour format d'affichage celui d'une surface SDL
    surface = cairo_image_surface_create_for_data (surfRect->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfRect->w,
                                                      surfRect->h,
                                                      surfRect->pitch);
        // Création d'une surface cairo ayant pour format d'affichage celui d'une surface SDL
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      surfLigne->w,
                                                      surfLigne->h,
                                                      surfLigne->pitch);
    cairo_t *droite = cairo_create(surfaceFond);
    cairo_t *rectangle = cairo_create(surface);
    cairo_move_to(droite, 0., 500.); //debut de ligne
    cairo_line_to(droite, ecran->w, 500.); //fin de ligne
    cairo_set_line_width(droite,10.);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_stroke_preserve(droite);
    cairo_line_to(droite, ecran->w-100, 600.);
    cairo_stroke(droite);
    cairo_rectangle(rectangle, 0., 440, 100, 50);
    cairo_set_line_width(rectangle,10.);
    cairo_set_source_rgba (rectangle, 0, 0, 0, 1); // rgb, transparence tous compris entre 0 et 1
    cairo_fill_preserve(rectangle);
    cairo_stroke(rectangle);
    SDL_UnlockSurface(ecran);
    SDL_BlitSurface(surfLigne, NULL, ecran, &pos);
    SDL_BlitSurface(surfRect, NULL, ecran, &pos);
    SDL_Flip(ecran);
    while(continuer){
        SDL_Delay(30);
        pos.x = (pos.x + 2)%(ecran -> w);
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, ecran, &posligne);
        SDL_BlitSurface(surfRect, NULL, ecran, &pos);
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

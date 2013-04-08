#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include "lvl1.h"
#include "main.h"

int niveau(SDL_Surface *ecran);
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface);


//A la definition de la fonction avancer, j'ai cree :
unsigned int getpixel(SDL_Surface *s, int x, int y);
int avancer ( SDL_Rect* pposperso, SDL_Surface* terrain);
int solsouspieds ( SDL_Rect* pposperso, SDL_Surface* terrain);
void tomber ( SDL_Rect* pposperso, SDL_Surface* terrain);
int monter( SDL_Rect* pposperso, SDL_Surface* terrain );
int plater( SDL_Rect* pposperso, SDL_Surface* terrain );
int descendre( SDL_Rect* pposperso, SDL_Surface* terrain );


int niveau(SDL_Surface *ecran){

    int continuer = 1;
    SDL_Rect pos, posligne, posperso;
    pos.x = posligne.x = 0;
    pos.y = posligne.y = 0;
    posperso.x = 0;
    posperso.y = 50;
    cairo_surface_t *surface, *surfaceFond;
    SDL_Surface *surfPerso = NULL, *surfLigne = NULL;
    surfPerso = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 140, 32, 0, 0, 0, 0);
    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
    SDL_Event event;
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
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
    cairo_move_to(droite, 250., 600.);
    cairo_line_to(droite, ecran->w-200, 600.);
    cairo_curve_to(droite, ecran->w, 600., ecran->w, 400., ecran->w, 400.);
    cairo_line_to(droite, ecran->w-200., 550.);
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
    SDL_Flip(ecran);
    while(continuer){
        SDL_Delay(30);
        continuer = avancer(&posperso, surfLigne);//posperso.x = (posperso.x + 2)%(ecran -> w);
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
        SDL_BlitSurface(surfLigne, NULL, ecran, &posligne);
        SDL_BlitSurface(surfPerso, NULL, ecran, &posperso);
        SDL_Flip(ecran);
        SDL_PollEvent(&event);
        switch(event.type){
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

unsigned int getpixel(SDL_Surface *s, int x, int y) {
    return ((unsigned int*)s->pixels)[y*(s->pitch/sizeof(unsigned int)) + x];
}

int avancer ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    if (solsouspieds(pposperso, terrain) == 0) {
        tomber(pposperso, terrain);
        return 1;
    }
    if (monter(pposperso, terrain) == 1) {
        tomber(pposperso, terrain);
        tomber(pposperso, terrain);
    }
    else {
        if ( plater(pposperso, terrain) == 1 ) {
            tomber(pposperso, terrain);
        }
        else {
            descendre(pposperso, terrain);
        }
    }
    return 1;

}

int monter( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    for(i=0 ; i<= 10 ; i++) {
        if ( getpixel(terrain, pposperso->x + 5*i + 4, pposperso->y - 5) == 4278190080 )
            return 0;
    }
    for(i=0 ; i<= 20 ; i++) {
        if ( getpixel(terrain, pposperso->x + 54, pposperso->y + 5*i - 5) == 4278190080 )
            return 0;
    }
    //Si on a survécu aux deux boucles, on peut monter (meme si on est sur du plat). Du coup on s'en prive pas.
    pposperso->x += 4;
    pposperso->y -= 5;
    return 1;
}

int descendre( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    for(i=0 ; i<= 10 ; i++) {
        if ( getpixel(terrain, pposperso->x + 5*i + 4, pposperso->y + 105) == 4278190080 )
            return 0;
    }
    for(i=0 ; i<= 20 ; i++) {
        if ( getpixel(terrain, pposperso->x + 54, pposperso->y + 5 + 5*i) == 4278190080 )
            return 0;
    }
    //Si on a survécu aux deux boucles, on peut avancer. Du coup on s'en prive pas.
    pposperso->x += 4;
    pposperso->y += 5;
    return 1;
}

int plater( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    for(i=0 ; i< 20 ; i++) {
        if ( getpixel(terrain, pposperso->x + 54, pposperso->y + 5*i) == 4278190080 )
            return 0;
    }
    if ( getpixel(terrain, pposperso->x + 54, pposperso->y + 99) == 4278190080 ) //On teste le pixel tout en bas à par, pour le réhausser d'un pixel.
        return 0;
    pposperso->x += 4;
    return 1;
}

int solsouspieds ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    for(i=0 ; i<= 10 ; i++) {
        if ( getpixel(terrain, pposperso->x - 5*i + 50, pposperso->y + 100) == 4278190080 )
            return 1;
    }
    return 0;

}

void tomber ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    for(i = 0 ; i < 5 ; i++) {
        if (solsouspieds(pposperso, terrain) == 1)
            break;
        (pposperso->y) ++ ;
    }
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


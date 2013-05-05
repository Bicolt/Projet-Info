#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include "niveau.h"
#include "interface.h"
#include "main.h"
#include "affichage.h"
#include "deplacement.h"


unsigned int getpixel(SDL_Surface *s, int x, int y) {
   // if(y*(s->pitch/sizeof(unsigned int))+x>=(s->w)*(s->h))
    return ((unsigned int*)s->pixels)[y*(s->pitch/sizeof(unsigned int)) + x];
}

int avancer ( SDL_Rect* pposperso, SDL_Surface* terrain, SDL_Rect selecNiveau) {
    if (sortir(pposperso, terrain, selecNiveau))
            return 0; // game over
    if(finir(pposperso) == -1)
        return -1; // Victoire
    if (solsouspieds(pposperso, terrain) == -1)
        return 0; // game over
    if (solsouspieds(pposperso, terrain) == 0) {
        tomber(pposperso, terrain);
        return 2; // tomber
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
        if (monter(pposperso, terrain) == -1)
            return 0; // game over
    }
    return 1; // avancer
}

int monter( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i<=L_PERSO/5 ; i++) {
        pixelou = getpixel(terrain, pposperso->x + 5*i, pposperso->y);
        if ( pixelou == 0LL || pixelou == 4278190080LL ) //noir
            return 0;
    }
    for(i=0 ; i<=H_PERSO/5 -1 ; i++) {
        pixelou = getpixel(terrain, pposperso->x + L_PERSO, pposperso->y + 5*i);
        if ( pixelou == 0LL || pixelou == 4278190080LL )
            return 0;
    }
    for(i=0 ; i<= H_PERSO/5 - 1; i++) {
        if ( getpixel(terrain, pposperso->x + L_PERSO, pposperso->y + 5*i) == 0xFFFF0000L )
            return -1;
    }
    //Si on a survécu aux deux boucles, on peut monter (meme si on est sur du plat). Du coup on s'en prive pas.
    pposperso->x += 4;
    pposperso->y -= 5;
    return 1;
}

int descendre( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i<= L_PERSO/5 ; i++) {
        pixelou = getpixel(terrain, pposperso->x + 5*i, pposperso->y + (H_PERSO + EPAISSEUR_TRAIT/2));
        if ( pixelou == 0LL || pixelou == 4278190080LL )
            return 0;
    }
    for(i=0 ; i<= H_PERSO/5 ; i++) {
        pixelou =  getpixel(terrain, pposperso->x + L_PERSO , pposperso->y + EPAISSEUR_TRAIT/2 + 5*i) ;
        if ( pixelou == 0LL || pixelou == 4278190080LL )
            return 0;
    }
    //Si on a survécu aux deux boucles, on peut avancer. Du coup on s'en prive pas.
    pposperso->x += 4;
    pposperso->y += 5;
    return 1;
}

int plater( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i< H_PERSO/5; i++) {
        pixelou = getpixel(terrain, pposperso->x + L_PERSO , pposperso->y + 5*i);
        if ( pixelou == 0LL || pixelou == 4278190080LL )
            return 0;
    }
    pixelou = getpixel(terrain, pposperso->x + L_PERSO , pposperso->y + H_PERSO - 1);
    if ( pixelou == 0LL || pixelou == 4278190080LL ) //On teste le pixel tout en bas à par, pour le réhausser d'un pixel.
        return 0;
    pposperso->x += 4;
    return 1;
}

int solsouspieds ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i<= L_PERSO/5 ; i++) {
        pixelou =  getpixel(terrain, pposperso->x - 5*i + L_PERSO, pposperso->y + H_PERSO);
        if (pixelou == 0LL || pixelou == 4278190080LL)
            return 1;
        else if (pixelou == 0xFFFF0000L ) // zone rouge
            return -1;
    }
    return 0;

}

void tomber ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    for(i = 0 ; i < 5 ; i++) {
        if (solsouspieds(pposperso, terrain) == 1)
            break;
        (pposperso->y) ++;
    }
}

int sortir (SDL_Rect *pposperso, SDL_Surface *terrain, SDL_Rect selecNiveau){
    if (pposperso->y > terrain->h - (H_PERSO + 5)){
        return 1;
    }
    if (pposperso->x < selecNiveau.x + 6){
        return 1;
    }
    if(pposperso->y < 6)
        return 1;
    return 0;
}

int finir (SDL_Rect *pposperso){
    if ((pposperso->x >= X_FIN) && (pposperso->x <= X_FIN + ew/5))
        return -1;
    return 0;
}

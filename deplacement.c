/**
* \file deplacement.c
* \version 1.0
* \date 2013
* \brief Gère les déplacements du personnage
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
#include "main.h"
#include "affichage.h"
#include "deplacement.h"

/**
 * \brief permet de récuperer la couleur ainsi que la transparence d'un pixel donné
 * \param s La surface dont le pixel est extrait
 * \param x,y La position du pixel 
 */
unsigned int getpixel(SDL_Surface *s, int x, int y) {
    return ((unsigned int*)s->pixels)[y*(s->pitch/sizeof(unsigned int)) + x];
}

/**
 * \brief Fait avancer le personnage 
 * 
 * La fonction fait appelle à différentes sous-fonctions qui testent si le personnage monte, descend, est bloqué, ...
 * \param pposperso La position du personnage sur le terrain 
 * \param terrain Le terrain sur lequel évolue le personnage
 * \param selecNiveau La position relative actuelle de l'écran de visualisation par rapport au niveau
 * \return un \e int qui correspond à l'état du déplacement (le personnage est bloqué, chutte, avance librement)
 */
int avancer ( SDL_Rect* pposperso, SDL_Surface* terrain, SDL_Rect selecNiveau) {
    //La fontion teste les différents cas de déplacement possibles
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

/**
 * \brief teste si le personnage peut monter
 * 
 * \param pposperso La position du personnage sur le terrain 
 * \param terrain Le terrain sur lequel évolue le personnage
 * \return un \e int qui est différent selon que le personnage est bloqué, peut monter, ou non 
 */
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
    //Si on a survécu aux deux boucles, on peut monter (meme si on est sur du plat).
    pposperso->x += 4;
    pposperso->y -= 5;
    return 1;
}

/**
 * \brief teste si le personnage peut descendre
 * 
 * \param pposperso La position du personnage sur le terrain 
 * \param terrain Le terrain sur lequel évolue le personnage
 * \return un \e int qui est différent selon que le personnage peut descendre, ou non 
 */
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
    //Si on a survécu aux deux boucles, on peut descendre (ainsi si le personnage est sur du plat, il monte et descend donc ne progresse qu'horizontalement). 
    pposperso->x += 4;
    pposperso->y += 5;
    return 1;
}

/**
 * \brief teste si le personnage peut avancer
 * 
 * \param pposperso La position du personnage sur le terrain
 * \param terrain Le terrain sur lequel évolue le personnage
 * \return un \e int qui est différent selon que le personnage est bloqué, ou non 
 */
int plater( SDL_Rect* pposperso, SDL_Surface* terrain ) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i< H_PERSO/5; i++) {
        pixelou = getpixel(terrain, pposperso->x + L_PERSO , pposperso->y + 5*i);
        if ( pixelou == 0LL || pixelou == 4278190080LL )
            return 0;
    }
    pixelou = getpixel(terrain, pposperso->x + L_PERSO , pposperso->y + H_PERSO - 1);
    if ( pixelou == 0LL || pixelou == 4278190080LL ) //On teste le pixel tout en bas à part.
        return 0;
    pposperso->x += 4;
    return 1;
}

/**
 * \brief teste si le personnage chute 
 *
 * et gère aussi la mort du personnage en cas de passage sur une zone rouge 
 * \param pposperso La position du personnage sur le terrain 
 * \param terrain Le terrain sur lequel évolue le personnage
 * \return un \e int qui est différent selon que le personnage est sur une zone rouge, chute, ou non
 */
int solsouspieds ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    unsigned int pixelou = 0;
    for(i=0 ; i<= L_PERSO/5 ; i++) {
        pixelou =  getpixel(terrain, pposperso->x - 5*i + L_PERSO, pposperso->y + H_PERSO);
        if (pixelou == 0LL || pixelou == 4278190080LL)
            return 1;
        else if (pixelou == 0xFFFF0000L ) // zone rouge synonime de mort
            return -1;
    }
    return 0;

}

/**
 * \brief modifie la position du personnage en fonction de s'il chute ou non 
 * \param pposperso La position du personnage sur le terrain
 * \param terrain Le terrain sur lequel évolue le personnage
 */
void tomber ( SDL_Rect* pposperso, SDL_Surface* terrain) {
    int i;
    for(i = 0 ; i < 5 ; i++) {
        if (solsouspieds(pposperso, terrain) == 1)
            break;
        (pposperso->y) ++;
    }
}

/**
 * \brief teste si le personnage sort de l'écran visible 
 *
 * \param pposperso La position du personnage sur le terrain 
 * \param terrain Le terrain sur lequel évolue le personnage
 * \param selecNiveau La position relative actuelle de l'écran de visualisation par rapport au niveau
 * \return un \e int qui est différent selon que le personnage sort de l'écran (auquel cas il meurt) ou non
 */
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

/**
 * \brief teste si le personnage est arrivé à la fin du niveau
 *
 * \param pposperso La position du personnage sur le terrain
 * \return un \e int qui est différent selon que le personnage a atteint la fin du niveau ou non 
 */
int finir (SDL_Rect *pposperso){
    if ((pposperso->x >= X_FIN) && (pposperso->x <= X_FIN + ew/5))
        return -1; // fin de niveau 
    return 0;
}

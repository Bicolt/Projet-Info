/**
 * \file      affichage.c
 * \version   1.0
 * \date      2013
 * \brief     Affichage simple de certains éléments graphiques
 *
 * \details   Fonctions d'affichage simple de textes ou de rectangle sur l'écran via la bibliothèque SDL.
 *            Création automatique des SDL_Surface nécesaires, du remplissage, du chargement des polices utilisées...
 */

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h> // pensez à modifier le chemin
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "affichage.h"
#include "main.h"


/**
 * \brief      Afficher un texte à l'écran via SDL
 * \details    Affiche un texte avec origine en haut à gauche.
 * \param    ecran  surface sur laquelle on affiche le texte
 * \param    font   nom de la police à utiliser
 * \param    size   taille de police
 * \param    txt    texte à afficher
 * \param    x      abscisse
 * \param    y      ordonnée
 * \return   void
 */
void afficherTexte (SDL_Surface *ecran, char *font, int size, char *txt, int x, int y) {
    TTF_Font *police = TTF_OpenFont(font, size);
    SDL_Color Black = {0, 0, 0};
    SDL_Color White = {255, 255, 255};
    SDL_Rect positiontexte;
    SDL_Surface *texte = TTF_RenderText_Shaded(police, txt, Black, White);
    positiontexte.x = x;
    positiontexte.y = y;
    SDL_BlitSurface(texte, NULL, ecran, &positiontexte);
    TTF_CloseFont(police);
}

/**
 * \brief      Afficher un texte à l'écran via SDL
 * \details    Affiche un texte avec origine au centre.
 * \param    ecran  surface sur laquelle on affiche le texte
 * \param    font   nom de la police à utiliser
 * \param    size   taille de police
 * \param    txt    texte à afficher
 * \param    x      abscisse
 * \param    y      ordonnée
 * \return   void
 */
void afficherTexteCentre (SDL_Surface *ecran, char *font, int size, char *txt, int x, int y) {
    TTF_Font *police = TTF_OpenFont(font, size);
    SDL_Color Black = {0, 0, 0};
    SDL_Color White = {255, 255, 255};
    SDL_Rect positiontexte;
    SDL_Surface *texte = TTF_RenderText_Shaded(police, txt, Black, White);
    positiontexte.x = x-texte->w/2;
    positiontexte.y = y-texte->h/2;
    SDL_BlitSurface(texte, NULL, ecran, &positiontexte);
    TTF_CloseFont(police);
}

/**
 * \brief      Afficher un rectangle à l'écran via SDL
 * \details    Affiche un rectangle avec origine en haut à gauche.
 * \param    ecran  surface sur laquelle on affiche le rectangle
 * \param    largeur    largeur du rectangle
 * \param    hauteur    hauteur du rectangle
 * \param    posx   abscisse
 * \param    posy   ordonnée
 * \param    alpha  transparence (valeur comprise entre 0 et 255)
 * \return   void
 */
void afficherRectangle (SDL_Surface *ecran, int largeur, int hauteur, int posx, int posy, int alpha) {
    SDL_Surface *rectangle = NULL;
    SDL_Rect position;

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_SetAlpha(rectangle, SDL_SRCALPHA, alpha);
    SDL_SetColorKey(rectangle, SDL_SRCCOLORKEY, SDL_MapRGB(rectangle->format, 255, 255, 255));

    position.x = posx;
    position.y = posy;
    SDL_BlitSurface(rectangle, NULL, ecran, &position);
}

/**
 * \brief      Afficher un rectangle à l'écran via SDL
 * \details    Affiche un rectangle avec origine au centre.
 * \param    ecran  surface sur laquelle on affiche le rectangle
 * \param    largeur    largeur du rectangle
 * \param    hauteur    hauteur du rectangle
 * \param    posx   abscisse
 * \param    posy   ordonnée
 * \param    alpha  transparence (valeur comprise entre 0 et 255)
 * \return   void
 */
void afficherRectangleCentre (SDL_Surface *ecran, int largeur, int hauteur, int posx, int posy, int alpha) {
    SDL_Surface *rectangle = NULL;
    SDL_Rect position;

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
    SDL_SetAlpha(rectangle, SDL_SRCALPHA, alpha);
    SDL_SetColorKey(rectangle, SDL_SRCCOLORKEY, SDL_MapRGB(rectangle->format, 255, 255, 255));

    position.x = posx-rectangle->w/2 ;
    position.y = posy-rectangle->h/2;
    SDL_BlitSurface(rectangle, NULL, ecran, &position);
}

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h> // pensez à modifier le chemin
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "affichage.h"

void afficherTexte (SDL_Surface *ecran, char *font, int size, char *txt, int x, int y) {
    TTF_Font *police = TTF_OpenFont(font, size);
    SDL_Color Black = {0, 0, 0};
    SDL_Rect positiontexte;
    SDL_Surface *texte = TTF_RenderText_Blended(police, txt, Black);
    positiontexte.x = x;
    positiontexte.y = y;
    SDL_BlitSurface(texte, NULL, ecran, &positiontexte);
    TTF_CloseFont(police);
}

void afficherTexteCentre (SDL_Surface *ecran, char *font, int size, char *txt, int x, int y) {
    TTF_Font *police = TTF_OpenFont(font, size);
    SDL_Color Black = {0, 0, 0};
    SDL_Rect positiontexte;
    SDL_Surface *texte = TTF_RenderText_Blended(police, txt, Black);
    positiontexte.x = x-texte->w/2;
    positiontexte.y = y-texte->h/2;
    SDL_BlitSurface(texte, NULL, ecran, &positiontexte);
    TTF_CloseFont(police);
}

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

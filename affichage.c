#include <stdlib.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "main.h"
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"
#include "terrains.h"

#include "edit_main.h"
#include "edit_affichage.h"

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

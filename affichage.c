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

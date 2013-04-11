#define SORTIE 502713
#define MENU 7

SDL_Surface* selection(int largeur, int hauteur, SDL_PixelFormat *pf);
int gameover(SDL_Surface *ecran);
int menu(SDL_Surface *ecran, TTF_Font *police);
int pause(SDL_Surface* ecran);
int victoire(SDL_Surface *ecran);
int choixNiveau (SDL_Surface *ecran);


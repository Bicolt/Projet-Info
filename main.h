#define SORTIE 502713

void menu(SDL_Surface *ecran, TTF_Font *police, int *choix);
int pause(SDL_Surface* ecran);
SDL_Surface* selection(int largeur, int hauteur, SDL_PixelFormat *pf);
int choixNiveau (SDL_Surface *ecran);
int gameover(SDL_Surface *ecran);

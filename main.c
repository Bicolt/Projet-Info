#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "affichage.h"
#include "interface.h"
#include "niveau.h"


int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
    TTF_Init();

    SDL_Surface *ecran = NULL, *icone = SDL_LoadBMP("sdl_icone.bmp");
    TTF_Font *police = NULL;
    int choix = 0, retourNiveau = 0, retourGO = 0, retourVict = 0;
    int continuer = 1;

    SDL_WM_SetIcon(icone, NULL);
    ecran = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    if (ecran == NULL){  // Si l'ouverture a échoué, on le note et on arrête
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("essai de menu", NULL);

    police = TTF_OpenFont("ariblk.ttf", 55);

    while(continuer > 0){
        choixMenu: menu(ecran, police, &choix);
        if(choix == 0){
            switch(choixNiveau(ecran))
            {
				case SORTIE:
                    continuer=0;
                    break;
                case 0:
                    tuto(ecran);
                    goto choixMenu;
                    break;
                case 1:
                    Niveaux: retourNiveau = niveau(ecran);
                    if(retourNiveau == 0){
                        retourVict = victoire(ecran);
                        if (retourVict == 0)
                            goto choixMenu;
                        else if (retourVict == 1)
                            goto choixMenu;
                        else if (retourVict == SORTIE)
                            continuer =0;
                        else continuer =0;
                    }
					if(retourNiveau == -1){
						retourGO = gameover(ecran);
						if(retourGO == 0)
							goto Niveaux;
						else if(retourGO == 1)
							goto choixMenu;
						else if(retourGO == SORTIE)
							continuer = 0;
						else continuer = 0;
					}
					else if(retourNiveau == 1)
                        goto choixMenu;
                default:
                    goto choixMenu;
                    break;
            }
        }
        else if(choix == 2)
            continuer = 0;
    }

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}


#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "main.h"
#include "affichage.h"
#include "interface.h"
#include "niveau.h"

int ew, eh;

int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
    TTF_Init();

    SDL_Surface *ecran = NULL, *icone = SDL_LoadBMP("sdl_icone.bmp");
    TTF_Font *police = NULL;
    int retourMenu = 0, retourChoixNiveau = 0, retourNiveau = 0, retourGO = 0, retourVict = 0;
    int continuer = 1, enNiveau = 1;

    SDL_WM_SetIcon(icone, NULL);
    ecran = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if (ecran->h > ecran->w*9/16)
        ecran = SDL_SetVideoMode(ecran->w, ecran->w*9/16, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN );
    else
        ecran = SDL_SetVideoMode(ecran->h*16/9, ecran->h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN );
    eh = ecran->h;
    ew = ecran->w;
    if (ecran == NULL){  // Si l'ouverture a échoué, on le note et on arrête
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("essai de menu", NULL);

    police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);

    while(continuer){
        enNiveau=1;
        retourMenu = menu(ecran,police);
        if(retourMenu == 0){
            retourChoixNiveau = choixNiveau(ecran);
            if((0<=retourChoixNiveau) && (retourChoixNiveau<=6)){
                while(enNiveau){
                    retourNiveau = niveau(ecran, retourChoixNiveau);
                    switch(retourNiveau){
                        case -1:
                            retourGO = gameover(ecran);
                            switch(retourGO){
                                case 0:
                                    break;
                                case MENU:
                                    enNiveau=0;
                                    break;
                                case SORTIE:
                                    enNiveau=0;
                                    continuer=0;
                                    break;
                            }
                            break;
                        case 0: //victoire
                            retourVict = victoire(ecran);
                            switch(retourVict){
                                case 0:
                                    if(retourChoixNiveau<6){
                                        retourChoixNiveau++;
                                    }
                                    else retourChoixNiveau = 1; //ecran spécial vitoire finale !
                                    break;
                                case MENU:
                                    enNiveau=0;
                                    break;
                                case SORTIE:
                                    enNiveau=0;
                                    continuer=0;
                            }
                            break;
                        case MENU:
                            enNiveau=0;
                            break;
                        case SORTIE:
                            enNiveau=0;
                            continuer=0;
                            break;
                    }
                }
            }
            else if(retourChoixNiveau==MENU){
                enNiveau=0;
            }
            else if(retourChoixNiveau==SORTIE){
                enNiveau=0;
                continuer=0;
            }
        }
        else if(retourMenu == 1){} //options
        else if(retourMenu == SORTIE){
            continuer = 0;
        }
    }

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}


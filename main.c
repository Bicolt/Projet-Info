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
#include "fmod.h"

int ew, eh;

int main(int argc, char *argv[]){

    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // �criture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
    TTF_Init();
    SDL_Surface *ecran = NULL, *icone = SDL_LoadBMP("sdl_icone.bmp");
    TTF_Font *police = NULL;
    int retourMenu = 0, retourChoixNiveau = 0, retourNiveau = 0, retourGO = 0, retourVict = 0;
    int continuer = 1, enNiveau = 1;

    FMOD_SYSTEM *system;
    FMOD_SOUND *lose, *win, *mort, *menumus, *gamemus;
    FMOD_CHANNELGROUP *channelgroup;
    FMOD_RESULT resultat;
    /* Cr�ation et initialisation d'un objet syst�me */
    FMOD_System_Create(&system);
    FMOD_System_Init(system, 32, FMOD_INIT_NORMAL, NULL);
    /* Chargement du son et v�rification du chargement */
    resultat = FMOD_System_CreateSound(system, "OST-Lose.wav", FMOD_CREATESAMPLE, 0, &lose);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier\n");
        exit(EXIT_FAILURE);
    }
    resultat = FMOD_System_CreateSound(system, "OST-Win.wav", FMOD_CREATESAMPLE, 0, &win);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier\n");
        exit(EXIT_FAILURE);
    }
    resultat = FMOD_System_CreateSound(system, "OST-Lose.wav", FMOD_CREATESAMPLE, 0, &mort);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier\n");
        exit(EXIT_FAILURE);
    }
    resultat = FMOD_System_CreateSound(system, "menumus.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &menumus);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier mp3\n");
        exit(EXIT_FAILURE);
    }
    resultat = FMOD_System_CreateSound(system, "gamemus.mp3", FMOD_SOFTWARE | FMOD_2D | FMOD_CREATESTREAM, 0, &gamemus);
    if (resultat != FMOD_OK)
    {
        fprintf(stderr, "Impossible de lire le fichier mp3\n");
        exit(EXIT_FAILURE);
    }

    /* On active la r�p�tition de la musique � l'infini */
    FMOD_Sound_SetLoopCount(menumus, -1);
    FMOD_Sound_SetLoopCount(gamemus, -1);

    FMOD_System_GetMasterChannelGroup(system, &channelgroup);

    /* On joue la musique */
    FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, menumus, 0, NULL);


    SDL_WM_SetIcon(icone, NULL);
    ecran = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    eh = ecran->h;
    ew = ecran->w;
    if (ecran == NULL){  // Si l'ouverture a �chou�, on le note et on arr�te
        fprintf(stderr, "Impossible de charger le mode vid�o : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("essai de menu", NULL);

    police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);
    if (police==NULL)
        printf("La police VirtualVectorVortex.ttf n'est pas dans le bon repertoire.\n");

    while(continuer){
        enNiveau=1;
        retourMenu = menu(ecran,police);
        if(retourMenu == 0){
            retourChoixNiveau = choixNiveau(ecran);
            if((0<=retourChoixNiveau) && (retourChoixNiveau<=6)){
                while(enNiveau){
                        FMOD_ChannelGroup_Stop(channelgroup);
                        FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, gamemus, 0, NULL);
                    retourNiveau = niveau(ecran, retourChoixNiveau);
                    switch(retourNiveau){
                        case -1:
                            FMOD_ChannelGroup_Stop(channelgroup);
                             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, menumus, 0, NULL);
                             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, mort, 0, NULL);
                             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, lose, 0, NULL);
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
                            FMOD_ChannelGroup_Stop(channelgroup);
                             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, menumus, 0, NULL);
                            FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, win, 0, NULL);
                            retourVict = victoire(ecran);
                            switch(retourVict){
                                case 0:
                                    if(retourChoixNiveau<6){
                                        retourChoixNiveau++;
                                    }
                                    else retourChoixNiveau = 1; //ecran sp�cial vitoire finale !
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
                            FMOD_ChannelGroup_Stop(channelgroup);
                             FMOD_System_PlaySound(system, FMOD_CHANNEL_FREE, menumus, 0, NULL);
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
    /* On lib�re le son et on ferme et lib�re l'objet syst�me */
    FMOD_Sound_Release(menumus);
    FMOD_Sound_Release(gamemus);
    FMOD_Sound_Release(lose);
    FMOD_Sound_Release(mort);
    FMOD_Sound_Release(win);
    FMOD_System_Close(system);
    FMOD_System_Release(system);
    return EXIT_SUCCESS;
}


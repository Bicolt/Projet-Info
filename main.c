/**
 * \file      	main.c
 * \version   	1.0
 * \date      	2013
 * \brief		Gestion globale de l'application
 *
 * \details   	Permet d'appeler les différentes fonctions qui composent le programme
 * \details		et gère les différentes valeurs de retour
 */

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
#include <SDL_mixer.h>

#include "edit_main.h"
#include "edit_affichage.h"

/** 
 * \param ew largeur de l'écran
 * \param eh hauteur de l'écran
 */
int ew, eh;
/**
 * indicateur de volume
 */
int mute = 0;
Mix_Music *musique;

/**
 * \brief Gestion globale du programme
 *
 * main fait appel aux fonctions permettant d'afficher les niveaux,
 * les menus, les écrans spéciaux et l'éditeur de terrain
 * et gère les retours de ces différentes fonctions 
 */
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

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1) //Initialisation de l'API Mixer
    {
        fprintf(stderr,"%s", Mix_GetError());
    }

    Mix_AllocateChannels(2); // allocation de 2 canaux de son
    Mix_Chunk *sonWin;
    Mix_Chunk *sonLose;
    Mix_Chunk *sonMort;
    sonWin = Mix_LoadWAV("GameSound/OST-Win.wav"); //Chargement du son joué lors d'une victoire
    sonLose = Mix_LoadWAV("GameSound/OST-Lose.wav"); //Chargement du son joué lors d'une défaite
    sonMort = Mix_LoadWAV("GameSound/OST-Mort.wav"); //Chargement du son joué lors d'une mort

    musique = Mix_LoadMUS("GameSound/menumus.mp3"); //Chargement de la musique du menu

    SDL_WM_SetIcon(icone, NULL);
    ecran = SDL_SetVideoMode(0, 0, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    eh = ecran->h;
    ew = ecran->w;
    if (ecran == NULL){  // Si l'ouverture a échoué, on le note et on arrête
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
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
                        musique = Mix_LoadMUS("GameSound/gamemus.mp3"); //Chargement de la musique du jeu 
                        if (!mute) {
                            Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
                        }
                    retourNiveau = niveau(ecran, retourChoixNiveau);
                    switch(retourNiveau){
                        case -1:
                            musique = Mix_LoadMUS("GameSound/menumus.mp3"); //Chargement de la musique
                            if (!mute) {
                                Mix_PlayChannel(-1, sonMort, 0);
                                Mix_PlayChannel(-1, sonLose, 0);
                                Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
                            }
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
                            musique = Mix_LoadMUS("GameSound/menumus.mp3"); //Chargement de la musique
                            if (!mute) {
                                Mix_PlayChannel(-1, sonWin, 0);
                                Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
                            }

                            retourVict = victoire(ecran);
                            switch(retourVict){
                                case 0:
                                    if(retourChoixNiveau<6){
                                        retourChoixNiveau++;
                                    }
                                    else retourChoixNiveau = 1; //écran spécial vitoire finale !
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
                            musique = Mix_LoadMUS("GameSound/menumus.mp3"); //Chargement de la musique
                            if (!mute) {
                                Mix_PlayMusic(musique, -1); //Jouer infiniment la musique
                            }
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
        else if(retourMenu == 1){ //Editeur de terrain
            edit_main(ecran);
        }
        else if(retourMenu == SORTIE){
            continuer = 0;
        }
    }

    TTF_CloseFont(police);
    TTF_Quit();
    Mix_FreeMusic(musique);
    Mix_FreeChunk(sonWin);
    Mix_FreeChunk(sonLose);
    Mix_FreeChunk(sonMort);
    Mix_CloseAudio(); //Fermeture de l'API
    SDL_Quit();
    return EXIT_SUCCESS;
}


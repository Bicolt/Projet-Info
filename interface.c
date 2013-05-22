#include <stdlib.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "main.h"
#include "niveau.h"
#include "affichage.h"
#include "interface.h"
#include <SDL/SDL_mixer.h>

#include "edit_main.h"
#include "edit_affichage.h"


// sélection sous forme de barre horizontale
SDL_Surface* selection(int largeur, int hauteur, SDL_PixelFormat *pf){

    SDL_Surface *rectangle = NULL;

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(pf, 0, 0, 0));
    SDL_SetAlpha(rectangle, SDL_SRCALPHA, 100);
    SDL_SetColorKey(rectangle, SDL_SRCCOLORKEY, SDL_MapRGB(rectangle->format, 255, 255, 255));

    return rectangle;
}

//menu
int menu(SDL_Surface *ecran, TTF_Font *police){
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos, posmute;
    int xSouris, ySouris;
    int continuer = 1, select=0;

    musique = Mix_LoadMUS("GameSound/menumus.mp3"); //Chargement de la musique
    if (!mute)
        Mix_PlayMusic(musique, -1);

    SDL_Surface *speaker;
    speaker = SDL_LoadBMP("speaker.bmp");
    if (mute)
        SDL_SetAlpha(speaker, SDL_SRCALPHA, 100);
    posmute.x = 0; posmute.y = 0;

    pos.x = 0; pos.y = 0;

    rect = selection(ew, eh/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "Super Scalable Land", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Jouer", Black, White);
	texte2 = TTF_RenderText_Shaded(police, "Editeur de terrain", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ew - titre->w)/2);
    positiontitre.y = (eh/8);
    positiontexte1.x = ((ew - texte1->w)/2);
    positiontexte2.x = ((ew - texte2->w)/2);
	positiontexte3.x = ((ew - texte3->w)/2);
    positiontexte1.y = (eh*6/16);
	positiontexte2.y = (eh*9/16);
    positiontexte3.y = (eh*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_BlitSurface(speaker, NULL, ecran, &posmute);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);

    while(continuer){
        while (SDL_PollEvent(&event)) {
            switch(event.type)
            {
                case SDL_MOUSEMOTION:
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
                        select = 0;
                        posrec.y=eh*6/16;
                    }
                    else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                        select = 1;
                        posrec.y=eh*9/16;
                    }
                    else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
                        select = 2;
                        posrec.y=eh*12/16;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        xSouris = event.button.x;
                        ySouris = event.button.y;
                        if ((xSouris < speaker->w) && (ySouris < speaker->h)) {
                            mute = (mute+1)%2;
                            if (mute) {
                                SDL_SetAlpha(speaker, SDL_SRCALPHA, 100);
                                Mix_HaltMusic();
                            }
                            else {
                                SDL_SetAlpha(speaker, SDL_SRCALPHA, 255);
                                Mix_PlayMusic(musique, -1);
                            }
                        }
                        else if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
                            return 0;
                        }
                        else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                            Mix_HaltMusic();
                            return 1;
                        }
                        else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
                            return SORTIE;
                        }
                    }
                break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            select = (select + 2)%3;
                            if(select == 2)
                                posrec.y = posrec.y + eh*6/16;
                            else posrec.y = posrec.y - eh*3/16;
                            break;
                        case SDLK_DOWN:
                            select = (select + 1)%3;
                            if(select == 0)
                                posrec.y = posrec.y - eh*6/16;
                            else posrec.y = posrec.y + eh*3/16;
                            break;
                        case SDLK_ESCAPE:
                                return SORTIE;
                            break;
                        case SDLK_RETURN:
                            if(select == 2){
                                return SORTIE;
                            }
                            else if(select == 1){
                                Mix_HaltMusic();
                                return 1;
                            }
                            else{
                                return 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                        return SORTIE;
                    break;
            }
        }
    //SDL_Delay(30);

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(speaker, NULL, ecran, &posmute);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return SORTIE;
}

int jeupause(SDL_Surface* ecran){

    TTF_Font *police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte3, posrec, pos, posmute;
    int xSouris, ySouris;
    int continuer = 1, select=0;

    SDL_Surface *speaker;
    speaker = SDL_LoadBMP("speaker.bmp");
    if (mute)
        SDL_SetAlpha(speaker, SDL_SRCALPHA, 100);
    posmute.x = 0; posmute.y = 0;

    pos.x = 0; pos.y = 0;

    rect = selection(ew, eh/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "Pause", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Reprendre", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ew - titre->w)/2);
    positiontitre.y = (eh/8);
    positiontexte1.x = ((ew - texte1->w)/2);
	positiontexte3.x = ((ew - texte3->w)/2);
    positiontexte1.y = (eh*6/16);
    positiontexte3.y = (eh*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(speaker, NULL, ecran, &posmute);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);

    while(continuer){
        while(SDL_PollEvent(&event)) {
            switch(event.type)
            {
                case SDL_MOUSEMOTION:
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if((ySouris < (eh*9/16))){
                        select = 0;
                        posrec.y=eh*6/16;
                    }
                    else if(((eh*12/16) <= ySouris)){
                        select = 2;
                        posrec.y=eh*12/16;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        xSouris = event.button.x;
                        ySouris = event.button.y;
                        if ((xSouris < speaker->w) && (ySouris < speaker->h)) {
                            mute = (mute+1)%2;
                            if (mute) {
                                SDL_SetAlpha(speaker, SDL_SRCALPHA, 100);
                                Mix_HaltMusic();
                            }
                            else {
                                SDL_SetAlpha(speaker, SDL_SRCALPHA, 255);
                                Mix_PlayMusic(musique, -1);
                            }
                        }
                        else if((ySouris < (eh*9/16))){
                            return 0;
                        }
                        else if(((eh*12/16) <= ySouris)){
                            return MENU;
                        }
                    }
                break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            select = (select + 1)%2;
                            if(select == 1)
                                posrec.y = posrec.y + eh*6/16;
                            else posrec.y = posrec.y - eh*6/16;
                            break;
                        case SDLK_DOWN:
                            select = (select + 1)%2;
                            if(select == 0)
                                posrec.y = posrec.y - eh*6/16;
                            else posrec.y = posrec.y + eh*6/16;
                            break;
                        case SDLK_ESCAPE:
                                return 0;
                            break;
                        case SDLK_RETURN:
                            if(select == 1){
                                return MENU;
                            }
                            else{
                                return 0;
                            }
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                        return SORTIE;
                    break;
            }
        }

    //SDL_Delay(30);
    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(speaker, NULL, ecran, &posmute);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return MENU;
}


int choixNiveau (SDL_Surface *ecran) {

    /*TTF_Font *police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);*/
    SDL_Surface *fond = NULL;
    SDL_Event event;
    SDL_Rect pos;
    int xSouris, ySouris;

    int continuer = 1, select = 0;
    int centrex = ew/2;
    int posyTitre = eh/6, posy0 = 2*eh/6, posyl1 = 3*eh/6, posyl2 = 4*eh/6, posyMenu = 5*eh/6;
    int posx = centrex, posy = posy0, largeur = ew, hauteur = eh/7;

    pos.x = 0; pos.y = 0;

	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image


    SDL_BlitSurface(fond, NULL, ecran, &pos);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Choix du niveau", centrex, posyTitre);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Menu principal", centrex, posyMenu);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Tutoriel", centrex, posy0);
    afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "1", ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "2", 2*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "3", 3*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "4", 4*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "5", 5*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "6", 6*ew/7, posyl1);
    afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Niveau perso", centrex, posyl2);

    afficherRectangleCentre (ecran, largeur, hauteur, posx, posy, 100);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);


    while(continuer){
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                xSouris = event.button.x;
                ySouris = event.button.y;
                    if (ySouris < posy0 + eh/12)
                        select = 0;
                    if ((ySouris < posyl1 + eh/12)&&(ySouris > posyl1 - eh/12)){
                      /*  if ((xSouris > centrex - 3*ew/14)&&(xSouris < centrex + 3*ew/14)){
                            if (xSouris < centrex - ew/14)
                                select = 1;
                            else if (xSouris > centrex + ew/14)
                                select = 3;
                            else
                                select = 2;
                        }
                        largeur = hauteur; */
                        select = (xSouris+ew/14)/(ew/7);
                        largeur = hauteur;
                    }
                    else if  ((ySouris < posyl2 + eh/12)&&(ySouris > posyl2 - eh/12)){
                        select = NIVEAUPERSO;
                    }
                    else if (ySouris > posyMenu - eh/12)
                        select = MENU;
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT)
                    return select;
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        switch (select) {
                            case 0:
                                select = 7;
                                break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                select = 0;
                                break;
                            case NIVEAUPERSO:
                                select = 6;
                                break;
                            case MENU:
                                select = NIVEAUPERSO;
                                break;
                            default:
                                break;

                        }
                        break;
                    case SDLK_DOWN:
                        switch (select) {
                            case 0:
                                select = 1;
                                break;
                            case 1:
                            case 2:
                            case 3:
                            case 4:
                            case 5:
                            case 6:
                                select = NIVEAUPERSO;
                                break;
                            case NIVEAUPERSO:
                                select = MENU;
                                break;
                            case 7:
                                select = 0;
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDLK_RIGHT:
                        if ((select > 0) && (select < 6))
                            select = (select + 1)%7;
                        else if (select == 6)
                            select = 1;
                        break;
                    case SDLK_LEFT:
                        if ((select > 1) && (select < 7))
                            select = (select + 6)%7;
                        else if (select == 1)
                            select = 6;
                        break;
                    case SDLK_ESCAPE:
                        return MENU;
                        break;
                    case SDLK_RETURN:
                        return select;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                return SORTIE;
                break;
        }
    //SDL_Delay(30);
    if ((select > 0) && (select < 7)){
        posy = posyl1;
        largeur = hauteur;
        posx = select*ew/7;
    }
    switch (select){
        case 0:
            posx = centrex;
            posy = posy0;
            largeur = ew;
            break;
        case NIVEAUPERSO:
            posx = centrex;
            posy = posyl2;
            largeur = ew;
            break;
        case 7:
            posy = posyMenu;
            posx = centrex;
            largeur = ew;
            break;
        default:
            break;
    }
    SDL_BlitSurface(fond, NULL, ecran, &pos);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Choix du niveau", centrex, posyTitre);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Menu principal", centrex, posyMenu);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Tutoriel", centrex, posy0);
    afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "1", ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "2", 2*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "3", 3*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "4", 4*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "5", 5*ew/7, posyl1);
	afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "6", 6*ew/7, posyl1);
    afficherTexteCentre(ecran, "VirtualVectorVortex.ttf", 85*eh/768, "Niveau perso", centrex, posyl2);
	afficherRectangleCentre (ecran, largeur, hauteur, posx, posy, 100);
    SDL_Flip(ecran);
    }
    return MENU;
}

int gameover(SDL_Surface *ecran){ // Devra prendre en entrée plus tard le niveau chargé pour le recharger dans recommencer

    TTF_Font *police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos;
    int ySouris;
    int continuer = 1, select=0;

    pos.x = 0; pos.y = 0;

    rect = selection(ew, eh/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "GAME OVER", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Recommencer", Black, White);
    texte2 = TTF_RenderText_Shaded(police, "Menu principal", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ew - titre->w)/2);
    positiontitre.y = (eh/8);
    positiontexte1.x = ((ew - texte1->w)/2);
    positiontexte2.x = ((ew - texte2->w)/2);
	positiontexte3.x = ((ew - texte3->w)/2);
    positiontexte1.y = (eh*6/16);
    positiontexte2.y = (eh*9/16);
    positiontexte3.y = (eh*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);

   while(continuer){
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                ySouris = event.button.y;
                if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
                    select = 0;
                    posrec.y=eh*6/16;
                }
                else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                    select = 1;
                    posrec.y=eh*9/16;
                }
                else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
                    select = 2;
                    posrec.y=eh*12/16;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    ySouris = event.button.y;
                    if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
						return 0;
                    }
                    else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                        return MENU;
                    }
                    else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
						return SORTIE;
                    }
                }
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        select = (select + 2)%3;
                        if(select == 2)
                            posrec.y = posrec.y + 2*eh*3/16;
                        else posrec.y = posrec.y - eh*3/16;
                        break;
                    case SDLK_DOWN:
                        select = (select + 1)%3;
                        if(select == 0)
                            posrec.y = posrec.y - 2*eh*3/16;
                        else posrec.y = posrec.y + eh*3/16;
                        break;
                    case SDLK_ESCAPE:
                            return SORTIE;
                        break;
                    case SDLK_RETURN:
                        if(select == 2){
                            return SORTIE;
                        }
                        else if(select == 1){
                            return MENU;
                        }
                        else{
                            return 0;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                    return SORTIE;
                break;
        }
    //SDL_Delay(30);
    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return MENU;
}

int victoire(SDL_Surface *ecran){

    TTF_Font *police = TTF_OpenFont("VirtualVectorVortex.ttf", 85*eh/768);
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos;
    int ySouris;
    int continuer = 1, select=0;

    pos.x = 0; pos.y = 0;

    rect = selection(ew, eh/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "VICTOIRE !", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Niveau suivant", Black, White);
    texte2 = TTF_RenderText_Shaded(police, "Menu principal", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ew, eh, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ew - titre->w)/2);
    positiontitre.y = (eh/8);
    positiontexte1.x = ((ew - texte1->w)/2);
    positiontexte2.x = ((ew - texte2->w)/2);
	positiontexte3.x = ((ew - texte3->w)/2);
    positiontexte1.y = (eh*6/16);
    positiontexte2.y = (eh*9/16);
    positiontexte3.y = (eh*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);

   while(continuer){
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                ySouris = event.button.y;
                if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
                    select = 0;
                    posrec.y=eh*6/16;
                }
                else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                    select = 1;
                    posrec.y=eh*9/16;
                }
                else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
                    select = 2;
                    posrec.y=eh*12/16;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    ySouris = event.button.y;
                    if(((eh*6/16) <= ySouris) && (ySouris < (eh*9/16))){
						return 0;
                    }
                    else if(((eh*9/16) <= ySouris) && (ySouris < (eh*12/16))){
                        return MENU;
                    }
                    else if(((eh*12/16) <= ySouris) && (ySouris < (eh*14/16))){
						return SORTIE;
                    }
                }
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        select = (select + 2)%3;
                        if(select == 2)
                            posrec.y = posrec.y + eh*6/16;
                        else posrec.y = posrec.y - eh*6/16;
                        break;
                    case SDLK_DOWN:
                        select = (select + 1)%3;
                        if(select == 0)
                            posrec.y = posrec.y - eh*6/16;
                        else posrec.y = posrec.y + eh*6/16;
                        break;
                    case SDLK_ESCAPE:
                            return SORTIE;
                        break;
                    case SDLK_RETURN:
                        if(select == 2){
                            return SORTIE;
                        }
                        else if(select == 1){
                            return MENU;
                        }
                        else{
                            choixNiveau(ecran);
                            return 0;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                    return SORTIE;
                break;
        }
    //SDL_Delay(30);
    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return MENU;
}

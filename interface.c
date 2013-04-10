#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "niveau.h"
#include "affichage.h"
#include "interface.h"

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
void menu(SDL_Surface *ecran, TTF_Font *police, int *choix){
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos;
    int xSouris, ySouris;
    int continuer = 1, select=0;

    pos.x = 0; pos.y = 0;

    rect = selection(ecran->w, ecran->h/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "Super Scalable Land", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Jouer", Black, White);
	texte2 = TTF_RenderText_Shaded(police, "Options", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ecran->w - titre->w)/2);
    positiontitre.y = (ecran->h/8);
    positiontexte1.x = ((ecran->w - texte1->w)/2);
    positiontexte2.x = ((ecran->w - texte2->w)/2);
	positiontexte3.x = ((ecran->w - texte3->w)/2);
    positiontexte1.y = (ecran->h*6/16);
	positiontexte2.y = (ecran->h*9/16);
    positiontexte3.y = (ecran->h*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    afficherTexte(ecran, "ariblk.ttf", 18, "testlol", 12, 12);
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
                xSouris = event.button.x;
                ySouris = event.button.y;
                if(((ecran->h*6/16) <= ySouris) && (ySouris < (ecran->h*9/16))){
                    select = 0;
                    posrec.y=ecran->h*6/16;
                }
                else if(((ecran->h*9/16) <= ySouris) && (ySouris < (ecran->h*12/16))){
                    select = 1;
                    posrec.y=ecran->h*9/16;
                }
                else if(((ecran->h*12/16) <= ySouris) && (ySouris < (ecran->h*14/16))){
                    select = 2;
                    posrec.y=ecran->h*12/16;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if(((ecran->h*6/16) <= ySouris) && (ySouris < (ecran->h*9/16))){
                        *choix = 0; //jouer
                        continuer = 0;
                    }
                    else if(((ecran->h*9/16) <= ySouris) && (ySouris < (ecran->h*12/16))){
                        *choix = 1; //option
                        continuer = 0;
                    }
                    else if(((ecran->h*12/16) <= ySouris) && (ySouris < (ecran->h*14/16))){
                        *choix = 2; //quitter
                        continuer = 0;
                    }
                }
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        select = (select + 2)%3;
                        if(select == 2)
                            posrec.y = posrec.y + ecran->h*6/16;
                        else posrec.y = posrec.y - ecran->h*3/16;
                        break;
                    case SDLK_DOWN:
                        select = (select + 1)%3;
                        if(select == 0)
                            posrec.y = posrec.y - ecran->h*6/16;
                        else posrec.y = posrec.y + ecran->h*3/16;
                        break;
                    case SDLK_ESCAPE:
                            *choix = 2;
                            continuer = 0;
                        break;
                    case SDLK_RETURN:
                        if(select == 2){
                            *choix = 2;
                            continuer = 0;
                        }
                        else if(select == 1){
                            *choix = 1; //options
                            continuer = 0;
                        }
                        else{
                            *choix = 0;
                            continuer = 0;
                        }
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                    *choix = 2;
                    continuer = 0;
                break;
        }
    //SDL_Delay(30);

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    afficherTexte(ecran, "ariblk.ttf", 18, "testlol", 12, 12);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
}

int pause(SDL_Surface* ecran){

    TTF_Font *police = TTF_OpenFont("ariblk.ttf", 55);
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte3, posrec, pos;
    int xSouris, ySouris;
    int continuer = 1, select=0;

    pos.x = 0; pos.y = 0;

    rect = selection(ecran->w, ecran->h/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "Pause", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Reprendre", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ecran->w - titre->w)/2);
    positiontitre.y = (ecran->h/8);
    positiontexte1.x = ((ecran->w - texte1->w)/2);
	positiontexte3.x = ((ecran->w - texte3->w)/2);
    positiontexte1.y = (ecran->h*6/16);
    positiontexte3.y = (ecran->h*12/16);
    posrec.x = 0;
    posrec.y = positiontexte1.y;

    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);

    SDL_EnableKeyRepeat(150, 10);

    while(continuer){
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_MOUSEMOTION:
                xSouris = event.button.x;
                ySouris = event.button.y;
                if((ySouris < (ecran->h*9/16))){
                    select = 0;
                    posrec.y=ecran->h*6/16;
                }
                else if(((ecran->h*12/16) <= ySouris)){
                    select = 2;
                    posrec.y=ecran->h*12/16;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if((ySouris < (ecran->h*9/16))){
                        return 0;
                    }
                    else if(((ecran->h*12/16) <= ySouris)){
                        return 1;
                    }
                }
            break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_UP:
                        select = (select + 1)%2;
                        if(select == 1)
                            posrec.y = posrec.y + ecran->h*6/16;
                        else posrec.y = posrec.y - ecran->h*6/16;
                        break;
                    case SDLK_DOWN:
                        select = (select + 1)%2;
                        if(select == 0)
                            posrec.y = posrec.y - ecran->h*6/16;
                        else posrec.y = posrec.y + ecran->h*6/16;
                        break;
                    case SDLK_ESCAPE:
                            return 1;
                        break;
                    case SDLK_RETURN:
                        if(select == 1){
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
    //SDL_Delay(30);
    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return 1;
}


int choixNiveau (SDL_Surface *ecran) {

    TTF_Font *police = TTF_OpenFont("ariblk.ttf", 55);
    SDL_Surface *fond = NULL;
	SDL_Color Black = {0, 0, 0};
    SDL_Event event;
    SDL_Rect pos;
    int xSouris, ySouris;
    int continuer = 1, select = 1;
    int posx = ecran->w/2 - ecran->h/8, posy = ecran->h*7/16, largeur = ecran->h/12, hauteur = ecran->h/12;

    pos.x = 0; pos.y = 0;

	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image


    SDL_BlitSurface(fond, NULL, ecran, &pos);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "Choix du niveau", ecran->w/2, ecran->h/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "Menu principal", ecran->w/2, ecran->h*13/16);
    afficherTexteCentre(ecran, "ariblk.ttf", 58, "1", ecran->w/2 - ecran->h/8, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "2", ecran->w/2, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "3", ecran->w/2 + ecran->h/8, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "4", ecran->w/2 - ecran->h/8, ecran->h*5/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "5", ecran->w/2, ecran->h*5/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "6", ecran->w/2 + ecran->h/8, ecran->h*5/8);

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
                    if ((ySouris < 350)&&(ySouris > 250)){
                        if ((xSouris > ecran->w/2 - 200)&&(xSouris < ecran->w/2 + 200)){
                            if (xSouris < ecran->w/2 - 100)
                                select = 1;
                            else if (xSouris > ecran->w/2 +100)
                                select = 3;
                            else
                                select = 2;
                        }
                        largeur = hauteur = ecran->h/12;
                    }
                    else if  ((ySouris < 500)&&(ySouris > 400)){
                        if ((xSouris > ecran->w/2 - 200)&&(xSouris < ecran->w/2 + 200)){
                            if (xSouris < ecran->w/2 - 100)
                                select = 4;
                            else if (xSouris > ecran->w/2 +100)
                                select = 6;
                            else
                                select = 5;
                        }
                        largeur = hauteur = ecran->h/12;
                    }
                    else if (ySouris > 600)
                        select = 7;
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
                            case 1:
                            case 2:
                            case 3:
                                select = 7;
                                break;
                            case 4:
                            case 5:
                            case 6:
                                select = select - 3;
                                break;
                            case 0:
                            case 7:
                                select = 6;
                                break;
                            default:
                                break;

                        }
                        break;
                    case SDLK_DOWN:
                        switch (select) {
                            case 1:
                            case 2:
                            case 3:
                                select = select + 3;
                                break;
                            case 4:
                            case 5:
                            case 6:
                                select = 7;
                                break;
                            case 0:
                            case 7:
                                select = 1;
                                break;
                            default:
                                break;
                        }
                        break;
                    case SDLK_RIGHT:
                        select = (select + 1)%7;
                        break;
                    case SDLK_LEFT:
                        select = (select + 6)%7;
                        break;
                    case SDLK_ESCAPE:
                        return 7;
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
    if (select < 7 ){
        if (select < 4 )
            posy = ecran->h*7/16;
        else
            posy = ecran->h*5/8;
        largeur = ecran->h/12;
    }
    switch (select){
        case 1:
        case 4:
            posx = ecran->w/2 - ecran->h/8;
            break;
        case 2:
        case 5:
            posx = ecran->w/2;
            break;
        case 3:
        case 6:
            posx = ecran->w/2 + ecran->h/8;
            break;
        default:
            posy = ecran->h*13/16;
            posx = ecran->w/2;
            largeur = ecran->w;
    }
    SDL_BlitSurface(fond, NULL, ecran, &pos);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "Choix du niveau", ecran->w/2, ecran->h/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "Menu principal", ecran->w/2, ecran->h*13/16);
    afficherTexteCentre(ecran, "ariblk.ttf", 58, "1", ecran->w/2 - ecran->h/8, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "2", ecran->w/2, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "3", ecran->w/2 + ecran->h/8, ecran->h*7/16);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "4", ecran->w/2 - ecran->h/8, ecran->h*5/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "5", ecran->w/2, ecran->h*5/8);
	afficherTexteCentre(ecran, "ariblk.ttf", 58, "6", ecran->w/2 + ecran->h/8, ecran->h*5/8);
	afficherRectangleCentre (ecran, largeur, hauteur, posx, posy, 100);
    SDL_Flip(ecran);
    }
    return 0;
}

int gameover(SDL_Surface *ecran){ // Devra prendre en entrée plus tard le niveau chargé pour le recharger dans recommencer

    TTF_Font *police = TTF_OpenFont("ariblk.ttf", 55);
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *fond = NULL;
	SDL_Color Black = {0, 0, 0};
	SDL_Color White = {255, 255, 255};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos;
    int xSouris, ySouris;
    int continuer = 1, select=0;

    pos.x = 0; pos.y = 0;

    rect = selection(ecran->w, ecran->h/8, ecran->format);
    titre = TTF_RenderText_Shaded(police, "GAME OVER", Black, White);
    texte1 = TTF_RenderText_Shaded(police, "Recommencer", Black, White);
    texte2 = TTF_RenderText_Shaded(police, "Menu principal", Black, White);
    texte3 = TTF_RenderText_Shaded(police, "Quitter", Black, White);
	fond = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
	SDL_FillRect(fond, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // fond = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    positiontitre.x = ((ecran->w - titre->w)/2);
    positiontitre.y = (ecran->h/8);
    positiontexte1.x = ((ecran->w - texte1->w)/2);
    positiontexte2.x = ((ecran->w - texte2->w)/2);
	positiontexte3.x = ((ecran->w - texte3->w)/2);
    positiontexte1.y = (ecran->h*6/16);
    positiontexte2.y = (ecran->h*9/16);
    positiontexte3.y = (ecran->h*12/16);
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
                xSouris = event.button.x;
                ySouris = event.button.y;
                if(((ecran->h*6/16) <= ySouris) && (ySouris < (ecran->h*9/16))){
                    select = 0;
                    posrec.y=ecran->h*6/16;
                }
                else if(((ecran->h*9/16) <= ySouris) && (ySouris < (ecran->h*12/16))){
                    select = 1;
                    posrec.y=ecran->h*9/16;
                }
                else if(((ecran->h*12/16) <= ySouris) && (ySouris < (ecran->h*14/16))){
                    select = 2;
                    posrec.y=ecran->h*12/16;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    xSouris = event.button.x;
                    ySouris = event.button.y;
                    if(((ecran->h*6/16) <= ySouris) && (ySouris < (ecran->h*9/16))){
                        niveau(ecran);
						return 0;
                    }
                    else if(((ecran->h*9/16) <= ySouris) && (ySouris < (ecran->h*12/16))){
                        return 1;
                    }
                    else if(((ecran->h*12/16) <= ySouris) && (ySouris < (ecran->h*14/16))){
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
                            posrec.y = posrec.y + ecran->h*6/16;
                        else posrec.y = posrec.y - ecran->h*6/16;
                        break;
                    case SDLK_DOWN:
                        select = (select + 1)%3;
                        if(select == 0)
                            posrec.y = posrec.y - ecran->h*6/16;
                        else posrec.y = posrec.y + ecran->h*6/16;
                        break;
                    case SDLK_ESCAPE:
                            return SORTIE;
                        break;
                    case SDLK_RETURN:
                        if(select == 2){
                                  return SORTIE;
                            continuer = 0;
                        }
                        else if(select == 1){
                            return 1;
                        }
                        else{
                            niveau(ecran);
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
    SDL_Delay(30);
    SDL_BlitSurface(fond, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, ecran, &positiontitre);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return 1;
}


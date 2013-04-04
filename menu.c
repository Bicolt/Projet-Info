#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL.h> // pensez � modifier le chemin
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

int menu(SDL_Surface *ecran, TTF_Font *police);

int main(int argc, char *argv[]){

    SDL_Surface *ecran = NULL; // *icone = SDL_LoadBMP("ressources/sdl_icone.bmp");
    TTF_Font *police = NULL;
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // �criture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
    TTF_Init();
    int continuer = 1;

    // SDL_WM_SetIcon(icone, NULL);
    ecran = SDL_SetVideoMode(0, 0, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    if (ecran == NULL) // Si l'ouverture a �chou�, on le note et on arr�te
    {
        fprintf(stderr, "Impossible de charger le mode vid�o : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("essai de menu", NULL);
    police = TTF_OpenFont("ariblk.ttf", 55);

    while(continuer > 0){
        while(menu(ecran, police) > 0){
            // � compl�t�ter
        }
        continuer = 0;
    }

    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    return EXIT_SUCCESS;
}



//cr�ation d'un cadre de selection
/* SDL_Surface* selection(int largeur, int hauteur, SDL_PixelFormat *pf){

    SDL_Surface *rectangle = NULL, *barre_verticale = NULL, *barre_horizontale;
    SDL_Rect positiondebut;
    positiondebut.x = 0;
    positiondebut.y = 0;

    barre_verticale =  SDL_CreateRGBSurface(SDL_HWSURFACE, 5, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(barre_verticale, NULL, SDL_MapRGB(pf, 0, 0, 0));
    barre_horizontale = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, 5, 32, 0, 0, 0, 0);
    SDL_FillRect(barre_horizontale, NULL, SDL_MapRGB(pf, 0, 0, 0));
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(pf, 255, 255, 255));
    SDL_SetColorKey(rectangle, SDL_SRCCOLORKEY, SDL_MapRGB(rectangle->format, 255, 255, 255));
    SDL_BlitSurface(barre_horizontale, NULL, rectangle, &positiondebut);
    SDL_BlitSurface(barre_verticale, NULL, rectangle, &positiondebut);
    positiondebut.x = largeur-5;
    SDL_BlitSurface(barre_verticale, NULL, rectangle, &positiondebut);
    positiondebut.y = hauteur-5;
    positiondebut.x = 0;
    SDL_BlitSurface(barre_horizontale, NULL, rectangle, &positiondebut);
    return rectangle;
} */

// s�lection sous forme de barre horizontale
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
    SDL_Surface *titre = NULL, *texte1 =NULL, *texte2 = NULL, *texte3 = NULL, *rect = NULL, *font = NULL;
	SDL_Color Black = {0, 0, 0};
    SDL_Event event;
    SDL_Rect positiontitre, positiontexte1, positiontexte2, positiontexte3, posrec, pos;
    int continuer = 1, select=0;
    pos.x = 0; pos.y = 0;
    positiontitre.x = (ecran->w/2 - 300);
    positiontitre.y = (ecran->h/2 - 300);
    positiontexte1.x = (ecran->w/2 - 80);
    positiontexte2.x = (ecran->w/2 - 110);
	positiontexte3.x = (ecran->w/2 - 100);
    positiontexte1.y = (ecran->h/2 - 175);
	positiontexte2.y = (ecran->h/2 + 15);
    positiontexte3.y = (ecran->h/2 + 205);
    posrec.x = 0;
    posrec.y = positiontexte1.y + 5;

    rect = selection(ecran->w, 80, ecran->format);
    titre = TTF_RenderText_Blended(police, "Super Scalable Land", Black);
    texte1 = TTF_RenderText_Blended(police, "Jouer", Black);
	texte2 = TTF_RenderText_Blended(police, "Options", Black);
    texte3 = TTF_RenderText_Blended(police, "Quitter", Black);
	font = SDL_CreateRGBSurface(SDL_HWSURFACE, ecran->w, ecran->h, 32, 0, 0, 0, 0);
	SDL_FillRect(font, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    // font = IMG_Load("ressources/1.jpg"); lorsque le fond est une image

    SDL_BlitSurface(font, NULL, ecran, &pos);
    SDL_BlitSurface(titre, NULL, font, &positiontitre);
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
            case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_UP:
                    select = (select + 2)%3;
                    if(select == 2)
                        posrec.y = posrec.y + 380;
                    else posrec.y = posrec.y - 190;
                    break;
                case SDLK_DOWN:
                    select = (select + 1)%3;
                    if(select == 0)
                        posrec.y = posrec.y - 380;
                    else posrec.y = posrec.y + 190;
                    break;
                case SDLK_ESCAPE:
                    continuer = 0;
                    break;
                case SDLK_RETURN:
                    if(select == 2)
                        continuer = 0;
                    else if(select == 1){
						return 1; //options
                    }
					else return 2; //jouer
                    break;
                default:
                    break;
            }
            break;
            case SDL_QUIT:
                continuer = 0;
                break;
        }
    //SDL_Delay(30);
    SDL_BlitSurface(font, NULL, ecran, &pos);
    SDL_BlitSurface(texte1, NULL, ecran, &positiontexte1);
    SDL_BlitSurface(texte2, NULL, ecran, &positiontexte2);
	SDL_BlitSurface(texte3, NULL, ecran, &positiontexte3);
    SDL_BlitSurface(rect, NULL, ecran, &posrec);
    SDL_Flip(ecran);
    }
    return 0;
}

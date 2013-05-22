#include <stdlib.h>
#include <stdio.h>
#include <cairo/cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "main.h"
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"
#include "terrains.h"

#include "edit_main.h"
#include "edit_affichage.h"
#include <unistd.h>

int eEPAISSEUR_TRAIT;
int W_SUR_H;

/*
typedef enum {KeyPress, KeyUp, KeyRelease, MouseButtonPress, MouseButtonRelease, MouseButtonDown} Type;
typedef enum {n, r, e, s, d, left, right} Key;
typedef enum {Type type, int x, int y, Key key} Event;
*/





int edit_main(SDL_Surface *ecran){
/*
    if(SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError()); // Écriture de l'erreur
        exit(EXIT_FAILURE); // On quitte le programme
    }
    TTF_Init();
*/
//    SDL_Surface *ecran = NULL;
    if (ecran == NULL){  // Si l'ouverture a échoué, on le note et on arrête
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    SDL_WM_SetCaption("Editeur de terrain", NULL);
    eEPAISSEUR_TRAIT = 7*ecran->h/768;
    W_SUR_H = 10;

    Mode mode = Dessin;
    Bool continuer = Vrai;
    SDL_Event ev;
    SDL_EnableKeyRepeat(200, 20);
    Couleur couleur = Noir;
    Fragment* fragments = NULL;
    int posecran = 0;

    //Variables pour l'affichage
    cairo_surface_t *surfaceFond = NULL;
    SDL_Surface *surfLigne = NULL;
    SDL_Rect positionecran, tailleecran;
    tailleecran.x = 0;
    tailleecran.y = 0;
    tailleecran.h = ecran->h;
    tailleecran.w = ecran->w;
    positionecran.y = 0;
    positionecran.x = 0;
    positionecran.h = ecran->h;
    positionecran.w = ecran->w;
    cairo_t *droite = NULL;

    surfLigne = SDL_CreateRGBSurface(SDL_HWSURFACE, W_SUR_H * ecran->h, ecran->h, 32, 0, 0, 0, 255);
    surfaceFond = cairo_image_surface_create_for_data (surfLigne->pixels,
                                                        CAIRO_FORMAT_ARGB32,
                                                        surfLigne->w,
                                                        surfLigne->h,
                                                        surfLigne->pitch);



	//Variables en mode Selection
	//int selection = -1; //-1 = pas de selection
	//Bool deplacer = Faux; //Vrai -> l'objet suit la sélection

	//Variables en mode Dessin
	Bool debut = Vrai; //dès qu'on clique, on crée un nouvel objet.

	//Variables en mode Edition
	//La variable selection sert ici aussi
	//Point* edit = NULL;

    //racourcis clavier : n:noir, r:rouge, s:Selection, e:Edition, d:Dessin
    edit_affichage(mode, debut, fragments, ecran, posecran, surfaceFond, surfLigne, positionecran, droite, tailleecran);
    while (continuer==Vrai) {
        while (SDL_PollEvent(&ev)==1) {
            if (mode==Dessin)
                eventDessin(ev, &fragments, &debut, couleur, ecran->w, ecran->h, posecran);
            /*if (mode==Selection)
                eventSelection(ev);
            if (mode==Edition)
                eventEdition(ev);*/
            couleurevent(ev, &couleur, mode, debut, fragments);
            ecranevent(ev, &posecran, &tailleecran);
            changermode(ev, &mode, &debut);
            continuer = finevent(ev);
            saveevent(ev, fragments);
        }
        edit_affichage(mode, debut, fragments, ecran, posecran, surfaceFond, surfLigne, positionecran, droite, tailleecran);
		usleep(20000);
    }

//    TTF_CloseFont(police);
//    TTF_Quit();
//    SDL_Quit();
    return EXIT_SUCCESS;
}


/**
* \brief Fonction permettant d'enregistrer le terrain.
* \details En appuyant sur la touche w, cette fonction est appelé. Elle enregistre le terrain sous un format spécifique, qui pourra être lu par le jeu.
* Le terrain sera enregistré sous le nom de save.txt, dans le répertoire de l'exécutable.
* \param ev Vérifie que l'utilisatur a appuyé sur la touche w.
* \param fragments Pointeur vers le premier objet à dessiner. Cet objet contient un pointeur sur l'objet suivant, s'il existe.

*/
void saveevent(SDL_Event ev, Fragment* fragments) {
    if (ev.type == SDL_KEYDOWN)
        if (ev.key.keysym.sym==SDLK_w) {
            FILE *fichier = fopen("./save.txt", "w");
            int i;
            Couleur cols[] = {Noir, Rouge};
            for(i=0;i<2;i++) {
                Fragment* neuf = fragments;
                Point* pt = NULL;
                fprintf(fichier, "-2 -2 ");
                while (neuf!=NULL) {
                    if (neuf->couleur==cols[i]) {
                        pt = neuf->chaine;
                        fprintf(fichier, "-1 -1 ");
                        while (pt!=NULL) {
                            fprintf(fichier, "%d %d ", pt->x, pt->y);
                            pt = pt->next;
                        }
                    }
                    neuf = neuf->next;
                }
            }
            fprintf(fichier, "-3 -3");
            fclose(fichier);
        }
}

/**
* \brief Permet de changer de mode
* \details Passe en mode dessin losrque l'utilisateur appuie sur la touche D,
* en mode Selection lorsqu'il appuie sur la touche s,
* et en mode Edition lorsqu'il appuie sur la touche e.
* Pour l'instant, seul le mode Dessin a été développé.
* \param ev Permet de vérifier sur quelle touche a appuyé l'utilisateur.
* \param mode Variable déterminant le mode courant.
* \param debut Mets fin à la variable debut lorsqu'on quitte le mode dessin.

*/

void changermode(SDL_Event ev, Mode *mode, Bool *debut) {
    if (ev.type==SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_s) {
            *mode = Selection;
            *debut = Vrai;
        }
        if (ev.key.keysym.sym == SDLK_d)
            *mode = Dessin;
        if (ev.key.keysym.sym == SDLK_e) {
            *mode = Edition;
            *debut = Vrai;
        }
    }
}

/**
* \brief Permet de quitter l'éditeur.
* \details Quitte l'éditeur lorsque l'utilisateu clique sur la croix ou appuie sur Echap.
* \param ev Vérifie que l'utilisateur a demandé à quitter l'éditeur.

*/

Bool finevent(SDL_Event ev) {
    if (ev.type==SDL_KEYDOWN)
        if (ev.key.keysym.sym==SDLK_ESCAPE)
            return Faux;
    if (ev.type==SDL_QUIT)
        return Faux;
    return Vrai;
}

/**
* \brief Permet de modifier la position de l'écran.
* \details Modifie la position de l'écran en appuyant sur les flèches gauche et droite. Empêche d'atteindre une coordonnée négative ou de dépasser la taille de l'écran.
* \param ev Vérifie les actions de l'utilisateur pour les interpréter.
* \param posecran Position de l'écran.
* \param tailleecran Taille totale de l'écran.
*/

void ecranevent(SDL_Event ev, int *posecran, SDL_Rect* tailleecran) {
    if (ev.type==SDL_KEYDOWN) {
        if (ev.key.keysym.sym == SDLK_RIGHT)
            (*posecran)+=20;
        if (ev.key.keysym.sym == SDLK_LEFT && (*posecran)>0)
            (*posecran)-=20;
    }
    tailleecran->x = (*posecran);
}

/**
* \brief Permet de modifier la couleur de l'objet qu'on est en train de dessiner.
* \details Modifie la couleur de l'objet en train d'être selectionné. Le prochain objet selectionné sera dessiné de la dernière couleur selectionnée.
* Appuyer sur r dessine l'objet en rouge, appuyer sur n modifie la couleur en noir. La couleur par défaut est le noir.
* \param ev Vérifie sur quelle touche l'utilisateur a appuyé.
* \param couleur Variable définissant la couleur selectionnée.
* \param mode Mode actuel.
* \param debut Permet de déterminer si un objet est en train d'être dessiné.
* \param fragments Pointeur vers le premier objet dessné, qui permet d'obtenir l'objet en cours de dessin, s'il existe.

*/

void couleurevent(SDL_Event ev, Couleur *couleur, Mode mode, Bool debut, Fragment* fragments) {
    Fragment* neuf = NULL;
    if (ev.type==SDL_KEYDOWN) {
        if (ev.key.keysym.sym==SDLK_r)
            *couleur = Rouge;
        if (ev.key.keysym.sym==SDLK_n)
            *couleur = Noir;
    }
    if (mode==Dessin && debut==Faux) {
        neuf = fragments;
        while(neuf->next!=NULL)
            neuf = neuf->next;
        neuf->couleur = *couleur;
    }
}


/**
* \brief Permet de dessiner un objet.
* \details Permet de créer un nouvel objet et d'y rajouter de nouveaux points. Permet aussi de confirmer l'objet qui vient d'être dessiné.
* \param ev Détermine les actions de l'utilisateur.
* \param fragments Ponteur vers le premier objet.
* \param debut Détermine si un objet est déjà en train d'être dessiné, ou non.
* \param couleur Détermine la couleur de l'objet en train d'être dessin.
* \param w Largeur totale du terrain.
* \param h Hauteur totale du terrain.
* \param posecran Position de l'écran actuelle.

*/

void eventDessin(SDL_Event ev, Fragment** fragments, Bool * debut, Couleur couleur, int w, int h, int posecran) {
    Fragment* neuf = NULL;
    Point* neufchaine = NULL;
    if (ev.type == SDL_MOUSEBUTTONDOWN) {
        if (ev.button.button == SDL_BUTTON_LEFT) {
            if (*debut==Vrai) {
                *debut = Faux;
                if (*fragments==NULL) {
                    *fragments = malloc(sizeof(Fragment));
                    neuf = *fragments;
                }
                else {
                    neuf = (*fragments);
                    while (neuf->next != NULL)
                        neuf = neuf->next;
                    neuf->next = malloc(sizeof(Fragment));
                    neuf = neuf->next;
                }
                neuf->couleur = couleur;
                neuf->next = NULL;
                neuf->chaine = malloc(sizeof(Point));
                neuf->chaine->x = ev.button.x+posecran;
                neuf->chaine->y = ev.button.y;
                neuf->chaine->next = NULL;
                neuf->lench = 1;
            }
            else {
                neuf = (*fragments);
                while (neuf->next != NULL)
                    neuf = neuf->next;
                neufchaine = neuf->chaine;
                while (neufchaine->next!=NULL)
                    neufchaine = neufchaine->next;
                neufchaine->next = malloc(sizeof(Point));
                (neufchaine->next)->x = ev.button.x+posecran;
                (neufchaine->next)->y = ev.button.y;
                (neufchaine->next)->next = NULL;
                (neuf->lench)++;
            }
        }
        if (ev.button.button == SDL_BUTTON_RIGHT) {
            *debut = Vrai;
        }
    }
}


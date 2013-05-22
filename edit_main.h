extern int eEPAISSEUR_TRAIT;
extern int W_SUR_H;


typedef enum {Noir, Rouge} Couleur; /** \brief Couleur courante. */
typedef enum {Selection, Edition, Dessin} Mode; /** \brief Mode courant. */
typedef enum {Vrai, Faux} Bool;


/**
* \brief Point d'une courbe.
* \details Point d'une courbe,
* pouvant aussi bien représenter un point par lequel passe la courbe, qu'un point fictif permettant de déterminer la courbure de la courbe.
*/
typedef struct point{
    int x; /**< Abscisse du point */
    int y; /**< Ordonnée du point */
    struct point* next; /**< Pointeur vers le point suivant, initialisé à NULL. */
} Point;

/**
* \brief Objet à dessiner, constitué de plusieurs points et d'une couleur.
*/
typedef struct fragment{
    Couleur couleur; /**< Couleur de l'objet */
    Point* chaine; /**< Pointeur vers le premier point de l'objet. */
    int lench; /**< Longueur de la liste de points. */
    struct fragment* next; /**< Pointeur vers l'objet suivant, initialisé à NULL. */
} Fragment;

int edit_main();
void eventDessin(SDL_Event ev, Fragment** , Bool * debut, Couleur couleur, int w, int h, int posecran);
void couleurevent(SDL_Event ev, Couleur *couleur, Mode mode, Bool debut, Fragment* fragments);
void ecranevent(SDL_Event ev, int *posecran, SDL_Rect* tailleecran);
void changermode(SDL_Event ev, Mode *mode, Bool *debut);
Bool finevent(SDL_Event ev);
void saveevent(SDL_Event, Fragment* fragments);

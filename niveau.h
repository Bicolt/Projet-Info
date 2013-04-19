#define EPAISSEUR_TRAIT 11
#define NOMBRE_ECRANS 10
#define H_PERSO 130
#define L_PERSO 60
#define X_FIN 3400

int min(int a, int b);
int max(int a, int b);
double min_d(double a, double b);
double max_d(double a, double b);
SDL_Rect pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris);
void recollementContinu(SDL_Surface *surfLigne, SDL_Rect posSelection, SDL_Rect *posDestination);
void decouperColler(int enAppercu, SDL_Surface * surfLigne, cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination, double angle);
int niveau(SDL_Surface *ecran, int choixTerrain);
void chargerTerrain(SDL_Surface *ecran, cairo_surface_t *surfaceFond, int choixTerrain);
cairo_t * pperso(SDL_Surface *ecran, SDL_Surface * surfPerso);
void insererSurface(SDL_Surface *collage, SDL_Rect *posSelection, SDL_Surface *surfLigne, SDL_Rect *posDestination);

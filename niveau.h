extern int H_PERSO, L_PERSO, EPAISSEUR_TRAIT, X_FIN, NOMBRE_ECRANS;
int min(int a, int b);
int max(int a, int b);
double min_d(double a, double b);
double max_d(double a, double b);
double mod_d(double a, double b);
SDL_Rect pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris);
void recollementContinu(SDL_Surface *surfLigne, SDL_Rect posSelection, SDL_Rect *posDestination);
void decouperColler(int enAppercu, SDL_Surface * surfLigne, cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination, double angle, double scale);
int niveau(SDL_Surface *ecran, int choixTerrain);
void chargerTerrain(SDL_Surface *ecran, cairo_surface_t *surfaceFond, int choixTerrain);
void pperso(SDL_Surface * surfPerso, int k);
void insererSurface(SDL_Surface *collage, SDL_Rect *posSelection, SDL_Surface *surfLigne, SDL_Rect *posDestination);

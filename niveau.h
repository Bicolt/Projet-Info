#define EPAISSEUR_TRAIT 11
#define NOMBRE_ECRANS 10
#define H_PERSO 130
#define L_PERSO 60
#define X_FIN 3400

int min(int a, int b);
int max(int a, int b);
void pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris);
void decouperColler(cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination);
int niveau(SDL_Surface *ecran, int choixTerrain);
void chargerTerrain(SDL_Surface *ecran, cairo_surface_t *surfaceFond, int choixTerrain);
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface, SDL_Surface * surfPerso);
void tterrain0(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain1(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain2(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain3(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain4(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain5(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
void tterrain6(SDL_Surface *ecran, cairo_surface_t * surfaceFond);

#define EPAISSEUR_TRAIT 11
#define NOMBRE_ECRANS 10

int min(int a, int b);
int max(int a, int b);
void pointilleSelection(SDL_Surface *surfSelec, SDL_Rect selecNiveau, SDL_Rect pos, int xSouris, int ySouris);
void decouperColler(cairo_surface_t *surfaceFond, SDL_Rect posSelection, SDL_Rect posDestination);
int niveau(SDL_Surface *ecran);
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface, SDL_Surface * surfPerso);
cairo_t * tterrain(SDL_Surface *ecran, cairo_surface_t * surfaceFond);

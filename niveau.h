#define EPAISSEUR_TRAIT 11
#define NOMBRE_ECRANS 10

int niveau(SDL_Surface *ecran);
cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface);
cairo_t * tterrain(SDL_Surface *ecran, cairo_surface_t * surfaceFond);
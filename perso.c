#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include "lvl1.h"


cairo_t * pperso(SDL_Surface *ecran)
{
    surface = cairo_image_surface_create_for_data (ecran->pixels,
                                                      CAIRO_FORMAT_ARGB32,
                                                      ecran->w,
                                                      ecran->h,
                                                      ecran->pitch);
    cairo_t *perso = cairo_create(surface);
    cairo_arc(perso, 29, 19, 14, 0, 2*M_PI);
    cairo_move_to(perso, 26, 26);
    cairo_line_to(perso, 24, 71);
    cairo_set_line_width(droite,11.);

}

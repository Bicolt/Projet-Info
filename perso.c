#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <math.h>
#include "lvl1.h"


cairo_t * pperso(SDL_Surface *ecran, cairo_surface_t *surface)
{
    cairo_t *perso = cairo_create(surface);
    cairo_set_line_width(perso, EPAISSEUR_TRAIT);
    cairo_set_source_rgba (perso, 0, 0, 0, 1);
    cairo_arc(perso, 31, 19, 13, 0, 2*M_PI);
    cairo_fill_preserve(perso);
    cairo_set_line_cap(perso, CAIRO_LINE_CAP_ROUND);
    //Corps
    cairo_move_to(perso, 28, 27);
    cairo_line_to(perso, 26, 72);
    cairo_stroke_preserve(perso);
    //Bras 1
    cairo_move_to(perso, 28, 29);
    cairo_line_to(perso, 15, 43);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 14, 44);
    cairo_line_to(perso, 11, 65);
    cairo_stroke_preserve(perso);
    //Bras 2
    cairo_move_to(perso, 32, 32);
    cairo_line_to(perso, 37, 48);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 38, 49);
    cairo_line_to(perso, 52, 63);
    cairo_stroke_preserve(perso);
    //Jambe 1
    cairo_move_to(perso, 25, 74);
    cairo_line_to(perso, 17, 94);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 17, 95);
    cairo_line_to(perso, 5, 133);
    cairo_stroke_preserve(perso);
    //Jambe 2
    cairo_move_to(perso, 27, 76);
    cairo_line_to(perso, 40, 96);
    cairo_stroke_preserve(perso);
    cairo_move_to(perso, 41, 97);
    cairo_line_to(perso, 52, 128);
    cairo_stroke_preserve(perso);
    return perso;
}

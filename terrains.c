#include <stdlib.h>
#include <stdio.h>
#include <cairo.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <math.h>
#include "niveau.h"
#include "interface.h"
#include "affichage.h"
#include "deplacement.h"
#include "terrains.h"

void tterrain0(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);
    cairo_select_font_face (droite, "Sans", CAIRO_FONT_SLANT_NORMAL,
                               CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (droite, 90.0);

    cairo_move_to (droite, 10.5, 135.5);
    cairo_show_text (droite, "Hello");

    cairo_move_to(droite, 0, 500.5);
    cairo_line_to(droite, X_FIN+200.5, 500.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain1(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain2(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    cairo_stroke(droite);
    cairo_set_source_rgba (droite, 1, 0, 0, 1);
    cairo_move_to(droite, 1450.5, 500.5);
    cairo_line_to(droite, 1600.5, 600.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain3(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain4(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain5(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

void tterrain6(SDL_Surface *ecran, cairo_surface_t * surfaceFond){

    cairo_t *droite = cairo_create(surfaceFond);
    cairo_set_line_width(droite,EPAISSEUR_TRAIT);
    cairo_set_source_rgba (droite, 0, 0, 0, 1);

    cairo_move_to(droite, 0., 500.5);
    cairo_line_to(droite, 400.5, 500.5);
    cairo_curve_to(droite, 700.5, 600.5, 800.5, 50.5, 1100.5, 200.5);
    cairo_line_to(droite, 1300.5, 200.5);
    cairo_move_to(droite, 1250.5, 400.5);
    cairo_line_to(droite, 1300.5, 400.5);
    cairo_line_to(droite, 1600.5, 600.5);
    cairo_curve_to(droite, 1800.5, 300.5, 1900.5, 350.5, 2200.5, 600.5);
    cairo_curve_to(droite, 2200.5, 600.5, 2500.5, 300.5, 2800.5, 200.5);
    cairo_line_to(droite, 3000.5, 200.5);
    cairo_move_to(droite, 2900.5, 500.5);
    cairo_line_to(droite, X_FIN + 200.5, 500.5);
    cairo_move_to(droite, 2450.5, 400.5);
    cairo_line_to(droite, 2450.5, 350.5);
    //cairo_curve_to(droite, ecran->w, 400., ecran->w, 400., ecran->w, 400.);
    //cairo_line_to(droite, ecran->w-200., 550.);
    cairo_stroke(droite);
    cairo_destroy(droite);
}

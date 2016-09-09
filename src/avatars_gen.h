#ifndef _HASH_H
#define _HASH_H

#include <cairo.h>
#include <stdio.h>
#include <math.h>


typedef struct avatars_gen_rgb avatars_gen_rgb;
struct avatars_gen_rgb {
    double red;
    double green;
    double blue;
};


void generate_avatar(avatars_gen_rgb *background_color, avatars_gen_rgb *contour_color, avatars_gen_rgb *font_color, char *text, char *font);

#endif

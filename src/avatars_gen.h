#ifndef _NGX_AVATARS_GEN
#define _NGX_AVATARS_GEN

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <stdlib.h>
#include <cairo.h>
#include <stdio.h>
#include <math.h>


typedef struct avatars_gen_rgb avatars_gen_rgb;
struct avatars_gen_rgb {
    double red;
    double green;
    double blue;
};


typedef struct avatars_gen_closure avatars_gen_closure;
struct avatars_gen_closure {
    ngx_http_request_t *r;
    ngx_chain_t *first_chain;
    ngx_chain_t *curr_chain;
    size_t total_length;
};


void generate_avatar(
    avatars_gen_closure *closure, avatars_gen_rgb *background_color, avatars_gen_rgb *contour_color,
    avatars_gen_rgb *font_color, char *font, unsigned int font_size, int font_italic, int font_bold, unsigned int avatar_size, int show_contour, char *text);

#endif

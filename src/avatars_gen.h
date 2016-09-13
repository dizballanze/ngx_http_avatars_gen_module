#ifndef _NGX_AVATARS_GEN
#define _NGX_AVATARS_GEN

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
#include <stdlib.h>
#include <cairo.h>
#include <stdio.h>
#include <math.h>


#define INITIALS_MAX_SIZE 12


typedef struct avatars_gen_rgb avatars_gen_rgb;
struct avatars_gen_rgb {
    double red;
    double green;
    double blue;
};


/* Configuration structure */
typedef struct {
    avatars_gen_rgb bg_color;
    avatars_gen_rgb contour_color;
    avatars_gen_rgb font_color;
    ngx_uint_t avatar_size; // in pixels
    ngx_str_t font_face;
    ngx_uint_t font_size; // in pixels
    ngx_flag_t font_italic;
    ngx_flag_t font_bold;
    ngx_flag_t show_contour;
    ngx_flag_t square;
} ngx_http_avatars_gen_loc_conf_t;


typedef struct avatars_gen_closure avatars_gen_closure;
struct avatars_gen_closure {
    ngx_http_request_t *r;
    ngx_chain_t *first_chain;
    ngx_chain_t *curr_chain;
    size_t total_length;
};


void generate_avatar(avatars_gen_closure *closure, ngx_http_avatars_gen_loc_conf_t *conf, char *text);


int get_initials_from_uri(ngx_str_t *uri, unsigned char *initials);

#endif

#include "avatars_gen.h"
#define INITIALS_MAX_SIZE 12


/* Directives handlers declarations */
static char *ngx_http_avatars_gen_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_avatars_gen_bg_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_avatars_gen_contour_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
static char *ngx_http_avatars_gen_font_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

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

/* Allocate memory for configuration */
static void *ngx_http_avatars_gen_create_loc_conf(ngx_conf_t *cf) {
    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "create loc conf");
    ngx_http_avatars_gen_loc_conf_t *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_avatars_gen_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }
    /* Set default directives values */
    conf->bg_color.red = 0.26;
    conf->bg_color.green = 0.52;
    conf->bg_color.blue = 0.96;
    conf->contour_color.red = 0.2;
    conf->contour_color.green = 0.2;
    conf->contour_color.blue = 0.2;
    conf->font_color.red = 1.0;
    conf->font_color.green = 1.0;
    conf->font_color.blue = 1.0;
    conf->avatar_size = NGX_CONF_UNSET_UINT;
    conf->font_size = NGX_CONF_UNSET_UINT;
    conf->show_contour = NGX_CONF_UNSET;
    conf->font_italic = NGX_CONF_UNSET;
    conf->font_bold = NGX_CONF_UNSET;
    conf->square = NGX_CONF_UNSET;
    return conf;
}

/* Merge location configuration */
static char *ngx_http_avatars_gen_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child) {
    ngx_http_avatars_gen_loc_conf_t *conf = child;
    unsigned char *default_font_face;
    if (!conf->font_face.len) {
        default_font_face = ngx_palloc(cf->pool, 5);
        if (default_font_face == NULL) {
            return NGX_CONF_ERROR;
        }
        ngx_memcpy(default_font_face, "sans", 4);
        conf->font_face.len = 4;
        conf->font_face.data = default_font_face;
    }
    if (conf->avatar_size == NGX_CONF_UNSET_UINT) {
        conf->avatar_size = 100;
    }
    if (conf->font_size == NGX_CONF_UNSET_UINT) {
        conf->font_size = conf->avatar_size / 2;
    }
    if (conf->show_contour == NGX_CONF_UNSET) {
        conf->show_contour = 1;
    }
    if (conf->font_italic == NGX_CONF_UNSET) {
        conf->font_italic = 0;
    }
    if (conf->font_bold == NGX_CONF_UNSET) {
        conf->font_bold = 0;
    }
    if (conf->square == NGX_CONF_UNSET) {
        conf->square = 0;
    }
    return NGX_CONF_OK;
}


/* Module directives */
static ngx_command_t ngx_http_avatars_gen_commands[] = {
    { ngx_string("avatars_gen"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_http_avatars_gen_found_cb,
      0,
      0,
      NULL},
    { ngx_string("avatars_gen_bg_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_http_avatars_gen_bg_color_found_cb,
      NGX_HTTP_LOC_CONF_OFFSET,
      0,
      NULL},
    { ngx_string("avatars_gen_contour_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_http_avatars_gen_contour_color_found_cb,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, contour_color),
      NULL},
    { ngx_string("avatars_gen_font_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_http_avatars_gen_font_color_found_cb,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_color),
      NULL},
    { ngx_string("avatars_gen_font_face"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_face),
      NULL},
    { ngx_string("avatars_gen_font_size"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_size),
      NULL},
    { ngx_string("avatars_gen_font_italic"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_italic),
      NULL},
    { ngx_string("avatars_gen_font_bold"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_bold),
      NULL},
    { ngx_string("avatars_gen_size"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_num_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, avatar_size),
      NULL},
    { ngx_string("avatars_gen_show_contour"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, show_contour),
      NULL},
    { ngx_string("avatars_gen_square"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_flag_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, square),
      NULL},

    ngx_null_command
};


static ngx_http_module_t ngx_http_avatars_gen_module_ctx = {
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    ngx_http_avatars_gen_create_loc_conf,
    ngx_http_avatars_gen_merge_loc_conf
};


/*
 * The module which binds the context and commands
 */
ngx_module_t ngx_http_avatars_gen_module = {
    NGX_MODULE_V1,
    &ngx_http_avatars_gen_module_ctx,
    ngx_http_avatars_gen_commands,
    NGX_HTTP_MODULE,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NGX_MODULE_V1_PADDING
};

/* Fetch initials from URI and return result status */
int get_initials_from_uri(ngx_str_t *uri, unsigned char *initials) {
    unsigned char *curr_character = (uri->data + uri->len);
    unsigned int characters_copied = 0;
    while ((*curr_character != '/') && (characters_copied != INITIALS_MAX_SIZE) && (curr_character != uri->data)) {
        initials[characters_copied] = *curr_character;
        characters_copied++;
        curr_character--;
    }
    if (*curr_character != '/') {
        return 0;
    }
    /* Revert bytes */
    unsigned char tmp;
    unsigned int i;
    for (i = 0; i < (characters_copied / 2); i++) {
        tmp = initials[i];
        initials[i] = initials[characters_copied - i - 1];
        initials[characters_copied - i - 1] = tmp;
    }
    initials[characters_copied] = '\0';
    /* Validate length */
    size_t utf8_count = ngx_utf8_length(initials, characters_copied - 1);
    if ((utf8_count > 2) || (utf8_count < 1)) {
        return 0;
    }
    return 1;
}


/*
 * Main handler function of the module.
 */
static ngx_int_t ngx_http_avatars_gen_handler(ngx_http_request_t *r) {
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "HANDLER !!!");
    ngx_int_t rc;
    ngx_chain_t out;
    ngx_http_avatars_gen_loc_conf_t *loc_conf;
    avatars_gen_closure draw_closure;
    unsigned char initials[INITIALS_MAX_SIZE + 1] = "";

    loc_conf = ngx_http_get_module_loc_conf(r, ngx_http_avatars_gen_module);
    if (loc_conf->font_face.len)
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "font_face %s", loc_conf->font_face.data);

    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "URI %s", r->uri.data);
    // 1 symbol
    if (get_initials_from_uri(&r->uri, initials)) {
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "INITIALS %s", initials);
    } else {
        return NGX_HTTP_BAD_REQUEST;
    }

    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK) {
        return rc;
    }
    r->headers_out.content_type_len = sizeof("image/png") - 1;
    r->headers_out.content_type.data = (u_char *) "image/png";

    draw_closure.first_chain = &out;
    draw_closure.curr_chain = NULL;
    draw_closure.total_length = 0;
    draw_closure.r = r;
    generate_avatar(&draw_closure, &loc_conf->bg_color, &loc_conf->contour_color, &loc_conf->font_color, (char *)loc_conf->font_face.data, loc_conf->font_size, loc_conf->font_italic, loc_conf->font_bold, loc_conf->avatar_size, loc_conf->show_contour, loc_conf->square, (char *)initials);
    ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "TOTAL LENGTH %zu", draw_closure.total_length);

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = draw_closure.total_length;
    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only || (r->method == NGX_HTTP_HEAD)) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}


static char *ngx_http_avatars_gen_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen found callback");
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_avatars_gen_handler;
    return NGX_CONF_OK;
}


int is_valid_color(ngx_str_t *color) {
    if (color->len != 6) {
        return 0;
    }
    return 1;
}

/* Convert RRGGBB string to avatars_gen_rgb structure */
void str_to_rgb(ngx_str_t *str, avatars_gen_rgb *color) {
    long int number = strtol((char *)str->data, NULL, 16);
    color->red = ((number >> 16) & 0xFF) / 255.0;
    color->green = ((number >> 8) & 0xFF) / 255.0;
    color->blue = (number & 0xFF) / 255.0;
}


/* Colors directives parsing callbacks */
static char *ngx_http_avatars_gen_bg_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_bg_color found callback");
    ngx_http_avatars_gen_loc_conf_t *loc_conf = conf;
    ngx_str_t *value;
    value = cf->args->elts;
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_bg_color %s len %d", value[1].data, value[1].len);
    if (!is_valid_color(&value[1]))
        return NGX_CONF_ERROR;
    str_to_rgb(&value[1], &loc_conf->bg_color);
    return NGX_CONF_OK;
}


static char *ngx_http_avatars_gen_font_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_font_color found callback");
    ngx_http_avatars_gen_loc_conf_t *loc_conf = conf;
    ngx_str_t *value;
    value = cf->args->elts;
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_font_color %s len %d", value[1].data, value[1].len);
    if (!is_valid_color(&value[1]))
        return NGX_CONF_ERROR;
    str_to_rgb(&value[1], &loc_conf->font_color);
    return NGX_CONF_OK;
}


static char *ngx_http_avatars_gen_contour_color_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_contour_color found callback");
    ngx_http_avatars_gen_loc_conf_t *loc_conf = conf;
    ngx_str_t *value;
    value = cf->args->elts;
    ngx_conf_log_error(NGX_LOG_DEBUG, cf, 0, "avatars_gen_contour_color %s len %d", value[1].data, value[1].len);
    if (!is_valid_color(&value[1]))
        return NGX_CONF_ERROR;
    str_to_rgb(&value[1], &loc_conf->contour_color);
    return NGX_CONF_OK;
}

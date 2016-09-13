#include "avatars_gen.h"


#define COLOR_VAL_LEN 6


/* Fetch initials from URI and return result status */
int ngx_http_avatars_gen_get_initials_from_uri(ngx_str_t *uri, unsigned char *initials) {
    unsigned char *curr_character = (uri->data + uri->len - 1);
    unsigned int characters_copied = 0;
    /* Skip trailing / and ? symbols */
    while ((*curr_character == '/') || (*curr_character == '?')) {
        curr_character--;
    }
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


/* Searches `param_name` in `args` and change `rgb_color` if `param_name` was found and valid */
static void set_color(ngx_str_t *args, avatars_gen_rgb *rgb_color, char *param_name) {
    char *args_data = (char *)args->data;
    char value[COLOR_VAL_LEN] = "";
    size_t param_len = ngx_strlen(param_name);
    char *match;
    match = strcasestr((char *)args->data, param_name);
    if (match == NULL) {
        return;
    }
    // Should be on the beginning or after &-symbol
    if ((match != args_data) && (*(match - 1) != '&')) {
        return;
    }
    // Next symbol should be =
    if (((match + param_len) >= (args_data + args->len)) || (*(match + param_len) != '=')) {
        return;
    }
    // Value should have correct length
    if ((match + param_len + 1 + COLOR_VAL_LEN) <= args_data) {
        return;
    }
    // TODO: validate value content
    ngx_memcpy(value, (match + param_len + 1), COLOR_VAL_LEN);
    str_to_rgb(value, rgb_color);
}


/* Update local conf with request specific params (i.e. color settings from GET parameters) */
ngx_http_avatars_gen_loc_conf_t *
ngx_http_avatars_gen_request_conf(ngx_http_request_t *r, ngx_http_avatars_gen_loc_conf_t *loc_conf) {
    if (!r->args.len) {
        return loc_conf;
    }
    ngx_http_avatars_gen_loc_conf_t *request_conf;
    request_conf = ngx_palloc(r->pool, sizeof(ngx_http_avatars_gen_loc_conf_t));
    if (request_conf == NULL) {
        return NULL;
    }
    ngx_memcpy(request_conf, loc_conf, sizeof(ngx_http_avatars_gen_loc_conf_t));
    set_color(&r->args, &request_conf->bg_color, "bg_color");
    set_color(&r->args, &request_conf->contour_color, "contour_color");
    set_color(&r->args, &request_conf->font_color, "font_color");
    return request_conf;
}

#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>


/* Directives handlers declarations */
/*static char *ngx_http_avatars_gen(ngx_conf_t *cf, void *post, void *data);*/
/*static ngx_conf_post_handler_pt ngx_http_avatars_gen_p = ngx_http_avatars_gen;*/
/*static char *ngx_http_avatars_gen_bg_color(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);*/
static char *ngx_http_avatars_gen_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


typedef struct {
    ngx_str_t  bg_color;
    ngx_str_t  contour_color;
    ngx_str_t  font_color;
    ngx_str_t  font_face;
} ngx_http_avatars_gen_loc_conf_t;


static void *ngx_http_avatars_gen_create_loc_conf(ngx_conf_t *cf) {
    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "create loc conf");
    ngx_http_avatars_gen_loc_conf_t  *conf;
    conf = ngx_pcalloc(cf->pool, sizeof(ngx_http_avatars_gen_loc_conf_t));
    if (conf == NULL) {
        return NULL;
    }
    return conf;
}


static ngx_command_t ngx_http_avatars_gen_commands[] = {
    { ngx_string("avatars_gen"),
      NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_http_avatars_gen_found_cb,
      0,
      0,
      NULL},
    { ngx_string("avatars_gen_bg_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, bg_color),
      NULL},
    { ngx_string("avatars_gen_contour_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, contour_color),
      NULL},
    { ngx_string("avatars_gen_font_color"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_color),
      NULL},
    { ngx_string("avatars_gen_font_face"),
      NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
      ngx_conf_set_str_slot,
      NGX_HTTP_LOC_CONF_OFFSET,
      offsetof(ngx_http_avatars_gen_loc_conf_t, font_face),
      NULL},

    ngx_null_command
};


static ngx_http_module_t ngx_http_avatars_gen_module_ctx = {
    NULL,                          /* preconfiguration */
    NULL,                          /* postconfiguration */

    NULL,                          /* create main configuration */
    NULL,                          /* init main configuration */

    NULL,                          /* create server configuration */
    NULL,                          /* merge server configuration */

    ngx_http_avatars_gen_create_loc_conf, /* create location configuration */
    NULL                           /* merge location configuration */
};


/*
 * The module which binds the context and commands
 *
 */
ngx_module_t ngx_http_avatars_gen_module = {
    NGX_MODULE_V1,
    &ngx_http_avatars_gen_module_ctx,    /* module context */
    ngx_http_avatars_gen_commands,       /* module directives */
    NGX_HTTP_MODULE,               /* module type */
    NULL,                          /* init master */
    NULL,                          /* init module */
    NULL,                          /* init process */
    NULL,                          /* init thread */
    NULL,                          /* exit thread */
    NULL,                          /* exit process */
    NULL,                          /* exit master */
    NGX_MODULE_V1_PADDING
};

/*
 * Main handler function of the module.
 */
static ngx_int_t ngx_http_avatars_gen_handler(ngx_http_request_t *r) {
    ngx_log_debug0(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "HANDLER !!!");
    ngx_int_t    rc;
    ngx_chain_t  out;
    ngx_http_avatars_gen_loc_conf_t *loc_conf;

    loc_conf = ngx_http_get_module_loc_conf(r, ngx_http_avatars_gen_module);
    if (loc_conf->bg_color.len)
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "bg_color %s", loc_conf->bg_color.data);
    if (loc_conf->contour_color.len)
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "contour_color %s", loc_conf->contour_color.data);
    if (loc_conf->font_color.len)
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "font_color %s", loc_conf->font_color.data);
    if (loc_conf->font_face.len)
        ngx_log_debug1(NGX_LOG_DEBUG_HTTP, r->connection->log, 0, "font_face %s", loc_conf->font_face.data);

    if (!(r->method & (NGX_HTTP_GET|NGX_HTTP_HEAD))) {
        return NGX_HTTP_NOT_ALLOWED;
    }
    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK) {
        return rc;
    }
    r->headers_out.content_type_len = sizeof("text/html") - 1;
    r->headers_out.content_type.data = (u_char *) "text/html";
    if (r->method == NGX_HTTP_HEAD) {
        r->headers_out.status = NGX_HTTP_OK;
        r->headers_out.content_length_n = 0;

        return ngx_http_send_header(r);
    }

    out.next = NULL;

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = 0;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}

/*
 * Function for the directive avatars_gen and sets up
 * http handler.
static char *ngx_http_avatars_gen(ngx_conf_t *cf, void *post, void *data) {
    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "avatars_gen post handler");
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_avatars_gen_handler;
    return NGX_CONF_OK;
}
 */

static char *ngx_http_avatars_gen_found_cb(ngx_conf_t *cf, ngx_command_t *cmd, void *conf) {
    ngx_conf_log_error(NGX_LOG_INFO, cf, 0, "avatars_gen post handler");
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_http_avatars_gen_handler;
    return NGX_CONF_OK;
}

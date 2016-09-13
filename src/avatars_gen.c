#include "avatars_gen.h"


/* Create buffer chains filled with avatar data */
cairo_status_t write_func(void *closure, const unsigned char *data, unsigned int length) {
    avatars_gen_closure *custom_closure = closure;
    ngx_chain_t *new_curr_chain;
    ngx_buf_t *b;
    unsigned char *buffer;
    int is_first = (custom_closure->curr_chain == NULL);
    /* Get chain item */
    if (is_first) {
        new_curr_chain = custom_closure->first_chain;
    } else {
        new_curr_chain = ngx_palloc(custom_closure->r->pool, sizeof(ngx_chain_t));
        if (new_curr_chain == NULL) {
            return CAIRO_STATUS_WRITE_ERROR;
        }
    }
    /* Allocate buffer */
    buffer = ngx_palloc(custom_closure->r->pool, length);
    if (buffer == NULL) {
        return CAIRO_STATUS_WRITE_ERROR;
    }
    ngx_memcpy(buffer, data, length);
    /* Allocate buffer container */
    b = ngx_pcalloc(custom_closure->r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return CAIRO_STATUS_WRITE_ERROR;
    }
    b->pos = buffer;
    b->last = buffer + length;
    b->memory = 1;
    b->last_buf = 1;
    /* Set buffer to chain item */
    new_curr_chain->buf = b;
    new_curr_chain->next = NULL;
    /* Process chain links */
    if (!is_first) {
        custom_closure->curr_chain->next = new_curr_chain;
        custom_closure->curr_chain->buf->last_buf = 0;
    }
    custom_closure->curr_chain = new_curr_chain;
    /* Accumulate total image size */
    custom_closure->total_length += length;
    return CAIRO_STATUS_SUCCESS;
}


/* Draw avatar by provided options */
void ngx_http_avatars_gen_generate_avatar(avatars_gen_closure *closure, ngx_http_avatars_gen_loc_conf_t *conf, char *text) {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, conf->avatar_size, conf->avatar_size);
    cairo_t *cr = cairo_create(surface);
    /* Draw circle and fill background */
    double contour_offset = 0;
    if (conf->show_contour) {
        contour_offset = 1.25;
    }
    if (conf->square) {
        cairo_rectangle(cr, contour_offset, contour_offset,
            conf->avatar_size - 2 * contour_offset, conf->avatar_size - 2 * contour_offset);
    } else {
        double radius = conf->avatar_size / 2.0;
        cairo_arc(cr, radius, radius, radius - contour_offset, 0, 2 * M_PI);
    }
    cairo_set_source_rgb(cr, conf->bg_color.red, conf->bg_color.green, conf->bg_color.blue);
    cairo_fill_preserve(cr);
    if (conf->show_contour) {
        cairo_set_line_width(cr, 2.5);
        cairo_set_source_rgb(cr, conf->contour_color.red, conf->contour_color.green, conf->contour_color.blue);
        cairo_stroke(cr);
    }
    /* Draw text */
    cairo_text_extents_t extents;
    int font_slant = CAIRO_FONT_SLANT_NORMAL;
    int font_weight = CAIRO_FONT_WEIGHT_NORMAL;
    if (conf->font_italic) {
        font_slant = CAIRO_FONT_SLANT_ITALIC;
    }
    if (conf->font_bold) {
        font_weight = CAIRO_FONT_WEIGHT_BOLD;
    }
    cairo_select_font_face(cr, (char *)conf->font_face.data, font_slant, font_weight);
    cairo_set_font_size(cr, conf->font_size);
    cairo_text_extents(cr, text, &extents);
    double x = (conf->avatar_size / 2.0) - (extents.width/2 + extents.x_bearing);
    double y = (conf->avatar_size / 2.0) - (extents.height/2 + extents.y_bearing);
    cairo_move_to(cr, x, y);
    cairo_set_source_rgb(cr, conf->font_color.red, conf->font_color.green, conf->font_color.blue);
    cairo_show_text(cr, text);
    /* Save and destroy */
    cairo_destroy (cr);
    cairo_surface_write_to_png_stream(surface, write_func, closure);
    cairo_surface_destroy (surface);
}

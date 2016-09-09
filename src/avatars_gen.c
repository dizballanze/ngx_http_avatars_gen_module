#include "avatars_gen.h"


cairo_status_t write_func(void *closure, const unsigned char *data, unsigned int length) {
    avatars_gen_closure *custom_closure = closure;
    ngx_chain_t *new_curr_chain;
    ngx_buf_t *b;
    unsigned char *buffer;
    int is_first = (custom_closure->curr_chain == NULL);
    if (is_first) {
        new_curr_chain = custom_closure->first_chain;
    } else {
        new_curr_chain = ngx_palloc(custom_closure->r->pool, sizeof(ngx_chain_t));
        if (new_curr_chain == NULL) {
            return CAIRO_STATUS_WRITE_ERROR;
        }
    }
    buffer = ngx_palloc(custom_closure->r->pool, length);
    if (buffer == NULL) {
        return CAIRO_STATUS_WRITE_ERROR;
    }
    ngx_memcpy(buffer, data, length);

    b = ngx_pcalloc(custom_closure->r->pool, sizeof(ngx_buf_t));
    if (b == NULL) {
        return CAIRO_STATUS_WRITE_ERROR;
    }

    b->pos = buffer;
    b->last = buffer + length;
    b->memory = 1;
    b->last_buf = 1;

    new_curr_chain->buf = b;
    new_curr_chain->next = NULL;

    if (!is_first) {
        custom_closure->curr_chain->next = new_curr_chain;
        custom_closure->curr_chain->buf->last_buf = 0;
    }
    custom_closure->curr_chain = new_curr_chain;

    custom_closure->total_length += length;
    return CAIRO_STATUS_SUCCESS;
}

void generate_avatar(avatars_gen_closure *closure, avatars_gen_rgb *background_color, avatars_gen_rgb *contour_color, avatars_gen_rgb *font_color, char *font, char *text) {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100, 100);
    cairo_t *cr = cairo_create(surface);
    /* Draw circle and fill background */
    cairo_arc(cr, 50.0, 50.0, 50.0 - 1.25, 0, 2 * M_PI);
    cairo_set_source_rgb(cr, background_color->red, background_color->green, background_color->blue);
    cairo_fill_preserve(cr);
    cairo_set_line_width(cr, 2.5);
    cairo_set_source_rgb(cr, contour_color->red, contour_color->green, contour_color->blue);
    cairo_stroke(cr);
    /* Draw text */
    cairo_text_extents_t extents;
    cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 50.0);
    cairo_text_extents(cr, text, &extents);
    double x = 50.0 - (extents.width/2 + extents.x_bearing);
    double y = 50.0 - (extents.height/2 + extents.y_bearing);
    cairo_move_to(cr, x, y);
    cairo_set_source_rgb(cr, font_color->red, font_color->green, font_color->blue);
    cairo_show_text(cr, text);
    /* Save and destroy */
    cairo_destroy (cr);
    cairo_surface_write_to_png_stream(surface, write_func, closure);
    cairo_surface_destroy (surface);
}

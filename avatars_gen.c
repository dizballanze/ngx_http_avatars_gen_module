#include "avatars_gen.h"


cairo_status_t write_func(void *closure, const unsigned char *data, unsigned int length) {
    FILE *fp = fopen("ava.png", "a");
    if (!fp)
        return CAIRO_STATUS_WRITE_ERROR;
    size_t written = fwrite(data, 1, length, fp);
    fclose(fp);
    if (written == length)
        return CAIRO_STATUS_SUCCESS;
    else
        return CAIRO_STATUS_WRITE_ERROR;
}


void generate_avatar(avatars_gen_rgb *background_color, avatars_gen_rgb *contour_color, avatars_gen_rgb *font_color, char *text, char *font) {
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100, 100);
    cairo_t *cr = cairo_create(surface);
    /* Draw circle and fill background */
    cairo_arc(cr, 50.0, 50.0, 50.0 - 1.25, 0, 2 * M_PI);
    cairo_set_source_avatars_gen_rgb(cr, background_color->red, background_color->green, background_color->blue);
    cairo_fill_preserve(cr);
    cairo_set_line_width(cr, 2.5);
    cairo_set_source_avatars_gen_rgb(cr, contour_color->red, contour_color->green, contour_color->blue);
    cairo_stroke(cr);
    /* Draw text */
    cairo_text_extents_t extents;
    cairo_select_font_face(cr, font, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 60.0);
    cairo_text_extents(cr, text, &extents);
    double x = 50.0 - (extents.width/2 + extents.x_bearing);
    double y = 50.0 - (extents.height/2 + extents.y_bearing);
    cairo_move_to(cr, x, y);
    cairo_set_source_avatars_gen_rgb(cr, font_color->red, font_color->green, font_color->blue);
    cairo_show_text(cr, text);
    /* Save and destroy */
    cairo_destroy (cr);
    cairo_surface_write_to_png_stream(surface, write_func, NULL);
    cairo_surface_destroy (surface);
}


int main() {
    avatars_gen_rgb bg_color, contour_color, font_color;
    contour_color.red = 0.5;
    contour_color.green = 0.5;
    contour_color.blue = 0.5;
    bg_color.red = 0.9;
    bg_color.green = 0.9;
    bg_color.blue = 0.9;
    font_color.red = 0.4;
    font_color.green = 0.4;
    font_color.blue = 0.4;
    generate_avatar(&bg_color, &contour_color, &font_color, "JD", "monospace");
    return 0;
}

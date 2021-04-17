/*
    The file defines the graphic algorithms that are been utilized in this app.
*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "variables.h"

void sivop(GdkPixbuf *);
GdkPixbuf *rivop();
struct Color *get_pixel(gint, gint);
void draw_pixel(gint, gint, struct Color *, gint);
gint region_code(gint, gint);
gboolean cohen_sutherland_clip(struct Point *, struct Point *);
void flood_fill(gint, gint, struct Color *, gint);
void bresenham_line(struct Point, struct Point, struct Color *, gint);
void bezier_curve(struct Point, struct Point, struct Point, struct Color *, gint);
void bresenham_circle(struct Point, struct Point, struct Color *, gint);

#endif

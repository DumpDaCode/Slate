/*
* The forward reference of all the algorithm that are used for
* the drawing line, circle, flood fill, clipping and curve.
*/
#ifndef ALGO_H
#define ALGO_H

#include "variable.h"
#include "data_structures.h"
#include <math.h>

void bresenham_line(GtkWidget *widget, cairo_t *cr);
gint region_code(gint x, gint y);
gboolean cohen_sutherland_clip();
void bezier_curve(GtkWidget *widget, cairo_t *cr);
void bresenham_circle(GtkWidget *widget, cairo_t *cr);
gboolean check_pixel(gint x, gint y);
void flood_fill(GtkWidget *widget, cairo_t *cr, gint x, gint y);

#endif
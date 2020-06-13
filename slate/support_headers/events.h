/*
* This file contains the forward reference of all the events that are connected
* to all the buttons, widgets and the custom widgets.
*/
#ifndef EVENT_H
#define EVENT_H

void init_color(gint index, gint red, gint green, gint blue);
void set_color(cairo_t *cr, gint color);
void draw_pencil(GtkWidget *widget);
void draw_line(GtkWidget *widget);
void draw_curve(GtkWidget *widget);
void draw_circle(GtkWidget *widget);
void erase(GtkWidget *widget);
gboolean set(GtkWidget *widget, gpointer data);
gboolean redraw(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean on_motion(GtkWidget* widget, GdkEvent *type, gpointer data);
gboolean on_click(GtkWidget* widget, GdkEvent *type, gpointer data);
void destroy(GtkWidget* widget, gpointer data);
gboolean set_tip(GtkWidget *widget, gpointer data);
gboolean draw_tip(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean update_size(GtkWidget* widget, GdkEvent *type, gpointer data);
gboolean update_area(GtkComboBox* widget, gpointer data);
gboolean set_current_tool_pencil(GtkButton *button, gpointer data);
gboolean set_current_tool_line(GtkButton *button, gpointer data);
gboolean set_current_tool_curve(GtkButton *button, gpointer data);
gboolean set_current_tool_circle(GtkButton *button, gpointer data);
gboolean set_current_tool_fill(GtkButton *button, GdkEvent *type, gpointer data);
gboolean set_fill_bg(GtkWidget *widget, gpointer data);
gboolean draw_fill_bg(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean set_current_tool_eraser(GtkButton *button, GdkEvent *type, gpointer data);
gboolean set_eraser_bg(GtkWidget *widget, gpointer data);
gboolean draw_eraser_bg(GtkWidget *widget, cairo_t *cr, gpointer data);
gboolean set_color_white(GtkWidget *widget, gpointer data);
gboolean set_color_yellow(GtkWidget *widget, gpointer data);
gboolean set_color_orange(GtkWidget *widget, gpointer data);
gboolean set_color_red(GtkWidget *widget, gpointer data);
gboolean set_color_green(GtkWidget *widget, gpointer data);
gboolean set_color_blue(GtkWidget *widget, gpointer data);
gboolean set_color_purple(GtkWidget *widget, gpointer data);
gboolean set_color_brown(GtkWidget *widget, gpointer data);
gboolean set_color_black(GtkWidget *widget, gpointer data);

#endif
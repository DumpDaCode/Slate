/*
    All event signal handler are declared over here.
*/

#ifndef EVENTS_H
#define EVENTS_H

#include <gtk/gtk.h>

//  Panel events.
gboolean set_current_tool_pencil(GtkButton *, gpointer );
gboolean set_current_tool_line(GtkButton *, gpointer );
gboolean set_current_tool_curve(GtkButton *, gpointer );
gboolean set_current_tool_circle(GtkButton *, gpointer );
gboolean set_current_tool_fill(GtkButton *, gpointer );
gboolean set_current_tool_eraser(GtkButton *, gpointer );
gboolean set_tip(GtkWidget *, gpointer);
gboolean draw_tip(GtkWidget *, cairo_t *, gpointer);
gboolean update_size(GtkWidget *, GdkEvent *, gpointer);
gboolean set_window_size(GtkComboBoxText *, gpointer );
gboolean set_foreground(GtkColorButton *, gpointer );
gboolean set_background(GtkColorButton *, gpointer );

//  Sheet events.
gboolean init_sheet(GtkWidget *, gpointer);
gboolean draw_sheet(GtkWidget *, cairo_t *, gpointer);
gboolean on_click(GtkWidget *, GdkEvent *, gpointer);
gboolean on_motion(GtkWidget *, GdkEvent *, gpointer);
gboolean on_release(GtkWidget *, GdkEvent *, gpointer);
gboolean on_key_press(GtkWidget *, GdkEvent *, gpointer);

#endif

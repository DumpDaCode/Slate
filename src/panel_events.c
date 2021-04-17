/*
	Events that are attached to tool panel are defined over here.
*/
#include "events.h"
#include "variables.h"

gint current_tool=0;
gint size=1;
static cairo_surface_t *size_surface;

/*
	The below group of six event handler set the current tool selected by the
	user. They also update the cursor for sheet.
*/
gboolean set_current_tool_pencil(GtkButton *button, gpointer data){
	current_tool = 0;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
	gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_PENCIL));
	return TRUE;
}

gboolean set_current_tool_line(GtkButton *button, gpointer data){
	current_tool = 1;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_CROSSHAIR));
	return TRUE;
}

gboolean set_current_tool_curve(GtkButton *button, gpointer data){
	current_tool = 2;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_CROSSHAIR));
	return TRUE;
}

gboolean set_current_tool_circle(GtkButton *button, gpointer data){
	current_tool = 3;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_CROSSHAIR));
	return TRUE;
}

gboolean set_current_tool_fill(GtkButton *button, gpointer data){
	current_tool = 4;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_SPRAYCAN));
	return TRUE;
}

gboolean set_current_tool_eraser(GtkButton *button, gpointer data){
	current_tool = 5;
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gdk_window_set_cursor(gtk_widget_get_window(GTK_WIDGET(sheet)), \
        gdk_cursor_new_for_display(gdk_display_get_default(), GDK_DOT));
	return TRUE;
}

/*
The below 3 functions are attached to tip size widget which is a customized
widget.
*/
gboolean set_tip(GtkWidget *widget, gpointer data){
//	Setting up the widget.
    if(size_surface)
		cairo_surface_destroy(size_surface);
	size_surface = gdk_window_create_similar_surface(gtk_widget_get_window (widget),
    	CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget),
    	gtk_widget_get_allocated_height (widget));
	cairo_t *cr;
  	cr = cairo_create(size_surface);
  	cairo_set_source_rgb(cr, 0,0,0);
 	cairo_paint (cr);
  	cairo_destroy (cr);
	return TRUE;
}

gboolean draw_tip(GtkWidget *widget, cairo_t *cr, gpointer data){
//	Drawing the widget.
	cairo_set_source_rgb(cr, 0,0,0);
	cairo_set_line_width(cr, (double)size);
	cairo_move_to(cr, 20, 15);
	cairo_line_to(cr, 90, 15);
	cairo_stroke(cr);
	gtk_widget_queue_draw(widget);
	return TRUE;
}

gboolean update_size(GtkWidget* widget, GdkEvent *type, gpointer data){
//	Updating the tip size hen user presses it.
	GdkEventButton *button = (GdkEventButton*)type;
	if(button->type == GDK_BUTTON_PRESS && button->button == 1){
		size = (size + 1)%4;
		if(size == 0)
			size = 1;
	}
	if(button->type == GDK_BUTTON_PRESS && button->button == 3){
		size = (size - 1)%4;
		if(size == 0)
			size = 3;
	}
	gtk_widget_queue_draw(widget);
	return TRUE;
}

gboolean set_window_size(GtkComboBoxText *cbox, gpointer data){
/*
	This function updates the window size of the sheet.
*/
    gint id = gtk_combo_box_get_active(GTK_COMBO_BOX(cbox));
    switch(id){
        case 0:
			drawing_sheet.width = 400;
			drawing_sheet.height = 500;
			break;
		case 1:
			drawing_sheet.width = 600;
			drawing_sheet.height = 500;
			break;
		case 2:
			drawing_sheet.width = 800;
			drawing_sheet.height = 600;
			break;
		case 3:
			drawing_sheet.width = 1000;
			drawing_sheet.height = 600;
			break;
    }
    GObject *sheet = gtk_builder_get_object(GTK_BUILDER(data), "sheet");
    gtk_widget_set_size_request(GTK_WIDGET(sheet), drawing_sheet.width, drawing_sheet.height);
    init_sheet(GTK_WIDGET(sheet), NULL);
	return TRUE;
}

/*
	The below two functions are for setting up foreground and background
	variables.
*/
gboolean set_foreground(GtkColorButton *button, gpointer data){
    GdkRGBA rgba;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &rgba);
    foreground.red = (gint)(rgba.red*255);
    foreground.green = (gint)(rgba.green*255);
    foreground.blue = (gint)(rgba.blue*255);
	return TRUE;
}

gboolean set_background(GtkColorButton *button, gpointer data){
    GdkRGBA rgba;
    gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &rgba);
    background.red = (gint)(rgba.red*255);
    background.green = (gint)(rgba.green*255);
    background.blue = (gint)(rgba.blue*255);
	return TRUE;
}

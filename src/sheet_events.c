/*
	Events that are attached to sheet or which affect it.
*/

#include "events.h"
#include "variables.h"
#include "app_ds.h"
#include "graphics.h"

static cairo_surface_t *surface;
static GdkPixbuf *buffer_surface, *sheet_buffer, *redo;
struct Stack *undo;

struct Point start, control, end, seed;
static gint curve_points_counter = 0 , start_registering = 0;

gboolean init_sheet(GtkWidget *widget, gpointer data){
/*
	This function initiates the sheet's pixel buffer.
*/
	if(surface)
		cairo_surface_destroy(surface);
    surface = gdk_window_create_similar_image_surface (gtk_widget_get_window (widget), \
    	CAIRO_FORMAT_RGB24, drawing_sheet.width, \
    	drawing_sheet.height, 1);
    cairo_t *cr;
  	cr = cairo_create(surface);
    gdouble red = (double)background.red/255;
    gdouble green = (double)background.green/255;
    gdouble blue = (double)background.blue/255;
  	cairo_set_source_rgb(cr, red, green, blue);
 	cairo_paint(cr);
  	cairo_destroy(cr);
	sheet_buffer = gdk_pixbuf_get_from_surface(surface, 0, 0, drawing_sheet.width, drawing_sheet.height);
	return TRUE;
}

gboolean draw_sheet(GtkWidget *widget, cairo_t *cr, gpointer data){
/*
	This function handles the initial drawing and redrawing of the sheet.
*/
	gdk_cairo_set_source_pixbuf(cr, sheet_buffer, 0, 0);
  	cairo_paint (cr);
  	gtk_widget_queue_resize(widget);
	return TRUE;
}

gboolean on_motion(GtkWidget* widget, GdkEvent *type, gpointer data){
/*
	Based on the current tool selected appropriate operations are performed
	on the sheet when the mouse cursor is in motion.
*/
	GdkEventMotion *motion = (GdkEventMotion*)type;
	sivop(sheet_buffer);
	if(start_registering == 1){
		switch(current_tool){
			case 0:
				end.x = motion->x;
				end.y = motion->y;
				bresenham_line(start, end, &foreground, (int)(size*2-1)/2);
				start.x = end.x;
				start.y = end.y;
				break;
			case 1:
				end.x = motion->x;
				end.y = motion->y;
				sheet_buffer = gdk_pixbuf_copy(buffer_surface);
				sivop(sheet_buffer);
				bresenham_line(start, end, &foreground, (int)(size*2-1)/2);
				break;
			case 2:
				switch(curve_points_counter) {
					case 0:
						end.x = motion->x;
						end.y = motion->y;
						sheet_buffer = gdk_pixbuf_copy(buffer_surface);
						sivop(sheet_buffer);
						bresenham_line(start, end, &foreground, (int)(size*2-1)/2);
						break;
					case 1:
						sheet_buffer = gdk_pixbuf_copy(buffer_surface);
						sivop(sheet_buffer);
						bezier_curve(start, end, control, &foreground, (int)(size*2-1)/2);
						control.x = motion->x;
						control.y = motion->y;
						break;
				}
				break;
			case 3:
				end.x = motion->x;
				end.y = motion->y;
				sheet_buffer = gdk_pixbuf_copy(buffer_surface);
				sivop(sheet_buffer);
				bresenham_circle(start, end, &foreground, (int)(size*2-1)/2);
				break;
			case 5:
				end.x = motion->x;
				end.y = motion->y;
				bresenham_line(start, end, &background, (int)(size*2-1)/2);
				start.x = end.x;
				start.y = end.y;
				break;
		}
	}
	return TRUE;
}

gboolean on_click(GtkWidget* widget, GdkEvent *type, gpointer data){
/*
	Based on the current tool selected appropriate operations are performed
	on the sheet when the mouse button is pressed on the sheet.
*/
	GdkEventButton *button = (GdkEventButton*)type;
	start_registering = 0;
	sivop(sheet_buffer);
	if(button->type == GDK_BUTTON_PRESS && button->button == 1){
		start_registering = 1;
		undo = push_stack(gdk_pixbuf_copy(sheet_buffer), undo);
		switch(current_tool){
			case 0:
				start.x = button->x;
				start.y = button->y;
				end.x = start.x;
				end.y = start.y;
				draw_pixel(end.x, end.y, &foreground, (int)(size*2-1)/2);
				break;
			case 1:
				start.x = button->x;
				start.y = button->y;
				buffer_surface = gdk_pixbuf_copy(sheet_buffer);
				break;
			case 2:
				switch(curve_points_counter){
					case 0:
						start.x = button->x;
						start.y = button->y;
						buffer_surface = gdk_pixbuf_copy(sheet_buffer);
						break;
					case 1:
						pop_stack(&undo);
						control.x = button->x;
						control.y = button->y;
						break;
				}
				break;
			case 3:
				start.x = button->x;
				start.y = button->y;
				buffer_surface = gdk_pixbuf_copy(sheet_buffer);
				break;
			case 4:
				seed.x = button->x;
				seed.y = button->y;
				flood_fill(seed.x, seed.y, &foreground, 0);
				start_registering = 0;
				break;
			case 5:
				start.x = button->x;
				start.y = button->y;
				end.x = start.x;
				end.y = start.y;
				draw_pixel(end.x, end.y, &background, (int)(size*2-1)/2);
		}
	}
	if(button->type == GDK_BUTTON_PRESS && button->button == 3){
		if(current_tool == 5)
			init_sheet(widget,NULL);
		else{
			curve_points_counter = 0;
			if(!is_empty_stack(undo)){
				redo = gdk_pixbuf_copy(sheet_buffer);
				sheet_buffer = gdk_pixbuf_copy(pop_stack(&undo));
			}
		}
	}
	return TRUE;
}

gboolean on_release(GtkWidget* widget, GdkEvent *type, gpointer data){
/*
	Based on the current tool selected appropriate operations are performed
	on the sheet when the mouse button is released.
*/
	GdkEventButton *button = (GdkEventButton*)type;
	if(button->type == GDK_BUTTON_RELEASE && button->button == 1){
		if(current_tool == 2){
			curve_points_counter = (curve_points_counter+1)%2;
		}
		else{
			curve_points_counter = 0;
		}
	}
	return TRUE;
}

gboolean on_key_press(GtkWidget* widget, GdkEvent *type, gpointer data){
/*
	This event handler is for ctrl+w, ctrl+y, ctrl+z operatins on the window.
	It is defined over here as it affects the sheet.
*/
	GdkEventKey *key = (GdkEventKey*)type;
	if(key->state & GDK_CONTROL_MASK){
		switch (key->keyval) {
			case 'w':
				g_application_quit(G_APPLICATION(data));
				break;
			case 'z':
				if(!is_empty_stack(undo)){
					redo = gdk_pixbuf_copy(sheet_buffer);
					sheet_buffer = gdk_pixbuf_copy(pop_stack(&undo));
				}
				break;
			case 'y':
				undo = push_stack(gdk_pixbuf_copy(sheet_buffer), undo);
				sheet_buffer = gdk_pixbuf_copy(redo);
				break;
		}
	}
	return TRUE;
}

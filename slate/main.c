/*
* Slate is a simple paint program which is built using GTK.
* Gtk provides the windowing enviroment and all the butttons and 
* the widgets that are needed for our program. 
* There are two data structures and five algorithms that have being used
* for developing this program.
*
* Attempted By: Rajiv Singh
* Last Modified: 10-06-2020
*/

#include "support_headers/data_structures.h"
#include "support_headers/algo.h"
#include "support_headers/events.h"

int main(int argc, char* argv[]) 
{ 
// Initialization stage
	init_color(0, 255, 255, 255);
	init_color(1, 255, 255, 0);
	init_color(2, 255, 164, 0);
	init_color(3, 255, 0, 0);
	init_color(4, 0, 128, 0);
	init_color(5, 0, 0, 255);
	init_color(6, 159, 32, 239);
	init_color(7, 150, 75, 0);
	init_color(8, 0, 0, 0);
	drawing_sheet.width = 400, drawing_sheet.height = 500;
	gtk_init(&argc, &argv);

// Main window
	GtkWidget *window; 
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 15); 
	gtk_window_set_title(GTK_WINDOW(window), "Slate");
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

// Cursors
	pencil_cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_PENCIL);
	paint_cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_SPRAYCAN);
	eraser_cursor = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_DOT);
	crosshair = gdk_cursor_new_for_display(gdk_display_get_default(), GDK_CROSSHAIR);

// Attaching a grid to main window. This container is divided into two parts panel and sheet.
	GtkWidget *container;
	container = gtk_grid_new();

// The panel. It contains all the functonalities that slate has to offer.
	GtkWidget *panel = gtk_grid_new();
	gtk_widget_set_size_request(GTK_WIDGET(panel), 110, 480);

// The tools part of panel. It contains 8 tools provided by Slate
	GtkWidget *tool = gtk_label_new("Tools");
	GtkWidget *pencil = gtk_button_new();
	gtk_widget_set_tooltip_text(pencil, "Pencil Tool");
	buf = gdk_pixbuf_new_from_file_at_scale("icons/pencil.png", 25, 25, TRUE, NULL);
	GtkWidget *pencil_image = gtk_image_new_from_pixbuf(buf); 
	gtk_button_set_image(GTK_BUTTON(pencil), pencil_image);
	GtkWidget *line = gtk_button_new();
	gtk_widget_set_tooltip_text(line, "Line Tool");
	buf = gdk_pixbuf_new_from_file_at_scale("icons/line.png", 25, 25, TRUE, NULL);
	GtkWidget *line_image = gtk_image_new_from_pixbuf(buf); 
	gtk_button_set_image(GTK_BUTTON(line), line_image);
	GtkWidget *curve = gtk_button_new();
	gtk_widget_set_tooltip_text(curve, "Curve Tool");
	buf = gdk_pixbuf_new_from_file_at_scale("icons/curve.png", 25, 25, TRUE, NULL);
	GtkWidget *curve_image = gtk_image_new_from_pixbuf(buf); 
	gtk_button_set_image(GTK_BUTTON(curve), curve_image);
	GtkWidget *circle = gtk_button_new();
	gtk_widget_set_tooltip_text(circle, "Circle Tool");
	buf = gdk_pixbuf_new_from_file_at_scale("icons/circle.png", 25, 25, TRUE, NULL);
	GtkWidget *circle_image = gtk_image_new_from_pixbuf(buf); 
	gtk_button_set_image(GTK_BUTTON(circle), circle_image);

// The fill tool
	GtkWidget *fill = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *background_fill = gtk_drawing_area_new();
	gtk_widget_set_size_request(background_fill, 35, 35);
	gtk_box_pack_start(GTK_BOX(fill), background_fill, FALSE, FALSE, 0);
	gtk_widget_set_events(background_fill, GDK_BUTTON_PRESS_MASK);	
	gtk_widget_set_tooltip_text(fill, "Fill Tool");

// The eraser tool
	GtkWidget *eraser= gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	GtkWidget *background_eraser = gtk_drawing_area_new();
	gtk_widget_set_size_request(background_eraser, 35, 35);
	gtk_box_pack_start(GTK_BOX(eraser), background_eraser, FALSE, FALSE, 0);
	gtk_widget_set_events(background_eraser, GDK_BUTTON_PRESS_MASK);
	gtk_widget_set_tooltip_text(eraser, "Eraser Tool");	

// Options for line size
	GtkWidget *tip_size= gtk_drawing_area_new();
	gtk_widget_set_size_request(tip_size,100,30);
	gtk_widget_set_events(tip_size, GDK_BUTTON_PRESS_MASK);
	gtk_widget_set_tooltip_text(tip_size, "Setting tip size");

// Options for window size.
	GtkTreeIter iter;
	GtkListStore *list;
	list = 	gtk_list_store_new(1 ,G_TYPE_STRING);
	gtk_list_store_insert_with_values(GTK_LIST_STORE(list), &iter, 0, 0, "400X500", -1);
	gtk_list_store_insert_with_values(GTK_LIST_STORE(list), &iter, 1, 0, "600X500", -1);
	gtk_list_store_insert_with_values(GTK_LIST_STORE(list), &iter, 2, 0, "800X600", -1);
	gtk_list_store_insert_with_values(GTK_LIST_STORE(list), &iter, 3, 0, "1000X600", -1);
	GtkWidget *window_size= gtk_combo_box_new_with_model(GTK_TREE_MODEL(list));
	g_object_unref(list);
	GtkCellRenderer *column = gtk_cell_renderer_text_new();
	gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(window_size), column, TRUE);
	gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(window_size), column,
        "text", 0,
        NULL);
	gtk_combo_box_set_active(GTK_COMBO_BOX(window_size) , 0);
	gtk_widget_set_tooltip_text(window_size, "Setting window size");

// Colors
	GtkWidget *color = gtk_label_new("Color");
	GtkWidget *color_grid = gtk_grid_new();
	GtkWidget *white = gtk_button_new();
	GtkWidget *red = gtk_button_new();
	GtkWidget *orange = gtk_button_new();
	GtkWidget *yellow = gtk_button_new();
	GtkWidget *green = gtk_button_new();
	GtkWidget *blue = gtk_button_new();
	GtkWidget *purple = gtk_button_new();
	GtkWidget *brown = gtk_button_new();
	GtkWidget *black = gtk_button_new();


// The drawing sheet. The area where we do all the drawing.
	GtkWidget *area;
	area = gtk_grid_new();
	GtkWidget  *sheet = gtk_drawing_area_new(); 
	gtk_widget_set_size_request(sheet, drawing_sheet.width, drawing_sheet.height); 
	gtk_widget_set_events(sheet, GDK_BUTTON_MOTION_MASK|GDK_BUTTON_PRESS_MASK);
	gtk_widget_queue_resize(area);

// Styles
	GtkCssProvider *styles = gtk_css_provider_new();
	gtk_css_provider_load_from_path(styles, "style/style.css", NULL);
	GtkStyleContext *tools = gtk_widget_get_style_context(tool);
	GtkStyleContext *pencil_tool = gtk_widget_get_style_context(pencil);
	GtkStyleContext *line_tool = gtk_widget_get_style_context(line);
	GtkStyleContext *curve_tool = gtk_widget_get_style_context(curve);
	GtkStyleContext *circle_tool = gtk_widget_get_style_context(circle);
	GtkStyleContext *colors = gtk_widget_get_style_context(color);
	GtkStyleContext *whiteC = gtk_widget_get_style_context(white);
	GtkStyleContext *redC = gtk_widget_get_style_context(red);
	GtkStyleContext *orangeC = gtk_widget_get_style_context(orange);
	GtkStyleContext *yellowC = gtk_widget_get_style_context(yellow);
	GtkStyleContext *blueC = gtk_widget_get_style_context(blue);
	GtkStyleContext *purpleC = gtk_widget_get_style_context(purple);
	GtkStyleContext *brownC = gtk_widget_get_style_context(brown);
	GtkStyleContext *blackC = gtk_widget_get_style_context(black);
	GtkStyleContext *greenC = gtk_widget_get_style_context(green);
	GtkStyleContext *panel_style = gtk_widget_get_style_context(GTK_WIDGET(panel)); 
	GtkStyleContext *area_style = gtk_widget_get_style_context(GTK_WIDGET(area));
	GtkStyleContext *color_grid_style = gtk_widget_get_style_context(GTK_WIDGET(color_grid));
	gtk_style_context_add_class(tools, "labels");
	gtk_style_context_add_class(pencil_tool, "pencil");
	gtk_style_context_add_class(line_tool, "line");
	gtk_style_context_add_class(curve_tool, "curve");
	gtk_style_context_add_class(circle_tool, "circle");
	gtk_style_context_add_class(colors, "labels");
	gtk_style_context_add_class(redC,"red");
	gtk_style_context_add_class(yellowC,"yellow");
	gtk_style_context_add_class(orangeC,"orange");
	gtk_style_context_add_class(greenC,"green");
	gtk_style_context_add_class(blueC,"blue");
	gtk_style_context_add_class(brownC,"brown");
	gtk_style_context_add_class(blackC,"black");
	gtk_style_context_add_class(purpleC,"purple");
	gtk_style_context_add_class(whiteC,"white");
	gtk_style_context_add_class(panel_style, "panels");
	gtk_style_context_add_class(area_style, "area");
	gtk_style_context_add_class(color_grid_style, "grid");
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
		GTK_STYLE_PROVIDER(styles),
		GTK_STYLE_PROVIDER_PRIORITY_USER);

//Signals
	g_signal_connect(G_OBJECT (window), "destroy", G_CALLBACK (destroy), NULL); 
	g_signal_connect(G_OBJECT(pencil), "clicked", G_CALLBACK(set_current_tool_pencil), sheet);
	g_signal_connect(G_OBJECT(line), "clicked", G_CALLBACK(set_current_tool_line), sheet);
	g_signal_connect(G_OBJECT(curve), "clicked", G_CALLBACK(set_current_tool_curve), sheet);	
	g_signal_connect(G_OBJECT(circle), "clicked", G_CALLBACK(set_current_tool_circle), sheet);	
	g_signal_connect(G_OBJECT(tip_size), "realize", G_CALLBACK(set_tip), NULL);
	g_signal_connect(G_OBJECT(tip_size), "draw", G_CALLBACK(draw_tip), area);
	g_signal_connect(G_OBJECT(tip_size), "button-press-event", G_CALLBACK(update_size), NULL);
	g_signal_connect(G_OBJECT(window_size), "changed", G_CALLBACK(update_area), sheet);
	g_signal_connect(G_OBJECT(background_fill), "realize", G_CALLBACK(set_fill_bg), NULL);
	g_signal_connect(G_OBJECT(background_fill), "draw", G_CALLBACK(draw_fill_bg), NULL);
	g_signal_connect(G_OBJECT(background_fill), "button-press-event", G_CALLBACK(set_current_tool_fill), sheet);
	g_signal_connect(G_OBJECT(background_eraser), "realize", G_CALLBACK(set_eraser_bg), NULL);
	g_signal_connect(G_OBJECT(background_eraser), "draw", G_CALLBACK(draw_eraser_bg), NULL);
	g_signal_connect(G_OBJECT(background_eraser), "button-press-event", G_CALLBACK(set_current_tool_eraser), sheet);
	g_signal_connect(G_OBJECT(white), "clicked", G_CALLBACK(set_color_white), tip_size);
	g_signal_connect(G_OBJECT(yellow), "clicked", G_CALLBACK(set_color_yellow), tip_size);
	g_signal_connect(G_OBJECT(orange), "clicked", G_CALLBACK(set_color_orange), tip_size);
	g_signal_connect(G_OBJECT(red), "clicked", G_CALLBACK(set_color_red), tip_size);
	g_signal_connect(G_OBJECT(green), "clicked", G_CALLBACK(set_color_green), tip_size);
	g_signal_connect(G_OBJECT(blue), "clicked", G_CALLBACK(set_color_blue), tip_size);
	g_signal_connect(G_OBJECT(purple), "clicked", G_CALLBACK(set_color_purple), tip_size);
	g_signal_connect(G_OBJECT(brown), "clicked", G_CALLBACK(set_color_brown), tip_size);
	g_signal_connect(G_OBJECT(black), "clicked", G_CALLBACK(set_color_black), tip_size);
	g_signal_connect(sheet, "button-press-event", G_CALLBACK(on_click), NULL);
	g_signal_connect(sheet, "motion-notify-event", G_CALLBACK(on_motion), NULL);
	g_signal_connect(sheet, "realize", G_CALLBACK(set), NULL);
	g_signal_connect(sheet, "draw", G_CALLBACK(redraw), NULL);


// Attaching everything
	gtk_grid_attach(GTK_GRID(color_grid), white, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), yellow, 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), orange, 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), red, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), green, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), blue, 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), purple, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), brown, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(color_grid), black, 2, 2, 1, 1);
	gtk_grid_set_row_spacing(GTK_GRID(color_grid), 2);
	gtk_grid_set_column_spacing(GTK_GRID(color_grid), 2);
	gtk_grid_attach(GTK_GRID(panel), tool, 0, 0, 2, 1);
	gtk_grid_attach(GTK_GRID(panel), pencil, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), line, 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), curve, 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), circle, 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), tip_size, 0, 3, 2, 1);
	gtk_grid_attach(GTK_GRID(panel), window_size, 0, 4, 2, 1);
	gtk_grid_attach(GTK_GRID(panel), fill, 0, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), eraser, 1, 5, 1, 1);
	gtk_grid_attach(GTK_GRID(panel), color, 0, 6, 2, 1);
	gtk_grid_attach(GTK_GRID(panel), color_grid, 0, 7, 3, 3);
	gtk_grid_set_row_spacing(GTK_GRID(panel), 5);
	gtk_grid_set_column_spacing(GTK_GRID(panel), 5);
	gtk_grid_attach(GTK_GRID(area) ,sheet , 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(container), panel, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(container), area, 1, 0, 1, 100);
	gtk_grid_set_column_spacing(GTK_GRID(container), 10);
	gtk_container_add(GTK_CONTAINER(window), container);
	gtk_widget_show_all(window);

// Setting the default cursor
	gtk_widget_set_has_window (GTK_WIDGET (sheet), TRUE);
	gdk_window_set_cursor(gtk_widget_get_window(sheet), pencil_cursor);
	gtk_main(); 
} 

// All event callback functions
void init_color(gint index, gint red, gint green, gint blue){
	color[index].red = red;
	color[index].green = green;
	color[index].blue = blue;
}

// Sets either the foreground or background color
void set_color(cairo_t *cr, gint color){
	switch(color){
		case 0:
			cairo_set_source_rgb(cr, 1, 1, 1);
			break;
		case 1:
			cairo_set_source_rgb(cr, 1, 1, 0);
			break;
		case 2:
			cairo_set_source_rgb(cr, 1, 0.64453125, 0);
			break;
		case 3:
			cairo_set_source_rgb(cr, 1, 0, 0);
			break;
		case 4:
			cairo_set_source_rgb(cr, 0, 0.5, 0);
			break;
		case 5:
			cairo_set_source_rgb(cr, 0, 0, 1);
			break;
		case 6:
			cairo_set_source_rgb(cr, 0.625, 0.125, 0.9375);
			break;
		case 7:
			cairo_set_source_rgb(cr, 0.586, 0.293, 0);
			break;
		case 8:
			cairo_set_source_rgb(cr, 0, 0, 0);
			break;
	}
}

// A set of functions for various drawing functionalities
void draw_pencil(GtkWidget *widget){
	cairo_t *cr = cairo_create(surface);
	set_color(cr, fg_color);
	bresenham_line(widget, cr);	
}

void draw_line(GtkWidget *widget){
	surface = gdk_cairo_surface_create_from_pixbuf(buffer_surface, 0, NULL);
	cairo_t *cr = cairo_create(surface);
	set_color(cr, fg_color);
	bresenham_line(widget, cr);	
}

void draw_curve(GtkWidget *widget){
	surface = gdk_cairo_surface_create_from_pixbuf(buffer_surface, 0, NULL);
	cairo_t *cr = cairo_create(surface);
	set_color(cr, fg_color);
	bezier_curve(widget, cr);	
}

void draw_circle(GtkWidget *widget){
	surface = gdk_cairo_surface_create_from_pixbuf(buffer_surface, 0, NULL);
	cairo_t *cr = cairo_create(surface);
	set_color(cr, fg_color);
	bresenham_circle(widget, cr);
}

void erase(GtkWidget *widget){	
	cairo_t *cr = cairo_create(surface);
	set_color(cr, bg_color);
	bresenham_line(widget, cr);	
}

// Initializes the sheet 
gboolean set(GtkWidget *widget, gpointer data){ 
	if(surface)
		cairo_surface_destroy(surface);
    surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget), 
    	CAIRO_CONTENT_COLOR, drawing_sheet.width, 
    	drawing_sheet.height);
    cairo_t *cr;
  	cr = cairo_create(surface);
  	set_color(cr, bg_color);
 	cairo_paint(cr);
  	cairo_destroy(cr);
    undo_buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0,drawing_sheet.width, drawing_sheet.height);
}

// Connected to draw signal redraws the previous content everytime a draw signal is emitted
gboolean redraw(GtkWidget *widget, cairo_t *cr, gpointer data){
	cairo_set_source_surface(cr, surface, 0, 0);
  	cairo_paint (cr);
  	gtk_widget_queue_resize(widget);
}

// Mouse control based funcctions
gboolean on_motion(GtkWidget* widget, GdkEvent *type, gpointer data){
	GdkEventMotion *motion = (GdkEventMotion*)type;
	if(start_registering == 1){
		switch(current_tool){
			case 0:
				end.x = motion->x;
				end.y = motion->y;
				if(cohen_sutherland_clip()){
					draw_pencil(widget);
				}
				start.x = end.x;
				start.y = end.y;
				break;
			case 1:
				end.x = motion->x;
				end.y = motion->y;
				if(cohen_sutherland_clip()){
					draw_line(widget);
				}
				break;
			case 2:
				if(cohen_sutherland_clip()){
					draw_curve(widget);
				}
				control.x = motion->x;
				control.y = motion->y;
				break;
			case 3:
				end.x = motion->x;
				end.y = motion->y;
				draw_circle(widget);
				break;
			case 4:
				break;
			case 5:
				end.x = motion->x;
				end.y = motion->y;
				if(cohen_sutherland_clip()){
					erase(widget);
				}
				start.x = end.x;
				start.y = end.y;
				break;
		}
	}
}

gboolean on_click(GtkWidget* widget, GdkEvent *type, gpointer data){
	GdkEventButton *button = (GdkEventButton*)type;
	start_registering = 0;
	if(button->type == GDK_BUTTON_PRESS && button->button == 1){
		start_registering = 1;
		undo_buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0,drawing_sheet.width, drawing_sheet.height);
		base = push_stack(undo_buffer_surface, base);
		switch(current_tool){
			case 0:
				start.x = button->x;
				start.y = button->y;
				end.x = start.x;
				end.y = start.y;
				if(cohen_sutherland_clip()){
					draw_pencil(widget);
				}
				break;
			case 1:
				start.x = button->x;
				start.y = button->y;
				buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0,drawing_sheet.width, drawing_sheet.height);
				break;
			case 2:
				switch(curve_points_counter){
					case 0:
						start.x = button->x;
						start.y = button->y;
						start_registering = 0;
						break;
					case 1:
						end.x = button->x;
						end.y = button->y;
						start_registering = 0;
						break;
					case 2:
						control.x = button->x;
						control.y = button->y;
						break;
				}
				curve_points_counter = (curve_points_counter + 1)%3;
				buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0,drawing_sheet.width, drawing_sheet.height);
				break;
			case 3:
				start.x = button->x;
				start.y = button->y; 
				buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0,drawing_sheet.width, drawing_sheet.height);
				break;
			case 4:
				seed.x = button->x;
				seed.y = button->y;
				cairo_t *cr = cairo_create(surface);
				set_color(cr, fg_color);
				buffer_surface = gdk_pixbuf_get_from_surface(surface, 0, 0, drawing_sheet.width, drawing_sheet.height);
	  			n_channels = gdk_pixbuf_get_n_channels (buffer_surface);
	  			rowstride = gdk_pixbuf_get_rowstride (buffer_surface);
	  			pixels = gdk_pixbuf_get_pixels (buffer_surface);
	  			for(base_color = 0 ; base_color < 9 ; base_color++){
					current = pixels + seed.y * rowstride + seed.x * n_channels;
					if(color[base_color].red == current[0] && color[base_color].green == current[1] && color[base_color].blue == current[2])
						break; 
				}
				flood_fill(widget, cr, seed.x, seed.y);
				start_registering = 0;
				break;
			case 5:
				start.x = button->x;
				start.y = button->y;
				end.x = start.x;
				end.y = start.y;
				if(cohen_sutherland_clip())
					erase(widget);
				break;
		}	
	}
	if(button->type == GDK_BUTTON_PRESS && button->button == 3){
		if(current_tool == 5)
			set(widget,NULL);
		else{
			if(!is_empty_stack()){
				surface = gdk_cairo_surface_create_from_pixbuf(pop_stack(base), 0, NULL);
				gtk_widget_queue_draw(widget);
			}
		}
	}
}

// Destroys the window when user closes the app
void destroy(GtkWidget* widget, gpointer data) { 
	gtk_main_quit(); 
}

// A set of function for drawing the tip size widget
gboolean set_tip(GtkWidget *widget, gpointer data){
	if(size_surface)
		cairo_surface_destroy(size_surface);
	size_surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget), 
    	CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget), 
    	gtk_widget_get_allocated_height (widget));
	cairo_t *cr;
  	cr = cairo_create(size_surface);
  	set_color(cr, fg_color);
 	cairo_paint (cr);
  	cairo_destroy (cr);
}

gboolean draw_tip(GtkWidget *widget, cairo_t *cr, gpointer data){
	set_color(cr, 8);
	cairo_set_line_width(cr, (double)size);
	cairo_move_to(cr, 10, 15);
	cairo_line_to(cr, 90, 15);
	cairo_stroke(cr);
	gtk_widget_queue_draw(widget);
}

gboolean update_size(GtkWidget* widget, GdkEvent *type, gpointer data){
	GdkEventButton *button = (GdkEventButton*)type;
	if(button->type == GDK_BUTTON_PRESS && button->button == 1){
		size = (size + 1)%7;
		if(size == 0)
			size = 1;
	}
	if(button->type == GDK_BUTTON_PRESS && button->button == 3){
		size = (size - 1)%7;
		if(size == 0)
			size = 7;
	}
	gtk_widget_queue_draw(widget);
}

// For setting the sheet size
gboolean update_area(GtkComboBox* widget, gpointer data){
	gint setWidth = gtk_combo_box_get_active(widget);
	switch(setWidth){
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
	gtk_widget_set_size_request(data, drawing_sheet.width, drawing_sheet.height);
	cairo_surface_destroy(surface);
    surface = gdk_window_create_similar_surface (gtk_widget_get_window (data), 
    	CAIRO_CONTENT_COLOR,drawing_sheet.width, 
    	drawing_sheet.height);
    cairo_t *cr;
  	cr = cairo_create(surface);
  	set_color(cr, bg_color);
 	cairo_paint (cr);
  	cairo_destroy (cr);
}

// Setting the current active tool 
gboolean set_current_tool_pencil(GtkButton *button, gpointer data){
	current_tool = 0;
	consider_bg_as_set = 0;
	gdk_window_set_cursor(gtk_widget_get_window(data), pencil_cursor);
}

gboolean set_current_tool_line(GtkButton *button, gpointer data){
	current_tool = 1;
	consider_bg_as_set = 0;
	gdk_window_set_cursor(gtk_widget_get_window(data), crosshair);
}

gboolean set_current_tool_curve(GtkButton *button, gpointer data){
	current_tool = 2;
	consider_bg_as_set = 0;
	gdk_window_set_cursor(gtk_widget_get_window(data), crosshair);
}

gboolean set_current_tool_circle(GtkButton *button, gpointer data){
	current_tool = 3;
	consider_bg_as_set = 0;
	gdk_window_set_cursor(gtk_widget_get_window(data), crosshair);
}

gboolean set_current_tool_fill(GtkButton *button, GdkEvent *type, gpointer data){
	current_tool = 4;
	consider_bg_as_set = 0;
	gdk_window_set_cursor(gtk_widget_get_window(data), paint_cursor);
}

// Fill and eraser button are custom widgets. Following are the functions associated with it
gboolean set_fill_bg(GtkWidget *widget, gpointer data){
	if(fill_surface)
		cairo_surface_destroy(fill_surface);
	eraser_surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget), 
    	CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget), 
    	gtk_widget_get_allocated_height (widget));
	cairo_t *cr;
  	cr = cairo_create(fill_surface);
  	set_color(cr, 0);
 	cairo_paint (cr);
  	cairo_destroy (cr);
}

gboolean draw_fill_bg(GtkWidget *widget, cairo_t *cr, gpointer data){
// Fill bucket
	set_color(cr, 8);
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, 31, 8);
	cairo_line_to(cr, 31, 28);
	cairo_line_to(cr, 14, 28);
	cairo_line_to(cr, 14, 8);
	cairo_stroke(cr);

// Fill color
	set_color(cr, fg_color);
	cairo_set_line_cap (cr, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, 10, 8);
	cairo_line_to(cr, 35, 8);
	cairo_line_to(cr, 35, 12);
	cairo_curve_to(cr, 24, 16, 24, 16, 24, 20);	
	cairo_arc(cr, 23, 20, 1, 0, G_PI);
	cairo_line_to(cr, 22, 18);
	cairo_arc_negative(cr, 21, 18, 1, 0, G_PI);
	cairo_line_to(cr, 20, 22);
	cairo_arc(cr, 19, 22, 1, 0, G_PI);
	cairo_line_to(cr, 18, 15);
	cairo_arc_negative(cr, 17, 15, 1, 0, G_PI);
	cairo_line_to(cr, 16, 20);
	cairo_arc(cr, 15, 20, 1, 0, G_PI);
	cairo_curve_to(cr, 14, 12, 14, 12, 10, 12);
	cairo_line_to(cr, 10, 8);
	cairo_fill(cr);
	gtk_widget_queue_draw(widget);
}

gboolean set_current_tool_eraser(GtkButton *button, GdkEvent *type, gpointer data){
	current_tool = 5;
	consider_bg_as_set = 1;
	gdk_window_set_cursor(gtk_widget_get_window(data), eraser_cursor);
}

gboolean set_eraser_bg(GtkWidget *widget, gpointer data){
	if(eraser_surface)
		cairo_surface_destroy(eraser_surface);
	eraser_surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget), 
    	CAIRO_CONTENT_COLOR, gtk_widget_get_allocated_width (widget), 
    	gtk_widget_get_allocated_height (widget));
	cairo_t *cr;
  	cr = cairo_create(eraser_surface);
  	set_color(cr, 0);
 	cairo_paint (cr);
  	cairo_destroy (cr);
}

gboolean draw_eraser_bg(GtkWidget *widget, cairo_t *cr, gpointer data){

// Eraser
	set_color(cr, 8);
	cairo_set_line_width(cr, 1);
	cairo_move_to(cr, 17, 28);
	cairo_line_to(cr, 10, 18);
	cairo_line_to(cr, 15, 14);
	cairo_line_to(cr, 22, 24);
	cairo_line_to(cr, 17, 28);
	cairo_stroke(cr);
	cairo_move_to(cr, 23, 25);
	cairo_line_to(cr, 33, 17);
	cairo_line_to(cr, 24, 5);
	cairo_line_to(cr, 14, 13);
	cairo_line_to(cr, 23, 25);
	cairo_close_path(cr);
	cairo_fill(cr);
	cairo_stroke(cr);

// Line that tells about the background color
	set_color(cr, bg_color);
	cairo_set_line_width(cr, 4);
	cairo_move_to(cr, 13, 28);
	cairo_line_to(cr, 35, 28); 
	cairo_stroke(cr);
	gtk_widget_queue_draw(widget);
}

// A set of funtions that set the corresponding color
gboolean set_color_white(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 0;
	else
		bg_color = 0;
}

gboolean set_color_yellow(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 1;
	else
		bg_color = 1;
}

gboolean set_color_orange(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 2;
	else
		bg_color = 2;
}

gboolean set_color_red(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 3;
	else
		bg_color = 3;
}

gboolean set_color_green(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 4;
	else
		bg_color = 4;
}

gboolean set_color_blue(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 5;
	else
		bg_color = 5;
}

gboolean set_color_purple(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 6;
	else
		bg_color = 6;
}

gboolean set_color_brown(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 7;
	else
		bg_color = 7;
}

gboolean set_color_black(GtkWidget *widget, gpointer data){
	if(consider_bg_as_set == 0)
		fg_color = 8;
	else
		bg_color = 8;
}

// UNDO Stack
gboolean is_full_stack(){
	Stack *current;
	int count = 0;
	for(current = base, count ; current->next != NULL ; count++, current = current->next);
	return (count == STACK_MAX);
}

gboolean is_empty_stack(){
	return (base == NULL);
}

Stack *push_stack(GdkPixbuf *buffer, Stack *base){
	Stack *current, *new_buffer;
	new_buffer = (Stack *)malloc(sizeof(Stack *));
	new_buffer->undo_buffer = buffer; 
	new_buffer->next = NULL;
	if(is_empty_stack()){
		return new_buffer;
	}
	if(is_full_stack()){
		base = base->next;
	}
	for(current = base; current->next != NULL; current = current->next);
	current->next = new_buffer;
	return base;
}

GdkPixbuf *pop_stack(Stack *base){
	Stack *current, *temp;
	GdkPixbuf *buffer;
	if(base->next== NULL){
		buffer = base->undo_buffer;
		base = NULL;
	}
	else{
		for(current = base, temp = base; current->next != NULL ; temp = current, current = current->next);
		buffer = current->undo_buffer;
		temp->next = NULL;
	}
	return buffer;
}

// Paint Queue
gboolean is_queue_empty(){
	return (front == NULL);	
}

Queue *push_queue(gint x, gint y, Queue *front){
	Queue *current, *new_point;
	new_point = (Queue *)malloc(sizeof(Queue *));
	new_point->point = (Point *)malloc(sizeof(Point *)); 
	new_point->point->x = x;
	new_point->point->y = y;
	new_point->next = NULL;
	if(is_queue_empty())
		return new_point;
	for(current = front ; current->next != NULL ; current = current->next);
	current->next = new_point;
	return front;
}

Point *pop_queue(){
	Queue *temp; 
	Point *buffer;
	buffer = front->point;
	if(front->next == NULL){
		front = NULL;
	}
	else{
		front = front->next;
	}
	return buffer;
}

// All the algorithms that have been used so far
void bresenham_line(GtkWidget *widget, cairo_t *cr){
	gint d;
	sign.y = 1, sign.x = 1;
	if(abs(end.y - start.y) < abs(end.x - start.x)){	
		buffer[0].x = start.x > end.x? end.x : start.x;
		buffer[0].y = start.x > end.x? end.y : start.y;
		buffer[1].x = start.x > end.x ? start.x : end.x;
		buffer[1].y = start.x > end.x? start.y : end.y; 
		difference.x = buffer[1].x - buffer[0].x;
		difference.y = buffer[1].y - buffer[0].y;
		if(difference.y < 0){
			sign.y = -1;
			difference.y = -difference.y;
		}
		d = 2*difference.y - difference.x;
		while(buffer[0].x <= buffer[1].x){
			if(d >= 0){
				cairo_rectangle (cr, buffer[0].x - (size/2), buffer[0].y - (size/2), size, size);	
				cairo_fill(cr);
				buffer[0].y = buffer[0].y + sign.y;
				d = d + 2*(difference.y - difference.x);
			}
			else{
				cairo_rectangle (cr, buffer[0].x - (size/2), buffer[0].y - (size/2), size, size);	
				cairo_fill(cr);
				d = d + 2*difference.y;
			}
			buffer[0].x = buffer[0].x + 1; 
		}	
	}
	else {
		buffer[0].x = start.y > end.y? end.x : start.x;
		buffer[0].y = start.y > end.y? end.y : start.y;
		buffer[1].x = start.y > end.y ? start.x : end.x;
		buffer[1].y = start.y > end.y? start.y : end.y; 
		difference.x = buffer[1].x - buffer[0].x;
		difference.y = buffer[1].y - buffer[0].y;
		if(difference.x < 0){
			sign.x = -1;
			difference.x = -difference.x;
		}
		d = 2*difference.x - difference.y;
		while(buffer[0].y <= buffer[1].y){
			if(d >= 0){
				cairo_rectangle (cr, buffer[0].x - (size/2), buffer[0].y - (size/2), size, size);	
				cairo_fill(cr);
				buffer[0].x = buffer[0].x + sign.x;
				d = d + 2*(difference.x - difference.y);
			}
			else{
				cairo_rectangle (cr, buffer[0].x - (size/2), buffer[0].y - (size/2), size, size);	
				cairo_fill(cr);
				d = d + 2*difference.x;
			}
			buffer[0].y = buffer[0].y + 1; 
		}
	}
	cairo_rectangle (cr, buffer[1].x - (size/2), buffer[1].y - (size/2), size, size);	
	cairo_fill(cr);
	cairo_destroy(cr);
}

gint region_code(gint x, gint y){

	gint code = inside; 
	if(x < 0)
		code != left;
	if(x > drawing_sheet.width)
		code != right;
	if(y < 0)
		code != bottom;
	if(y > drawing_sheet.height)
		code != top;
	return code;

}

gboolean cohen_sutherland_clip(){
	gint region_code_1 = region_code(start.x, start.y);
	gint region_code_2 = region_code(end.x, end.y);
	gint region_code_considered;
	gdouble x, y;
	while(TRUE){
		if(!(region_code_1 | region_code_2)){
			return TRUE;
		}
		else if(region_code_1 & region_code_2){
			return FALSE;
		}
		else{
			region_code_considered = region_code_1 < region_code_2 ? region_code_2 : region_code_1;
			if (region_code_considered & top) {           // point is above the clip window
				x = start.x + (end.x - start.x) * (drawing_sheet.height - start.y) / (end.y - start.y);
				y = drawing_sheet.height;
			} else if (region_code_considered & bottom) { // point is below the clip window
				x = start.x + (end.x - start.x) * (0 - start.y) / (end.y - start.y);
				y = 0;
			} else if (region_code_considered & right) {  // point is to the right of clip window
				y = start.y + (end.y - start.y) * (drawing_sheet.width - start.x) / (end.x - start.x);
				x = drawing_sheet.width;
			} else if (region_code_considered & left) {   // point is to the left of clip window
				y = start.y + (end.y - start.y) * (0 - start.x) / (end.x - start.x);
				x = 0;
			}
			if(region_code_considered == region_code_1){
				start.x = x;
				start.y = y;
				region_code_1 = region_code(start.x, start.y);
			}
			else{
				end.x = x;
				end.y = y;
				region_code_2 = region_code(end.x, end.y);
			}
		}
	}
}

void bezier_curve(GtkWidget *widget, cairo_t *cr){
	gdouble u = 0.0001;
	for(u ; u < 1 ; u +=0.0001){
		buffer[0].x = pow(1 - u, 2)*start.x + 2*(1 - u)*u*control.x + pow(u,2)*end.x;
		buffer[0].y = pow(1 - u, 2)*start.y + 2*(1 - u)*u*control.y + pow(u,2)*end.y;
		cairo_rectangle(cr, buffer[0].x - (size)/2, buffer[0].y - (size)/2, size, size);
		cairo_fill(cr); 
	}
	cairo_destroy(cr);
}

void bresenham_circle(GtkWidget *widget, cairo_t *cr){
	gint x = 0 , y = (gint)sqrt(pow(end.y - start.y, 2) + pow(end.x - start.x, 2)), d = 3 - 2*y;
	cairo_rectangle(cr, start.x - (size/2) + x, start.y - (size/2) + y, size, size);
	cairo_rectangle(cr, start.x - (size/2) - x, start.y - (size/2) + y, size, size);
	cairo_rectangle(cr, start.x - (size/2) + x, start.y - (size/2) - y, size, size);
	cairo_rectangle(cr, start.x - (size/2) - x, start.y - (size/2) - y, size, size);
	cairo_rectangle(cr, start.x - (size/2) + y, start.y - (size/2) + x, size, size);
	cairo_rectangle(cr, start.x - (size/2) - y, start.y - (size/2) + x, size, size);
	cairo_rectangle(cr, start.x - (size/2) + y, start.y - (size/2) - x, size, size);
	cairo_rectangle(cr, start.x - (size/2) - y, start.y - (size/2) - x, size, size);
	while(y >= x){
		x++;
		if(d > 0){
			y--;
			d = d + 4*(x - y) + 10;
		}
		else
			d = d + 4*x + 6;
		cairo_rectangle(cr, start.x - (size/2) + x, start.y - (size/2) + y, size, size);
		cairo_rectangle(cr, start.x - (size/2) - x, start.y - (size/2) + y, size, size);
		cairo_rectangle(cr, start.x - (size/2) + x, start.y - (size/2) - y, size, size);
		cairo_rectangle(cr, start.x - (size/2) - x, start.y - (size/2) - y, size, size);
		cairo_rectangle(cr, start.x - (size/2) + y, start.y - (size/2) + x, size, size);
		cairo_rectangle(cr, start.x - (size/2) - y, start.y - (size/2) + x, size, size);
		cairo_rectangle(cr, start.x - (size/2) + y, start.y - (size/2) - x, size, size);
		cairo_rectangle(cr, start.x - (size/2) - y, start.y - (size/2) - x, size, size);
	}
	cairo_fill(cr);
	cairo_destroy(cr);
}

gboolean check_pixel(gint x, gint y){
	if(x >= 0 && x < drawing_sheet.width && y >= 0 && y < drawing_sheet.height){
		current = pixels + y * rowstride + x * n_channels;
		if(color[fg_color].red == current[0] && color[fg_color].green == current[1] && color[fg_color].blue == current[2])
			return FALSE;
		if(color[base_color].red == current[0] && color[base_color].green == current[1] && color[base_color].blue == current[2])
			return TRUE;
	}
	return FALSE;
}

void flood_fill(GtkWidget *widget, cairo_t *cr, gint x, gint y){
	Point *current_point;
	gint west, east, valid_north, valid_south;
	if(!check_pixel(x,y))
		return;
	front = push_queue(x,y, NULL);
  	while(!is_queue_empty()){
  		valid_north = 0, valid_south = 0;
  		current_point = pop_queue();
  		for(west = current_point->x; check_pixel(west, current_point->y); west--);
  		for(east = current_point->x; check_pixel(east, current_point->y); east++);
  		for(west = west + 1 ; west < east ; west++){
  			current = pixels + current_point->y * rowstride + west * n_channels;
  			current[0] = color[fg_color].red;
		  	current[1] = color[fg_color].green;
		  	current[2] = color[fg_color].blue;
			cairo_rectangle(cr, west, current_point->y, 1, 1);
			cairo_fill(cr);
			if(check_pixel(west, current_point->y - 1)){
				if(valid_north == 0){
					front = push_queue(west, current_point->y - 1, front);
					valid_north = 1; 
				}
			}
			else
				valid_north = 0;
			if(check_pixel(west, current_point->y + 1)){
				if(valid_south == 0){
					front = push_queue(west, current_point->y + 1, front);
					valid_south = 1;
				}
			}
			else
				valid_south = 0;
  		}
	}
}
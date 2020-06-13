/*
* Variable.h file contains all the global variables and
* structures that are needed by the data_structures.h and algo.h.
*/
#ifndef VARIABLE_H
#define VARIABLE_H

#include<gtk/gtk.h> 

typedef struct Point{
	gint x;
	gint y;
}Point;

typedef struct Sheet{
	gint width;
	gint height;
}Sheet;

typedef struct Color{
	gint red;
	gint green;
	gint blue;
}Color;

static gint rowstride, n_channels,
	current_tool = 0, start_registering = 0, 
	bg_color = 0, fg_color = 8, base_color = 0, 
	size = 1, curve_points_counter = 0, 
	consider_bg_as_set = 0; 
static GdkPixbuf *buffer_surface,
	*undo_buffer_surface, *buf;
static GdkCursor *crosshair, *pencil_cursor, 
	*paint_cursor, *eraser_cursor;
static guchar *pixels, *current;
static cairo_surface_t *surface = NULL, 
	*size_surface = NULL, 
	*eraser_surface = NULL, 
	*fill_surface = NULL;
static Point start, end, control, buffer[2],
	sign, difference, seed;
static Sheet drawing_sheet;
static Color color[9];

#define STACK_MAX 10

typedef struct Stack{
	GdkPixbuf *undo_buffer; 
	struct Stack *next;
}Stack;
static Stack *base = NULL;

typedef struct Queue{
	Point *point;
	struct Queue *next;
}Queue;
static Queue *front = NULL;

static const gint inside = 0;
static const gint left = 1;
static const gint right = 2;
static const gint bottom = 4;
static const gint top = 8;
static Point start, 
	end, 
	control, 
	buffer[2],
	sign, 
	difference, 
	seed;

#endif
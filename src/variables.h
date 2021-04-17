/*
	I have declared some of the gloabal variables seperately in this file.
*/

#ifndef VARIABLES_H
#define VARIABLES_H

#include <gtk/gtk.h>

struct Sheet{
	gint width;
	gint height;
};

struct Color{
	gint red;
	gint green;
	gint blue;
};

struct Point{
	gint x;
	gint y;
};

extern struct Sheet drawing_sheet;
extern struct Color foreground;
extern struct Color background;
extern gint current_tool;
extern gint size;

#endif

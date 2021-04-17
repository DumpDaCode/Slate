/*
    The Pixel buffer modification functionalities.
*/

#include "graphics.h"
#include "variables.h"
#include "app_ds.h"
#include <math.h>

static const gint inside = 0;
static const gint left = 1;
static const gint right = 2;
static const gint bottom = 4;
static const gint top = 8;

static GdkPixbuf *sheet_buffer = NULL;
static struct Point buffer[2], sign, difference;
static struct Color pixel;
struct Queue *front;
static guchar *pixels, *current;
static gint rowstride, n_channels;

void sivop(GdkPixbuf *buffer_surface){
/*
    set internal variables for paint
*/
    sheet_buffer = buffer_surface;
    n_channels = gdk_pixbuf_get_n_channels (buffer_surface);
    rowstride = gdk_pixbuf_get_rowstride (buffer_surface);
    pixels = gdk_pixbuf_get_pixels (buffer_surface);
}

GdkPixbuf *rivop(){
/*
    return internal variables for paint
*/
    return sheet_buffer;
}

struct Color *get_pixel(gint x, gint y){
/*
    Returns a Pixel's color data from the Pixel buffer.
*/
    current = pixels + y * rowstride + x * n_channels;
    pixel.red = current[0];
    pixel.green = current[1];
    pixel.blue = current[2];
    return &pixel;
}

void draw_pixel(gint x, gint y, struct Color *color, gint p_size){
/*
    Changes a Pixels color data int the Pixel buffer.
*/
    if(x >= 0 && x < drawing_sheet.width && y >= 0 && y < drawing_sheet.height){
        if(p_size == 0){
            current = pixels + y * rowstride + x * n_channels;
            current[0] = color->red;
            current[1] = color->green;
            current[2] = color->blue;
        }
        else{
            flood_fill(x, y, color, p_size);
        }
    }
}

/*
    Except Flood fill. All the algorithms and their explanation can be found on
    the internet. The flood fill algorithm that I had used is one of the
    fastest. I have modified a little bit to use it for generating pixels of
    different sizes.
*/

void bresenham_line(struct Point start, struct Point end, struct Color *color, gint p_size){
    if(cohen_sutherland_clip(&start, &end)){
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
    				draw_pixel (buffer[0].x, buffer[0].y, color, p_size);
    				buffer[0].y = buffer[0].y + sign.y;
    				d = d + 2*(difference.y - difference.x);
    			}
    			else{
    				draw_pixel(buffer[0].x, buffer[0].y, color, p_size);
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
    				draw_pixel (buffer[0].x, buffer[0].y, color, p_size);
    				buffer[0].x = buffer[0].x + sign.x;
    				d = d + 2*(difference.x - difference.y);
    			}
    			else{
    				draw_pixel (buffer[0].x, buffer[0].y, color, p_size);
    				d = d + 2*difference.x;
    			}
    			buffer[0].y = buffer[0].y + 1;
    		}
    	}
    	draw_pixel (buffer[1].x , buffer[1].y , color, p_size);
    }
}

gint region_code(gint x, gint y){

	gint code = inside;
	if(x < 0)
		code |= left;
	if(x > drawing_sheet.width)
		code |= right;
	if(y < 0)
		code |= bottom;
	if(y > drawing_sheet.height)
		code |= top;
	return code;

}

gboolean cohen_sutherland_clip(struct Point *start, struct Point *end){
	gint region_code_1 = region_code(start->x, start->y);
	gint region_code_2 = region_code(end->x, end->y);
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
				x = start->x + (end->x - start->x) * (drawing_sheet.height - start->y) / (end->y - start->y);
				y = drawing_sheet.height;
			} else if (region_code_considered & bottom) { // point is below the clip window
				x = start->x + (end->x - start->x) * (0 - start->y) / (end->y - start->y);
				y = 0;
			} else if (region_code_considered & right) {  // point is to the right of clip window
				y = start->y + (end->y - start->y) * (drawing_sheet.width - start->x) / (end->x - start->x);
				x = drawing_sheet.width;
			} else if (region_code_considered & left) {   // point is to the left of clip window
				y = start->y + (end->y - start->y) * (0 - start->x) / (end->x - start->x);
				x = 0;
			}
			if(region_code_considered == region_code_1){
				start->x = x;
				start->y = y;
				region_code_1 = region_code(start->x, start->y);
			}
			else{
				end->x = x;
				end->y = y;
				region_code_2 = region_code(end->x, end->y);
			}
		}
	}
}

void bezier_curve(struct Point start, struct Point end, struct Point control, struct Color *color, gint p_size){
	gdouble u = 0.0001;
	for(u = 0.0001; u < 1 ; u +=0.0001){
		buffer[0].x = pow(1 - u, 2)*start.x + 2*(1 - u)*u*control.x + pow(u,2)*end.x;
		buffer[0].y = pow(1 - u, 2)*start.y + 2*(1 - u)*u*control.y + pow(u,2)*end.y;
		draw_pixel (buffer[0].x , buffer[0].y , color ,p_size);
	}
}

void bresenham_circle(struct Point start, struct Point end, struct Color *color, gint p_size){
	gint x = 0 , y = (gint)sqrt(pow(end.y - start.y, 2) + pow(end.x - start.x, 2)), d = 3 - 2*y;
	draw_pixel(start.x  + x, start.y  + y, color, p_size);
    draw_pixel(start.x  - x, start.y  + y, color, p_size);
    draw_pixel(start.x  + x, start.y  - y, color, p_size);
    draw_pixel(start.x  - x, start.y  - y, color, p_size);
	draw_pixel(start.x  + y, start.y  + x, color, p_size);
    draw_pixel(start.x  - y, start.y  + x, color, p_size);
    draw_pixel(start.x  + y, start.y  - x, color, p_size);
    draw_pixel(start.x  - y, start.y  - x, color, p_size);
	while(y >= x){
		x++;
		if(d > 0){
			y--;
			d = d + 4*(x - y) + 10;
		}
		else
			d = d + 4*x + 6;
        draw_pixel(start.x  + x, start.y  + y, color, p_size);
        draw_pixel(start.x  - x, start.y  + y, color, p_size);
        draw_pixel(start.x  + x, start.y  - y, color, p_size);
        draw_pixel(start.x  - x, start.y  - y, color, p_size);
        draw_pixel(start.x  + y, start.y  + x, color, p_size);
        draw_pixel(start.x  - y, start.y  + x, color, p_size);
        draw_pixel(start.x  + y, start.y  - x, color, p_size);
        draw_pixel(start.x  - y, start.y  - x, color, p_size);
	}
}

gboolean check_pixel(gint x, gint y, struct Color *fgcolor, struct Color *bgcolor, gint p_size, gint ogx, gint ogy){
	if(x >= 0 && x < drawing_sheet.width && y >= 0 && y < drawing_sheet.height){
		current = pixels + y * rowstride + x * n_channels;
        if(fgcolor->red == current[0] && fgcolor->green == current[1] && fgcolor->blue == current[2]){
            return FALSE;
        }
        if(p_size == 0){
		    if(bgcolor->red == current[0] && bgcolor->green == current[1] && bgcolor->blue == current[2]){
			    return TRUE;
            }
        }
        else{
            if(abs(x - ogx) <= p_size && abs(y-ogy) <= p_size)
                return TRUE;
            return FALSE;
        }
	}
	return FALSE;
}

void flood_fill(gint x, gint y, struct Color *color, gint p_size){
	struct Point *current_point;
	gint west, east, valid_north, valid_south;
    struct Color *base_color = get_pixel(x,y);
	if(!check_pixel(x, y ,color, base_color, p_size, x, y)){
        if(p_size > 0){
            return;
        }
    }
	front = push_queue(x,y, NULL);
  	while(!is_queue_empty()){
  		valid_north = 0, valid_south = 0;
  		current_point = pop_queue();
  		for(west = current_point->x; check_pixel(west, current_point->y, color, base_color, p_size, x, y); west--);
  		for(east = current_point->x; check_pixel(east, current_point->y, color, base_color, p_size, x, y); east++);
  		for(west = west + 1 ; west < east ; west++){
			draw_pixel(west, current_point->y, color, 0);
			if(check_pixel(west, current_point->y - 1, color, base_color, p_size, x, y)){
				if(valid_north == 0){
					front = push_queue(west, current_point->y - 1, front);
					valid_north = 1;
				}
			}
			else
				valid_north = 0;
			if(check_pixel(west, current_point->y + 1, color, base_color, p_size, x, y)){
				if(valid_south == 0){
					front = push_queue(west, current_point->y + 1, front);
					valid_south = 1;
				}
			}
			else
				valid_south = 0;
  		}
	}
    free(current_point);
    free(front);
}

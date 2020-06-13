/*
* The forward reference of all the functions that are utilized for creation of 
* undo stack and paint queue
*/
#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "variable.h"
#include <stdlib.h>

gboolean is_full_stack();
gboolean is_empty_stack();
Stack *push_stack(GdkPixbuf *buffer, Stack *base);
GdkPixbuf *pop_stack(Stack *base);
gboolean is_queue_empty();
Queue *push_queue(gint x, gint y, Queue *front);
Point *pop_queue();

#endif
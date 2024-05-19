#include "list.h"
#include <ncurses.h>

#ifndef DISPLAY_H
#define DISPLAY_H

WINDOW *initialize_display();

void display_welcome(WINDOW *);

void display_goal_list(WINDOW *, struct lnode *, int, char *);

#endif

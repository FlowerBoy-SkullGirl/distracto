#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers/global.h"
#include "headers/distracto_errors.h"
#include "headers/modeswitching.h"
#include "headers/terminal_mode.h"
#include "headers/list.h"
#include "headers/display.h"

char *append_to_buffer(char *buffer, char last)
{
	if (buffer == NULL)
		buffer = malloc(MAX_GOAL_SIZE);

	int end = strlen(buffer);

	if (end >= MAX_GOAL_SIZE -1)
		return buffer;

	*(buffer + end++) = last;
	*(buffer + end) = '\0';

	return buffer;
}

char *pop_buffer(char *buffer)
{
	int end = strlen(buffer) - 1;
	if (end < 0)
		return buffer;

	*(buffer + end) = '\0';
	return buffer;
}

int interpret_input(int wchar, char *buffer, int *selection, struct lnode *root)
{
	int select = *selection;
	int t_m = current_text_mode();
	struct lnode *np = root;
	switch(wchar){
		case ':':
			if (t_m == M_NAVIGATION)
				set_text_mode(M_EDIT);
			if (t_m == M_EDIT)
				set_text_mode(M_NAVIGATION);
			break;
		case 10:
		case KEY_ENTER:
			if (t_m == M_NAVIGATION){
				flip_complete_flag(root, select);
			}
			if (t_m == M_EDIT){
				if (np->goal != NULL)
					np = append_list_node(root);
				update_node_goal(np, 0, buffer);
				memset(buffer, '\0', MAX_GOAL_SIZE);
			}
			break;

		case 65:
		case KEY_UP:
			if (select > 0 && t_m == M_NAVIGATION)
				*selection = select - 1;
			
			break;

		case 66:
		case KEY_DOWN:
			if (t_m == M_NAVIGATION && return_node_at_index(root, select)->next != NULL)
				*selection = select + 1;
			
			break;

		case 127:
		case KEY_BACKSPACE:
			if (t_m == M_EDIT)
				pop_buffer(buffer);
			break;

		default:
			if (t_m == M_EDIT)
				append_to_buffer(buffer, wchar);
			break;
	}
	return 0;
}

int main()
{
	char *buffer = malloc(MAX_GOAL_SIZE);
	memset(buffer, '\0', MAX_GOAL_SIZE);
	struct lnode *root = create_list();

	int selection = 0;
	int inchar;
	
	WINDOW *win = initialize_display();
	keypad(win, TRUE);
	display_welcome(win);

	set_text_mode(M_EDIT);

	while (TRUE){
		inchar = wgetch(win);
		interpret_input(inchar, buffer, &selection, root);

		display_goal_list(win, root, selection, buffer);
	}

	return 0;

}

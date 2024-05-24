#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "headers/global.h"
#include "headers/distracto_errors.h"
#include "headers/modeswitching.h"
//#include "headers/terminal_mode.h"
#include "headers/list.h"
#include "headers/display.h"
#include "headers/config_loader.h"
#include "headers/cache.h"

// From config_loader.c
extern struct configs default_config;
extern struct termios _global_term_flag_state;
struct configs *confs;

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

// Pass the correct values to "write_to_cache"
int update_cache(struct lnode *rp)
{
	char *home_env = get_home_directory();
	char *cache_file = append_config_path(home_env, confs->cache_path.val);

	write_to_cache(rp, cache_file);
	if (cache_file != NULL)
		free(cache_file);

	return 0;
}

int cleanup_and_exit(struct lnode *rp)
{

	// If caching is enabled, update cache
	if (strcmp(confs->cache_incomplete.val, "true") == 0)
		update_cache(rp);

	endwin();
	//restore_flag_state(); only needed if terminal_mode is included
	exit(0);
	return 0;
}

int is_command(char *buffer, struct lnode **rp)
{
	if (strcmp(buffer, "exit") == 0)
		cleanup_and_exit(*rp); 
	if (strcmp(buffer, "q") == 0)
		cleanup_and_exit(*rp); 
	if (strcmp(buffer, "clear") == 0){
		//Set the pointer to root as null if destroy successful
		*rp = destroy_list(*rp);
		return TRUE;
	}
	return 0;
}

int interpret_input(int wchar, char *buffer, int *selection, struct lnode **rootp)
{
	int select = *selection;
	int t_m = current_text_mode();
	struct lnode *root = *rootp;
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
				// Check if the buffer is a command
				if (is_command(buffer, rootp)){
					//Clear buffer if command is interpreted
					memset(buffer, '\0', MAX_GOAL_SIZE);
					break;
				}
				
				if (np->goal != NULL)
					np = append_list_node(root);
				update_node_goal(np, 0, buffer);
				memset(buffer, '\0', MAX_GOAL_SIZE);
			}
			// If caching is enabled, update cache
			if (strcmp(confs->cache_incomplete.val, "true") == 0)
				update_cache(root);

			break;

		case KEY_UP:
			// If auto nav mode is true, switch to nav mode upon arrow key press
			if (strcmp(confs->auto_nav_mode.val, "true") == 0){
				set_text_mode(M_NAVIGATION);
				t_m = current_text_mode();
			}

			if (select > 0 && t_m == M_NAVIGATION)
				*selection = select - 1;
			
			break;

		case KEY_DOWN:
			// If auto nav mode is true, switch to nav mode upon arrow key press
			if (strcmp(confs->auto_nav_mode.val, "true") == 0){
				set_text_mode(M_NAVIGATION);
				t_m = current_text_mode();
			}

			if (t_m == M_NAVIGATION && return_node_at_index(root, select)->next != NULL)
				*selection = select + 1;
			
			break;

		case 127:
		case KEY_BACKSPACE:
			if (t_m == M_EDIT)
				pop_buffer(buffer);
			if (t_m == M_NAVIGATION){
				// If deleting root
				if (select == 0)
					*rootp = root->next;
				remove_list_node(root, select);
				*selection = select = 0;

				// If caching is enabled, update cache
				if (strcmp(confs->cache_incomplete.val, "true") == 0)
					update_cache(root);
			}
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
	struct lnode **rp = &root;

	confs = init_config();

	if (strcmp(confs->cache_incomplete.val, "true") == 0){
		char *home_env = get_home_directory();
		char *cache_file = append_config_path(home_env, confs->cache_path.val);
		load_from_cache(root, cache_file);
	}

	int selection = 0;
	int inchar;
	
	WINDOW *win = initialize_display();
	keypad(win, TRUE);
	display_welcome(win);

	set_text_mode(M_EDIT);

	display_goal_list(win, root, selection, buffer);

	while (TRUE){
		inchar = wgetch(win);
		interpret_input(inchar, buffer, &selection, rp);

		// If the root was deleted, update it and reinitialize if list is now empty
		root = *rp;
		if (root == NULL){
			root = create_list();
			*rp = root;
		}

		display_goal_list(win, root, selection, buffer);
	}

	return 0;

}

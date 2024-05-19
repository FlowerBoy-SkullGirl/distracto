#include <stdio.h>
#include <stdlib.h> //malloc
#include <string.h>
#include "headers/list.h"
#include "headers/distracto_errors.h"
#include "headers/global.h"

struct lnode *create_list()
{
	struct lnode *root = malloc(sizeof(struct lnode));
	if (root == NULL){
		distracto_error("Could not create list head");
		return NULL;
	}

	root->goal = NULL;
	root->complete_flag = FALSE;
	root->next = NULL;
	
	return root;
}


struct lnode *append_list_node(struct lnode *root)
{
	if (root->next == NULL){
		return root->next = create_list();
	}

	return append_list_node(root->next);
}

struct lnode *free_node_contents(struct lnode *node)
{
	if (node->goal == NULL)
		return node;

	free(node->goal);
	node->goal == NULL;
	return node;
}

struct lnode *remove_list_node(struct lnode *root, int index)
{
	struct lnode *rn = root->next;

	// If we are removing a non-root node
	if (index == 1){
		struct lnode *rnn = rn->next;
		root->next = rnn;
		if (rn == NULL)
			return NULL;
		free_node_contents(rn);
		free(rn);
		return root;
	}

	// If we are removing the root
	if (index == 0){
		if (root == NULL)
			return NULL;
		free_node_contents(root);
		free(root);
		return rn;
	}

	return remove_list_node(rn, index - 1);
}

struct lnode *update_node_goal(struct lnode *root, int index, char *src)
{
	if (index < 0 || root == NULL)
		return NULL;

	if (index == 0){
		root->goal = realloc(root->goal, MAX_GOAL_SIZE);
		if (root->goal == NULL)
			distracto_error("Goal could not be allocated");

		memset(root->goal, '\0', MAX_GOAL_SIZE);
		snprintf(root->goal, MAX_GOAL_SIZE, src);
		return root;
	}

	return update_node_goal(root->next, index - 1, src);
}

struct lnode *flip_complete_flag(struct lnode *root, int index)
{
	if (index < 0 || root == NULL)
		return NULL;

	if (index == 0){
		if (root->complete_flag == FALSE)
			root->complete_flag = TRUE;
		else
			root->complete_flag = FALSE;

		return root;
	}

	return flip_complete_flag(root->next, index - 1);
}

struct lnode *return_node_at_index(struct lnode *root, int index)
{
	if (index == 0)
		return root;

	return return_node_at_index(root->next, index - 1);
}

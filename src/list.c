#include <stdlib.h> //malloc
#include "headers/list.h"
#include "headers/distracto_erros.h"

struct lnode *create_list()
{
	struct lnode *root = malloc(sizeof(struct lnode));
	if (root == NULL){
		distracto_error("Could not create list head");
		return FUNC_ERROR;
	}
	
	return root;
}


struct lnode *append_list_node(struct lnode *root)
{
	
	return ;
}

struct lnode *remove_list_node(struct lnode *root, int index)
{

	return ;
}

struct lnode *update_node_goal(struct lnode *root, int index, char *)
{

	return ;
}

struct lnode *flip_complete_flag(struct lnode *root, int index)
{

	return ;
}

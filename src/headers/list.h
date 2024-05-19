#ifndef LIST_H
#define LIST_H

struct lnode{
	char *goal;
	int complete_flag;
	struct lnode *next;
};

struct lnode *create_list();

struct lnode *append_list_node(struct lnode *);

struct lnode *free_node_contents(struct lnode *);

struct lnode *remove_list_node(struct lnode *, int);

struct lnode *update_node_goal(struct lnode *, int, char *);

struct lnode *flip_complete_flag(struct lnode *, int);

#endif

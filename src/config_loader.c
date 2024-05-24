#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "headers/config_loader.h"
#include "headers/distracto_errors.h"
#include "headers/global.h"

struct configs default_config = {
	.config_path = {"config_path", "/.config/distracto"},
	.cache_path = {"cache_path", "/.cache/distracto"},
	.cache_incomplete = {"cache_incomplete", "true"},
	.mode_key = {"mode_key", ":"},
	.auto_nav_mode = {"auto_nav_mode", "true"},
	.del_key = {"del_key", "KEY_BACKSPACE"}
};

//Returns the OS home directory expanded from ~
char *get_home_directory()
{
	char default_home[] = "HOME=~/";
	char *homedir = getenv("HOME");
	if (homedir == NULL){
		homedir = malloc(strlen(default_home)+1);
		snprintf(homedir, strlen(default_home)+1, default_home);
	}

	return homedir;
}

// Copies an environment variable without the preceding variable name
/* Currently commented out because this is not how getenv() behaves, it delivers the expanded value only, not HOME=
char *copy_and_truncate_env(char *src)
{
	char *dest = malloc(strlen(src) +1);
	char passed_equal = FALSE;
	int start = 0;
	for (int i = 0; i <= strlen(src); i++){
		if (passed_equal)
			dest[i-start] = src[i];

		if (src[i] == '='){
			passed_equal = TRUE;
			start = i + 1;
		}
	}
	
	return dest;
}
*/

// Appends the config path to a str, used after copy_and_truncate_env to obtain an expanded file path for the config file
char *append_config_path(char *dest, char *config_path)
{
	if (dest == NULL)
		return NULL;

	char *temp = malloc(MAX_GOAL_SIZE);
	
	for (int i = 0; i < strlen(dest); i++)
		temp[i] = dest[i];
	int start = strlen(dest);

	//Additional +1 is for \0
	for (int i = start; i < start+strlen(config_path)+1; i++)
		temp[i] = config_path[i - start];

	return temp;
}

// Splits the config file entries on a '=' char and moves the lvalue to var and rvalue to val
struct config_var *extract_values_from_line(char *config_line)
{
	struct config_var *cv = malloc(sizeof(struct config_var));
	cv->var = malloc(MAX_GOAL_SIZE);
	cv->val = malloc(MAX_GOAL_SIZE);
	
	if (config_line == NULL)
		return cv;

	int passed_equals = FALSE;
	int offset = 0;
	for (int i = 0; config_line[i] != '\0'; i++)
	{
		if (config_line[i] == '='){
			passed_equals = TRUE;
			offset = i + 1;
			continue;
		}
		if (passed_equals){
			(cv->val)[i - offset] = config_line[i];
		}else{
			(cv->var)[i - offset] = config_line[i];
		}

	}
	
	// Remove newline
	if ((cv->val)[strlen(cv->val) - 1] == '\n')
		(cv->val)[strlen(cv->val) - 1] = '\0';

	return cv;
}

//Abandon faith in switch case, ye who enter here

// Parses which variable is being assigned
struct configs *assign_val(struct configs *confs, struct config_var *conf_entry)
{
	if (strcmp(conf_entry->var, default_config.config_path.var) == 0){
		confs->config_path.var = default_config.config_path.var;
		confs->config_path.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->config_path.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	if (strcmp(conf_entry->var, default_config.cache_path.var) == 0){
		confs->cache_path.var = default_config.cache_path.var;
		confs->cache_path.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->cache_path.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	if (strcmp(conf_entry->var, default_config.cache_incomplete.var) == 0){
		confs->cache_incomplete.var = default_config.cache_incomplete.var;
		confs->cache_incomplete.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->cache_incomplete.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	if (strcmp(conf_entry->var, default_config.mode_key.var) == 0){
		confs->mode_key.var = default_config.mode_key.var;
		confs->mode_key.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->mode_key.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	if (strcmp(conf_entry->var, default_config.auto_nav_mode.var) == 0){
		confs->auto_nav_mode.var = default_config.auto_nav_mode.var;
		confs->auto_nav_mode.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->auto_nav_mode.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	if (strcmp(conf_entry->var, default_config.del_key.var) == 0){
		confs->del_key.var = default_config.del_key.var;
		confs->del_key.val = malloc(MAX_GOAL_SIZE);
		snprintf(confs->del_key.val, MAX_GOAL_SIZE, conf_entry->val);
	}

	return confs;
}

// Assigns any null values to defaults
struct configs *pad_empty_config(struct configs *confs)
{
	if (confs->config_path.val == NULL){
		confs->config_path.var = default_config.config_path.var;
		confs->config_path.val = default_config.config_path.val;
	}

	if (confs->cache_path.val == NULL){
		confs->cache_path.var = default_config.cache_path.var;
		confs->cache_path.val = default_config.cache_path.val;
	}

	if (confs->cache_incomplete.val == NULL){
		confs->cache_incomplete.var = default_config.cache_incomplete.var;
		confs->cache_incomplete.val = default_config.cache_incomplete.val;
	}

	if (confs->mode_key.val == NULL){
		confs->mode_key.var = default_config.mode_key.var;
		confs->mode_key.val = default_config.mode_key.val;
	}

	if (confs->auto_nav_mode.val == NULL){
		confs->auto_nav_mode.var = default_config.auto_nav_mode.var;
		confs->auto_nav_mode.val = default_config.auto_nav_mode.val;
	}

	if (confs->del_key.val == NULL){
		confs->del_key.var = default_config.del_key.var;
		confs->del_key.val = default_config.del_key.val;
	}

	return confs;
}

// Initializes configuration using the config file
struct configs *init_config()
{
	struct configs *cp = malloc(sizeof(struct configs));
	struct config_var *conf_entry;

	char *home_env = get_home_directory();
	char *config_file = append_config_path(home_env, default_config.config_path.val);
	if (config_file != NULL){
		default_config.config_path.val = config_file;
	}	

	FILE *fp = fopen(default_config.config_path.val, "r");
	if (fp == NULL){
		distracto_error("Unable to open config file");
		return cp = &default_config;
	}

	char *conf_line = malloc(MAX_GOAL_SIZE);
	if (conf_line == NULL)
		return cp = &default_config;

	while(fgets(conf_line , MAX_GOAL_SIZE, fp)){
		conf_entry = extract_values_from_line(conf_line);
		if (conf_entry == NULL)
			continue;
		cp = assign_val(cp, conf_entry);
	}

	cp = pad_empty_config(cp);

	return cp;
}

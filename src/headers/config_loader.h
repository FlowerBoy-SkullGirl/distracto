#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#define DEFAULT_CONFIG_PATH = ".config/distracto/distractorc"

struct config_var{
	char *var;
	char *val;
};

struct configs{
	struct config_var config_path;
	struct config_var cache_path;
	struct config_var cache_incomplete;
	struct config_var mode_key;
	struct config_var auto_nav_mode;
	struct config_var del_key;
};

char *get_home_directory();

char *copy_and_truncate_env(char *);

char *append_config_path(char *, char *);

struct config_var *extract_values_from_line(char *);

struct configs *assign_val(struct configs *, struct config_var *);

struct configs *pad_empty_config(struct configs *);

struct configs *init_config();


#endif

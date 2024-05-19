#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#define DEFAULT_CONFIG_PATH = ".config/distracto/distractorc"

struct config_var{
	char *var;
	char *val;
}

char *get_home_directory();

char *copy_and_truncate_env(char *);

char *append_config_path(char *);

struct config_var *extract_values_from_line(char *);


#endif

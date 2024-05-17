#ifndef GLOBAL_H
#define GLOBAL_H

#define FALSE 0
#define TRUE 1

// Used for text_modesetting
extern int _global_text_mode;

// Used for terminal_mode
extern struct termios _global_term_flag_state;

#endif

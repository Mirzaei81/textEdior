#include <sys/ioctl.h>
#include <termios.h>
#include "./gapbuffer.h"


#define  MAX_BUFFLENGTH 255
#define CTRL_KEY(k) ((k) & 0x1f)
#define uint_16_t fa_char

struct term_config{
    struct winsize winsize;
    struct  termios orig_termios;
    struct  gap_buffer buffer;
};

void disable_raw_mode();
void die(const char *s);
void set_winsize();
void add_tildy();
void enable_raw_mode();
void disable_raw_mode();
void refresh_screen();
void process_key_press(char *c);


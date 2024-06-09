//NOTE: Include files
#include <ctype.h>
#include <locale.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include "../Include/foster.h"
#include "../Include/str_util.h"

char* editor = " Foster - Fa-En text editor ";
char* editorFA = "فا ستر ویرا ستار eng text برای دو زبانه ها اینگلیسی فارسی";
char* author = "by Amir arshia Mirzaei.";
char* authorFA = "توسعه یافته شده توسط امیرعرشیا میرزایی";


bool is_main_menu = true;
enum COLORS {
    Black=0,
    Red,
    Green,
    Yellow,
    Blue,
    Purple,
    Cyan,
    White,
};
struct term_config T={.buffer={0,0,10,0}};

// NOTE: Terminal Stuff

void die(const char *s){
    buffer_insert(&T.buffer,"\033[2J\r", 4);
    buffer_insert(&T.buffer,"\x1b[H", 3);
    buffer_insert(&T.buffer,"\033[0m", 4);
    (void)(write(STDOUT_FILENO, T.buffer.buff, T.buffer.length)+1);
    perror(s);
    exit(1);
}
void change_color(int color){
    // INFO: Ansi Color start at 30
    char* ansi_color = malloc(20);
    sprintf(ansi_color,"\x1b[0;%dm",color+30);
    buffer_insert(&T.buffer, ansi_color,strlen(ansi_color));
    free(ansi_color);
}
void reset_color(){
    buffer_insert(&T.buffer, "\x1b[0m",4);
}
void set_winsize(){
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ,&T.winsize)==-1||T.winsize.ws_col==0){
        /*
         * INFO: instead of using we will go \x1b[999;999H beacuse let's not go out of bound
         * goes to right ( C ) and down ( B ) 999 char
        */
        buffer_insert(&T.buffer,"\x1b[999C\x1b[999B",12);
        die("get_winsize");
    }
}

void add_tildy(){
    buffer_insert(&T.buffer,"\x1b[H", 3);// go to home position for the insert of tildy
    change_color(Yellow);
    for (unsigned short i=0;i<T.winsize.ws_col;i++) {
        buffer_insert(&T.buffer,"\x1b[K~", 4);
        if(i<T.winsize.ws_col-1){
            buffer_insert(&T.buffer,"\r\n", 2);
        }
    }
    reset_color();
    buffer_insert(&T.buffer,"\033[H", 3);
}
void  enable_raw_mode(){
    setlocale(LC_CTYPE,"");
    if(tcgetattr(STDIN_FILENO, &T.orig_termios)==-1)die("tcgetattr");
    struct termios raw= T.orig_termios;

    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    //raw.c_cc[VMIN]=0; // how many bytes should enter till read
    //raw.c_cc[VTIME]=1; //Stop for 1/10 of second for the next render if nothing puts return 0

    /*
     * INFO: disable echo,ICANON Mode (reading input byte by byte instead of every batch after byte 10 Enterd ;)
     * disable intrupt signals ctrl+z ctrl+c
    */
    raw.c_lflag &= ~(ECHO|ICANON|ISIG|IEXTEN);
    ioctl(STDIN_FILENO, IUTF8,1);

    if(tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw))die("tcsetattr");
}

void disable_raw_mode(){
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&T.orig_termios);
    free(T.buffer.buff);
}
void disp_main_menu(){
        // INFO:  position the curosr to third of screen
        char* editor_pos = malloc(20);
        sprintf(editor_pos,"\x1b[%d;%dH",T.winsize.ws_row/3,(int)(T.winsize.ws_col/2-(strlen(editor)/2)));
        buffer_insert(&T.buffer,editor_pos,strlen(editor_pos));
        buffer_insert(&T.buffer,editor,strlen(editor));

        // INFO:  position the curosr after FA Editor  and write
        char* editorFA_pos = malloc(20);
        sprintf(editorFA_pos,"\x1b[%d;%dH",T.winsize.ws_row/3+2,(int)(T.winsize.ws_col/2-(strlen(editorFA)/4)));
        buffer_insert(&T.buffer,editorFA_pos,strlen(editorFA_pos));
        buffer_insert(&T.buffer,editorFA,strlen(editorFA));


        // INFO:  position the curosr after the Latin Author
        char* author_pos = malloc(20);
        sprintf(author_pos,"\x1b[%d;%dH",(T.winsize.ws_row/3)+4,(int)(T.winsize.ws_col/2-(strlen(author)/2)));
        buffer_insert(&T.buffer,author_pos,strlen(author_pos));
        buffer_insert(&T.buffer,author,strlen(author));

        // INFO:  position the curosr after Latin author and write
        char* authorFA_pos = malloc(20);
        sprintf(authorFA_pos,"\x1b[%d;%dH",T.winsize.ws_row/3+6,(int)(T.winsize.ws_col/2-(strlen(authorFA)/4)));
        buffer_insert(&T.buffer,authorFA_pos,strlen(editorFA_pos));

        // INFO:  position the curosr after Fa author and write
        char* author_reversed = reverse_string(authorFA,' ');
        buffer_insert(&T.buffer,author_reversed,strlen(author_reversed));

        free(editor_pos);
        free(author_pos);
        free(editorFA_pos);
        free(authorFA_pos);
        free(author_reversed);
}

// INFO: Input Processing
void refresh_screen(){
    if (is_main_menu){
        buffer_insert(&T.buffer, "\x1b[?25l", 7);//clear Cursor
        add_tildy();
        disp_main_menu();
    }
    if(write(STDOUT_FILENO,T.buffer.buff, T.buffer.length)==-1)die("writeBuffer");
    buffer_free(&T.buffer); // clear the screan
}

void process_key_press(char *c){
    bool normal_mode=false;
    if(normal_mode){
        switch (*c) {
            case 0x69: // hex code for i goes to insert mode
                normal_mode=false;
                break;
            case CTRL_KEY('q'):
                buffer_insert(&T.buffer,"\x1b[2J", 4);
                buffer_insert(&T.buffer, "\x1b[?25h", 6);//Show Cursor
                buffer_insert(&T.buffer,"\x1b[H",3);
                exit(0);
                break;
            default:
                break;
        }
    }
    else{
        //if(*c==0x1b){ // any scape charecter
        //    normal_mode=true;
        //}
        if(*c==CTRL_KEY('q')){
            buffer_insert(&T.buffer,"\x1b[2J\r", 4);
            buffer_insert(&T.buffer, "\x1b[?25h", 6);//Show Cursor
            buffer_insert(&T.buffer,"\x1b[H",3);
            buffer_insert(&T.buffer,"\033[0m",4);
            if(write(STDOUT_FILENO,T.buffer.buff, T.buffer.length)==-1)die("writeBuffer");
            exit(0);
        }
        // TODO: Handle all CTRL KEY
        if(iscntrl(*c)){
            //buffer_insert(&T.buffer,&c,1);
        }
        else{
            if(is_main_menu){
                is_main_menu=false;
                buffer_insert(&T.buffer,"\x1b[2J", 4);//clear the screen
                buffer_insert(&T.buffer,"\x1b[H", 3);//go Home
                if(write(STDOUT_FILENO,T.buffer.buff, T.buffer.length)==-1)die("writeBuffer");
                buffer_free(&T.buffer);
            }
        }
    }
    refresh_screen();
}

// INFO: Main
int main(){
    enable_raw_mode();
    set_winsize();
    atexit(disable_raw_mode);
    char *c=malloc(2);
    refresh_screen();
    if(write(STDOUT_FILENO,T.buffer.buff, T.buffer.length)==-1)die("writeBuffer");

    while (1){
        //In cygwin throws -1 instead of zero if VTIME is set so wecheck for EAGAIN
        if(read(STDIN_FILENO,c,2)== -1 && errno!=EAGAIN )die("readchar");
        process_key_press(c);
    }
    return 0;
}

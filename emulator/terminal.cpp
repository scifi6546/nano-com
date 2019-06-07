#include <ncurses.h>
#include <unistd.h>
#include "terminal.hpp"
void init_term(){
    WINDOW *tep = initscr();
    clear();
    refresh();
}
void set_char(char pos_x, char pos_y, char code){
    mvaddch(pos_y,pos_x,code);
    refresh();
}
void end_term(){
    endwin();
}
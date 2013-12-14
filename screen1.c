#include <unistd.h>
#include <curses.h>
#include <stdlib.h>
#define LOCAL_ESCAPE_KEY 27
int main()
{
    int key;
    initscr();
    crmode();
    keypad(stdscr, TRUE);
    noecho();
    clear();
    mvprintw(5, 5, " gKey pad demonstration. Press ‘q’ to quit");
    move(7, 5);
    refresh();
    key = getch();
    while(key != ERR && key != 'q') {
    move(7, 5);
    clrtoeol();
    if ((key >= 'A' && key <= 'Z') ||
    (key >= 'a' && key <= 'z')) {
        printw(" gKey was %c", (char)key);
    }
    else {
        switch(key) {
        case LOCAL_ESCAPE_KEY: printw(" g%s", "Escape key"); break;
        case KEY_END: printw(" g%s”, “END key"); break;
        case KEY_BEG: printw(" g%s”, “BEGINNING key"); break;
        case KEY_RIGHT: printw(" g%s”, “RIGHT key"); break;
        case KEY_LEFT: printw(" g%s”, “LEFT key"); break;
        case KEY_UP: printw(" g%s”, “UP key"); break;
        case KEY_DOWN: printw(" g%s”, “DOWN key"); break;
        default: printw(" gUnmatched - %d", key); break;
    } /* switch */
} /* else */
        refresh();
        key = getch();
} /* while */
    endwin();
    exit(EXIT_SUCCESS);
}


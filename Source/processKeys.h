#include "header.h"

//***********************************************************//
//this file process the input buffer associated with the keys and
// return a single character associated with each key press.
//***********************************************************//
int kbhit(void){
    int c = 0;
    struct termios old_tio, new_tio;
    tcgetattr(0, &old_tio);
    memcpy(&new_tio, &old_tio, sizeof(new_tio));
    new_tio.c_lflag &= ~(ICANON | ECHO);
    new_tio.c_cc[VMIN] = 0;
    new_tio.c_cc[VTIME] = 1;
    tcsetattr(0, TCSANOW, &old_tio);
    c = getchar();
    tcsetattr(0, TCSANOW, &old_tio);
    if (c != -1) ungetc(c, stdin);
    return ((c != -1) ? 1 : 0);
}
int getch(void){
    int ch;
    struct termios oldt;
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= static_cast<unsigned int>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
int kbesc(void){
    int c;
    if (!kbhit()) return KEY_ESCAPE;
    c = getch();
    if (c == '[') {
        switch (getch()) {
            case 'A':
                c = KEY_UP;
                break;
            case 'B':
                c = KEY_DOWN;
                break;
            case 'C':
                c = KEY_LEFT;
                break;
            case 'D':
                c = KEY_RIGHT;
                break;
            default:
                c = 0;
                break;
        }
    } else {
        c = 0;
    }
    if (c == 0){ while (kbhit()) getch();};
    return c;
}
int kbget(void)
{
    int c;
    c = getch();
    return (c==KEY_ESCAPE)? kbesc() :c;
}


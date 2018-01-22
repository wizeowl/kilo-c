#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    struct termios raw = orig_termios;
    // c_lflag: local flags
    // disable ECHO
    // disable ICANON (so we can read char by char instead of wait for enter)
    // disable ISIG for ctrl-c and ctrl-v
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    // disable ctrl-s(suspend output) and ctrl-q(resume output)
    raw.c_lflag &= ~(IXON);


    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
    enableRawMode();

    char c;
    // read one char for standard input into c
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
        if (iscntrl(c)/*form <ctype.h>, is this a control character?*/) {
            printf/*from <stdio.h>*/("%d\n", c);
        } else {
            printf("%d ('%c')\n", c, c);
        }
    }

    return 0;
}
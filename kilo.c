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
    // disable ISIG for ctrl-c(SIGINT) and ctrl-z(SIGSTP)
    // disable ctrl-v(IEXTEN)
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    // disable ctrl-s(suspend output) and ctrl-q(resume output)
    // disable ICRNL input flag carriage return new line
    raw.c_lflag &= ~(ICRNL | IXON);
    // turn off output processing,
    raw.c_lflag &= ~(OPOST);

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
            // the \r is for carriage return
            printf("%d ('%c')\r\n", c, c);
        }
    }

    return 0;
}
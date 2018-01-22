/*** includes ***/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

/*** defines ***/

#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/
struct termios orig_termios;

/*** terminal ***/
void die(const char *s) {
    perror(s);
    exit(1);
}

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
        die("tcsetattr");
    }
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) {
        die("tcsetattr");
    }
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
    raw.c_lflag &= ~(BRKINT | INPCK | ISTRIP);
    raw.c_lflag |= (CS8);

    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    }
}

char editorReadKey() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != -1) {
        if (nread == -1 && errno != EAGAIN) die("read");
    }
    return c;
}

/*** input ***/

void editorProcessKeypress() {
    char c = editorReadKey();
    switch (c) {
        case CTRL_KEY('q'):
            exit(0);
            break;
    }
}

/*** init ***/
int main() {
    enableRawMode();

    // read one char for standard input into c
    while (1) {
        editorProcessKeypress();
    }

    return 0;
}
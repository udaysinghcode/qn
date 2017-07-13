#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

void stopRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void startRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(stopRawMode);
  // read attributes into struct
  struct termios raw = orig_termios;
  // modify local flags by flipping fourth bit to not print typing / quit at q
  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);
  // apply to terminal
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); 
}

int main() {
  startRawMode();
  char c;
  // Read input into char c and quit on reading q
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    // iscntrl tests whether character is a control character to determine
    // printing.
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      // %d tells it to format the byte as decimal number and %c tells to write
      // out byte as character 
      printf("%d ('%c')\n", c, c);
    }
  }
  return 0;
}

#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>

struct termios orig_termios;

// Print error function using and use perror to print descriptive error message
void die(const char *s) {
  perror(s);
  exit(1);
}

void stopRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void startRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(stopRawMode);
  // read attributes into struct
  struct termios raw = orig_termios;
  // modify local flags by flipping fourth bit to not print typing / quit at q
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP |  IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG);
  // Set minimum and maximum for control characters including minimum number of
  // bytes that are read and the maximum amount of time to wait before a read()
  // returns.
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
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
      printf("%d\r\n", c);
    } else {
      // %d tells it to format the byte as decimal number and %c tells to write
      // out byte as character 
      printf("%d ('%c')\r\n", c, c);
    }
  }
  return 0;
}

#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

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
  raw.c_lflag &= ~(ECHO | ICANON);
  // apply to terminal
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw); 
}
int main() {
  startRawMode();
  char c;
  // Read input into char c and quit on reading q
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
  return 0;
}

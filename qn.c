#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/*** data ***/
struct termios orig_termios;

/*** raw process ***/
// Print error function using and use perror to print descriptive error message
// take parameter to list alongside error so people know
void die(const char *s) {
  perror(s);
  exit(1);
}

void stopRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void startRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    die("tcgetattr");
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
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

// This should wait for one keypress and return the keypress upon it being
// handled. We'll later grow it to cover escape sequences, which involve reading
// more than one byte like arrow keys.

char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die ("read");
  }
  return c;
}

/*** input ***/

// Create function for waiting for keypress and handling it. Later we can
// map various Ctrl key combinations and other special keys to different editor
// functions.
void editorProcessKeypress() {
  char c = editorReadKey();

  switch (c) {
    case CTRL_KEY('q'):
      exit (0);
      break;
  }
}

/*** output ***/

// 4 implies we write out 4 bytes to the terminal. \x1b is the escape character.
// Remember we are using VT100 commands. We are going to use the H command to
// reposition the cursor.
// Now we can start the text editor at the top left.
void editorRefreshScreen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}


/*** initialize ***/
int main() {
  startRawMode();

  while(1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }
  // char c;
  // // Read input into char c and quit on reading q
  // while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
  //   // iscntrl tests whether character is a control character to determine
  //   // printing.
  //   if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
  //     die("read");
  //   if (iscntrl(c)) {
  //     printf("%d\r\n", c);
  //   } else {
  //     // %d tells it to format the byte as decimal number and %c tells to write
  //     // out byte as character
  //     printf("%d ('%c')\r\n", c, c);
  //   }
  //   if (c == CTRL_KEY('q')) break;
  // }
  return 0;
}

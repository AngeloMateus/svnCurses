#import "svn_curses.h"


void printCursorPosition(WINDOW *p_cursorWin){
    wclear(p_cursorWin);
    mvwprintw(p_cursorWin, State::cursorPosition, 2, "-");
    wrefresh(p_cursorWin);
}

void keyEvent() {
  int c = getch();
  switch (c) {
  case 'q':
  case 'Q':
    State::isRunning = false;
    break;
  case 'j':
    State::cursorPosition = State::cursorPosition + 1;
    break;
  case 'k':
    if (State::cursorPosition > 0) {
      State::cursorPosition = State::cursorPosition - 1;
    }
    break;
  default:
    return;
  }
  return;
}

string exec(const char *cmd) {
  array<char, 128> buffer;
  string result;
  unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  return result;
}

void printStatus(WINDOW *p_win) {
  wclear(p_win);
  string execResult = exec("svn st");

  split(State::statusItems, execResult, boost::is_any_of("\n"));
  for (int i = 0; i < State::statusItems.size(); i++) {

    switch (State::statusItems[i].front()) {
    case ' ':
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      break;
    case 'M':
      wattron(p_win, COLOR_PAIR(1));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(1));
      break;
    case '?':
      wattron(p_win, COLOR_PAIR(3));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(3));
      break;
    case '!':
      wattron(p_win, COLOR_PAIR(2));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(2));
      break;
    case 'A':
      wattron(p_win, COLOR_PAIR(4));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(4));
      break;
    case 'D':
      wattron(p_win, COLOR_PAIR(4));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(4));
      break;
    default:
      /* wprintw(p_win, (State::statusItems[i] + "\n").c_str()); */
      wprintw(p_win,
              ((string() + State::statusItems[i].front()) + "\n").c_str());
    }
  }

  refresh();
}

int main() {
  State::isRunning = true;
  WINDOW *win;
  WINDOW *cursorWin;
  initscr();
  cbreak();
  timeout(1000);
  keypad(stdscr, true);
  start_color();
  use_default_colors();
  curs_set(0);
  noecho();

  init_pair(1, COLOR_BLUE, -1);
  init_pair(2, COLOR_RED, -1);
  init_pair(3, COLOR_YELLOW, -1);
  init_pair(4, COLOR_GREEN, -1);
  init_pair(5, COLOR_MAGENTA, -1);

  int maxX = 0, maxY = 0;
  getmaxyx(stdscr, maxY, maxX);
  win = newwin(maxY, maxX - 4, 0, 4);
  cursorWin = newwin(maxY, 4, 0, 0);

  wprintw(win, "...");
  wrefresh(win);
  printStatus(win);
  printCursorPosition(cursorWin);
  wrefresh(win);

  while (State::isRunning == true) {
    keyEvent();
    printCursorPosition(cursorWin);
    /* refresh(); */
    /* wrefresh(win); */
  }

  endwin();
  return 0;
}

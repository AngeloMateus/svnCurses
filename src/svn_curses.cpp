#import "svn_curses.h"

void printStatusWithColor(WINDOW *p_win, int i) {

  if (i == State::cursorPosition) {
    wattron(p_win, COLOR_PAIR(6));
    wprintw(p_win, (State::statusItems[i] + "\n").c_str());
    wattroff(p_win, COLOR_PAIR(6));
  } else {
    int colorPair;
    switch (State::statusItems[i].front()) {
    case ' ':
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      return;
      break;
    case 'M':
      colorPair = 1;
      break;
    case '?':
      colorPair = 3;
      break;
    case '!':
      colorPair = 2;
      break;
    case 'A':
      colorPair = 4;
      break;
    case 'D':
      colorPair = 4;
      break;
    default:
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      return;
      /* wprintw(p_win, */
      /* ((string() + State::statusItems[i].front()) + "\n").c_str()); */
    }

    wattron(p_win, COLOR_PAIR(colorPair));
    wprintw(p_win, (State::statusItems[i] + "\n").c_str());
    wattroff(p_win, COLOR_PAIR(colorPair));
  }
}

void printCursorPosition(WINDOW *p_win) {
  printStatusWithColor(p_win, State::cursorPosition - 1);
  printStatusWithColor(p_win, State::cursorPosition);
  printStatusWithColor(p_win, State::cursorPosition + 1);
  wrefresh(p_win);
}

/* void printSelectedInCursorPosition(WINDOW *p_cursorWin) { */
/*   wclear(p_cursorWin); */
/*   mvwprintw(p_cursorWin, State::cursorPosition, 2, "-"); */
/*   wrefresh(p_cursorWin); */
/* } */

void printAllStatus(WINDOW *p_win) {
  wclear(p_win);
  int lines = getmaxy(p_win);
  int min =
      lines < State::statusItems.size() ? lines : State::statusItems.size();
  for (int i = 0; i < min ; i++) {

    // find selectedItems in statusItems
    /* vector<string>::iterator it = */
    /*     find(State::selectedItems.begin(), State::selectedItems.end(), */
    /*          State::statusItems[i]); */
    /* if (it != State::selectedItems.end()) { */
    /*   wprintw(p_win, " "); */
    /* } */

    /* usleep(25000); */
    printStatusWithColor(p_win, i);
  }
  wrefresh(p_win);
  refresh();
}

void keyEvent(WINDOW *p_win) {
  int c = getch();
  switch (c) {
  case 'q':
  case 'Q':
    State::isRunning = false;
    break;
  case 'j':
    if (State::cursorPosition < getmaxy(p_win) - 1) {
      State::cursorPosition = State::cursorPosition + 1;
      printAllStatus(p_win);
    }
    break;
  case 'k':
    if (State::cursorPosition > 0) {
      State::cursorPosition = State::cursorPosition - 1;
      printAllStatus(p_win);
    }
    break;
  case ' ':
    State::selectedItems.push_back(State::selectedItems[State::cursorPosition]);
    break;
  default:
    return;
  }
  return;
}

string exec(const char *cmd) {
  array<char, 256> buffer;
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

int main() {
  try {
    initscr();
    State::isRunning = true;
    WINDOW *win;
    cbreak();
    timeout(1000);
    keypad(stdscr, TRUE);
    start_color();
    use_default_colors();
    curs_set(0);
    noecho();

    init_pair(1, COLOR_BLUE, -1);
    init_pair(2, COLOR_RED, -1);
    init_pair(3, COLOR_YELLOW, -1);
    init_pair(4, COLOR_GREEN, -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_BLACK, COLOR_WHITE);

    int maxX = 0, maxY = 0;
    getmaxyx(stdscr, maxY, maxX);
    win = newwin(maxY - 2, maxX, 0, 0);

    string execResult = exec("svn st");
    split(State::statusItems, execResult, boost::is_any_of("\n"));

    wrefresh(win);
    refresh();
    printAllStatus(win);

    while (State::isRunning == true) {
      keyEvent(win);
      /* printCursorPosition(win); */
      /* wrefresh(win); */
      /* refresh(); */
    }

    endwin();

    return 0;
  } catch (const exception &e) {
    cout << e.what();
    endwin();

    return 1;
  }
}

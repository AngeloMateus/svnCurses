#import "svn_curses.h"

void printStatusWithColor(WINDOW *p_win, int i) {
  int padSize = State::maxX - State::statusItems[i].size();

  // find selectedItems in statusItems
  vector<string>::iterator it =
      find(State::selectedItems.begin(), State::selectedItems.end(),
           State::statusItems[i]);
  if (it != State::selectedItems.end()) {
  }

  if (i == State::cursorPosition) {
    wattron(p_win, COLOR_PAIR(6));
    wprintw(p_win, (State::statusItems[i]).append(padSize, ' ').c_str());
    wattroff(p_win, COLOR_PAIR(6));
  } else {
    int colorPair;
    switch (State::statusItems[i].front()) {
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
      colorPair = 5;
      break;
    default:
      colorPair = 8;
      /* wprintw(p_win, */
      /* ((string() + State::statusItems[i].front()) + "\n").c_str()); */
    }

    wattron(p_win, COLOR_PAIR(colorPair));
    /* wprintw(p_win, (State::statusItems[i] + "\n").c_str()); */

    wprintw(p_win, (State::statusItems[i]).append(padSize, ' ').c_str());
    wattroff(p_win, COLOR_PAIR(colorPair));
  }
}

/* void printSelectedInCursorPosition(WINDOW *p_cursorWin) { */
/*   wclear(p_cursorWin); */
/*   mvwprintw(p_cursorWin, State::cursorPosition, 2, "-"); */
/*   wrefresh(p_cursorWin); */
/* } */

void printAllStatus(WINDOW *pad) {
  wclear(pad);
  for (int i = State::padPos; i < State::statusItems.size(); i++) {

    /* usleep(25000); */
    printStatusWithColor(pad, i);
  }
  /* prefresh(pad, 0, 0, 0, 0, */
  /*          State::maxY + State::cursorPosition, State::maxX); */
  /* State::padPos++; */
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

void refreshStatusItems(WINDOW *pad, WINDOW *win) {

  mvwprintw(win, State::maxY - 1, 0, "...");
  doupdate();
  wrefresh(win);

  string execResult = exec("svn st");
  // TESTING
  /*for (int i = 0; i < 200; i++) {
    State::statusItems.push_back(to_string(i));
  }
  */

  execResult.pop_back();
  split(State::statusItems, execResult, boost::is_any_of("\n"));
  refresh();
  printAllStatus(pad);
  // pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol
  mvwprintw(win, State::maxY - 1, 0, "   ");
  doupdate();
  wrefresh(win);
  prefresh(pad, 0, 0, 0, 0, State::maxY - 2, State::maxX);
}

void keyEvent(WINDOW *pad, WINDOW *win) {
  int c = getch();
  switch (c) {
  case 'q':
  case 'Q':
    State::isRunning = false;
    break;
  case 'j':
    if (State::cursorPosition < State::statusItems.size() - 1) {
      State::cursorPosition++;
      if (State::cursorPosition >= State::maxY - 2 + State::padPos) {
        State::padPos++;
      }
    }
    printAllStatus(pad);
    prefresh(pad, 0, 0, 0, 0, State::maxY - 2, State::maxX);
    break;
  case 'k':
    if (State::cursorPosition > 0) {
      State::cursorPosition--;

      if (State::cursorPosition <= State::padPos) {
        State::padPos = max(0, State::padPos - 1);
      }
    }
    printAllStatus(pad);
    prefresh(pad, 0, 0, 0, 0, State::maxY - 2, State::maxX);
    break;
  case 'G':
    if (State::cursorPosition < State::statusItems.size() - 1) {
      State::cursorPosition = State::statusItems.size() - 1;
      State::padPos = State::statusItems.size() + 2 - State::maxY;
    }
    printAllStatus(pad);
    prefresh(pad, 0, 0, 0, 0, State::maxY - 2, State::maxX);
    break;
  case 'g':
    State::cursorPosition = 0;
    State::padPos = 0;
    printAllStatus(pad);
    prefresh(pad, 0, 0, 0, 0, State::maxY - 2, State::maxX);
    break;
  case 'r':
    State::cursorPosition = 0;
    State::padPos = 0;
    refreshStatusItems(pad, win);
  case ' ':
    State::selectedItems.push_back(State::statusItems[State::cursorPosition]);
    break;
  default:
    return;
  }
  return;
}

int main() {
  try {
    initscr();
    State::isRunning = true;
    WINDOW *win;
    WINDOW *pad;
    cbreak();
    timeout(1000);
    keypad(stdscr, TRUE);
    mouseinterval(0);
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
    init_pair(7, COLOR_BLUE, COLOR_WHITE);
    init_pair(8, -1, -1);

    getmaxyx(stdscr, State::maxY, State::maxX);
    win = newwin(State::maxY, State::maxX, 0, 0);
    pad = newpad(State::maxY - 2, State::maxX);
    if (pad == NULL) {
      endwin();
      return printf("ERROR: Couldn't create pad\n");
    }

    refreshStatusItems(pad, win);

    while (State::isRunning == true) {
      keyEvent(pad, win);
      /* wrefresh(win); */
    }

    delwin(pad);
    delwin(win);
    endwin();

    return 0;
  } catch (const exception &e) {
    endwin();
    std::cerr << "Exception caught : " << e.what() << std::endl;
    return 1;
  }
}

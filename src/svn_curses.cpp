#include "svn_curses.h"

int bottomWindowSize = 8;

void printStatusWithColor(WINDOW *p_win, int i) {
  int padSize = State::maxX - State::statusItems[i].size();

  // find selectedItems in statusItems
  bool isItemSelected =
      find(State::selectedItems.begin(), State::selectedItems.end(),
           State::statusItems[i]) != State::selectedItems.end();

  if (i == State::cursorPosition) {
    wattron(p_win, COLOR_PAIR(6));
    wprintw(p_win, (State::statusItems[i]).append(padSize, ' ').c_str());
    wattroff(p_win, COLOR_PAIR(6));
    // print rest of statusItems
  } else if (isItemSelected) {
    wattron(p_win, A_BOLD);
    wattron(p_win, COLOR_PAIR(8));
    wprintw(p_win, State::statusItems[i].c_str());
    wattroff(p_win, COLOR_PAIR(8));
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

void printAllStatus(WINDOW *pad) {
  wclear(pad);
  for (int i = State::padPos; i < State::statusItems.size(); i++) {
    /* usleep(25000); */
    printStatusWithColor(pad, i);
  }
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
  State::selectedItems.clear();
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
  State::allStatusItems = State::statusItems;

  refresh();
  printAllStatus(pad);
  // pminrow, pmincol, sminrow, smincol, smaxrow, smaxcol
  mvwprintw(win, State::maxY - 1, 0, "   ");
  doupdate();
  wrefresh(win);
  prefresh(pad, 0, 0, 0, 0, State::maxY - bottomWindowSize, State::maxX);
}

void moveOneRowUp(WINDOW *pad) {
  if (State::cursorPosition > 0) {
    State::cursorPosition--;

    if (State::cursorPosition <= State::padPos) {
      State::padPos = max(0, State::padPos - 1);
    }
  }
  printAllStatus(pad);
  prefresh(pad, 0, 0, 0, 0, State::maxY - bottomWindowSize, State::maxX);
}

void moveOneRowDown(WINDOW *pad) {
  if (State::cursorPosition < State::statusItems.size() - 1) {
    State::cursorPosition++;
    if (State::cursorPosition >=
        State::maxY - bottomWindowSize + State::padPos) {
      State::padPos++;
    }
  }
  printAllStatus(pad);
  prefresh(pad, 0, 0, 0, 0, State::maxY - bottomWindowSize, State::maxX);
}

void keyEvent(WINDOW *pad, WINDOW *win) {
  int c = getch();
  switch (c) {
  case 'q':
  case 'Q':
    State::isRunning = false;
    break;
  case 'j':
    moveOneRowDown(pad);
    break;
  case 'k':
    moveOneRowUp(pad);
    break;
  case 'G':
    if (State::cursorPosition < State::statusItems.size() - 1) {
      if (State::maxY < State::statusItems.size() - 1) {
        State::padPos =
            State::statusItems.size() - State::maxY + bottomWindowSize;
      }
      State::cursorPosition = State::statusItems.size() - 1;
      /* State::padPos = State::statusItems.size() + 2 - State::maxY; */
    }
    printAllStatus(pad);
    prefresh(pad, 0, 0, 0, 0, State::maxY - bottomWindowSize, State::maxX);
    break;
  case 'g':
    if (getch() == 'g') {
      State::cursorPosition = 0;
      State::padPos = 0;
      printAllStatus(pad);
      prefresh(pad, 0, 0, 0, 0, State::maxY - bottomWindowSize, State::maxX);
    }
    break;
  case 's':
    State::cursorPosition = 0;
    State::padPos = 0;
    refreshStatusItems(pad, win);
    break;
  case 'a': {
    vector<string> items = State::selectedItems;
    for_each(items.begin(), items.end(), [](string &str) { str.erase(0, 7); });
    wclear(win);
    wrefresh(win);
    mvwprintw(win, bottomWindowSize - 1, 0, "Add selected items? (y/n?)");
    wrefresh(win);
    timeout(12000);
    if (getch() == 'y') {
      wclear(win);
      wrefresh(win);
      string joinedStr = boost::algorithm::join(items, " ");
      string result = exec(("svn add " + joinedStr).c_str());

      wprintw(win, result.c_str());
      wrefresh(win);
      getch();
    }
    refreshStatusItems(pad, win);
    wclear(win);
    wrefresh(win);
    break;
  }
  case 'r': {
    vector<string> items = State::selectedItems;
    for_each(items.begin(), items.end(), [](string &str) { str.erase(0, 7); });
    wclear(win);
    wrefresh(win);
    mvwprintw(win, bottomWindowSize - 1, 0, "Delete selected items? (y/n?)");
    wrefresh(win);
    timeout(12000);
    string keepLocal = "";
    if (getch() == 'y') {
      wclear(win);
      wrefresh(win);
      mvwprintw(win, bottomWindowSize - 1, 0, "Keep local files? (y/n?)");
      wrefresh(win);
      if (getch() == 'y') {
        keepLocal = "--keep-local ";
      }
      wclear(win);
      wrefresh(win);
      string joinedStr = boost::join(items, " ");
      string result = exec(("svn rm " + keepLocal + joinedStr).c_str());

      wprintw(win, result.c_str());
      wrefresh(win);
      getch();
    }
    State::cursorPosition = 0;
    State::padPos = 0;
    wclear(win);
    refreshStatusItems(pad, win);
    wrefresh(win);
    break;
  }
  case 'u':
    mvwprintw(win, bottomWindowSize - 1, 0,
              "Bring changes from the repository into the working "
              "copy? (y/n?)");
    wrefresh(win);
    timeout(8000);
    if (getch() == 'y') {
      wclear(win);
      wrefresh(win);
      string result = exec("svn up");
      wprintw(win, result.c_str());
      wrefresh(win);
      getch();
      refreshStatusItems(pad, win);
    } else {
      wclear(win);
      wrefresh(win);
    }

    break;
  case ' ':
    if (find(State::selectedItems.begin(), State::selectedItems.end(),
             State::statusItems[State::cursorPosition]) !=
        State::selectedItems.end()) {
      State::selectedItems.erase(
          remove(State::selectedItems.begin(), State::selectedItems.end(),
                 State::statusItems[State::cursorPosition]),
          State::selectedItems.end());
    } else {
      State::selectedItems.push_back(State::statusItems[State::cursorPosition]);
    }
    moveOneRowDown(pad);
    break;
  case '.': {
    State::toggleMode(State::HIDDEN);
    bool hidden = State::getMode(State::HIDDEN);

    wclear(win);
    wrefresh(win);
    string s = "HIDDEN = ";
    wprintw(win, (s + (hidden ? "true" : "false")).c_str());
    wrefresh(win);
    break;
  }
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
    init_pair(8, 74, -1);

    getmaxyx(stdscr, State::maxY, State::maxX);
    win = newwin(bottomWindowSize, State::maxX, State::maxY - bottomWindowSize,
                 0);
    pad = newpad(State::maxY - bottomWindowSize, State::maxX);
    if (pad == NULL) {
      endwin();
      return printf("ERROR: Couldn't create pad\n");
    }

    refreshStatusItems(pad, win);

    scrollok(win, TRUE);
    idlok(win, TRUE);
    while (State::isRunning == true) {
      keyEvent(pad, win);
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

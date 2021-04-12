#import "svn_curses.h"

void printCursorPosition(WINDOW *p_cursorWin) {
  wclear(p_cursorWin);
  mvwprintw(p_cursorWin, State::cursorPosition, 2, "-");
  wrefresh(p_cursorWin);
}

void printSelectedInCursorPosition(WINDOW *p_cursorWin) {
  wclear(p_cursorWin);
  mvwprintw(p_cursorWin, State::cursorPosition, 2, "-");
  wrefresh(p_cursorWin);
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

void printAllStatus(WINDOW *p_win) {
  wclear(p_win);
  for (int i = 0; i < State::statusItems.size(); i++) {

    vector<string>::iterator it =
        find(State::selectedItems.begin(), State::selectedItems.end(),
             State::statusItems[i]);
    if (it != State::selectedItems.end()) {
      wprintw(p_win, " ");
    }

    if (i == State::cursorPosition) {
      wattron(p_win, COLOR_PAIR(6));
      wprintw(p_win, (State::statusItems[i] + "\n").c_str());
      wattroff(p_win, COLOR_PAIR(6));
    } else {
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
        wprintw(p_win, (State::statusItems[i] + "\n").c_str());
        /* wprintw(p_win, */
        /* ((string() + State::statusItems[i].front()) + "\n").c_str()); */
      }
    }
    wrefresh(p_win);
  }
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
    if (State::cursorPosition < State::statusItems.size() - 2) {
      State::cursorPosition = State::cursorPosition + 1;
    }
    break;
  case 'k':
    if (State::cursorPosition > 0) {
      State::cursorPosition = State::cursorPosition - 1;
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

void *initCDK(WINDOW *win, int maxY, int maxX) {
  CDKSCREEN *cdkScreen = initCDKScreen(win);
  initCDKColor();

  int size = State::statusItems.size();
  char *array[size];
  for (int i = 0; i < size - 1; i++) {
    array[i] = strcpy(array[i], State::statusItems[i].c_str());
  }

  CDKSCROLL *cdkScroll;

  newCDKScroll(cdkScreen, CENTER, CENTER, RIGHT, 8, 20, "title", array, size,
               TRUE, A_REVERSE, TRUE, FALSE);
  setCDKScrollPosition(cdkScroll, 2);
  int id_type = activateCDKScroll(cdkScroll, 0);

  char temp[256], *mesg[10];
  if (cdkScroll->exitType == vNORMAL) {
    sprintf(temp, "(%s)", chtype2Char(cdkScroll->item[id_type]));
    mesg[0] = temp;
    popupLabel(cdkScreen, mesg, 1);
  }

  destroyCDKScroll(cdkScroll);
  destroyCDKScreen(cdkScreen);
  return 0;
}

int main() {
  try {
    initscr();
    State::isRunning = true;
    WINDOW *win;
    WINDOW *cursorWin;
    /* cbreak(); */
    /* timeout(1000); */
    /* keypad(stdscr, true); */
    /* start_color(); */
    /* use_default_colors(); */
    /* curs_set(0); */
    /* noecho(); */
    /* scrollok(win, true); */
    /* scrollok(cursorWin, true); */

    /* init_pair(1, COLOR_BLUE, -1); */
    /* init_pair(2, COLOR_RED, -1); */
    /* init_pair(3, COLOR_YELLOW, -1); */
    /* init_pair(4, COLOR_GREEN, -1); */
    /* init_pair(5, COLOR_MAGENTA, -1); */
    /* init_pair(6, COLOR_BLACK, COLOR_WHITE); */

    int maxX = 0, maxY = 0;
    getmaxyx(stdscr, maxY, maxX);
    win = newwin(maxY, maxX - 4, 0, 4);
    cursorWin = newwin(maxY, 4, 0, 0);

    string execResult = exec("svn st");
    split(State::statusItems, execResult, boost::is_any_of("\n"));

    /* wrefresh(win); */
    /* refresh(); */
    /* printAllStatus(win); */
    initCDK(win, maxY, maxX);

    /* while (State::isRunning == true) { */
    /*   keyEvent(win); */
    /* printCursorPosition(cursorWin); */
    /* wrefresh(win); */
    /* refresh(); */
    /* } */

    endCDK();
    endwin();

    return 0;
  } catch (const exception &e) {
    cout << e.what();
    endCDK();
    endwin();

    return 1;
  }
}

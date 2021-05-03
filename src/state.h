#ifndef STATE_H
#define STATE_H

#include <string>
#include <tuple>
#include <vector>
#include <stdexcept>
#include <cstdio>

using namespace std;

class State {
protected:
  State();

public:
  static bool isRunning;
  static vector<string> statusItems;
  static vector<string> allStatusItems;
  static vector<string> selectedItems;
  static int cursorPosition, maxX, maxY, padPos;
  enum mode { ADD, HIDDEN, RM };
  static void toggleMode(State::mode m);
  static bool getMode(mode m);
  static tuple<mode, bool> getAddMode();
  static void setAddMode(tuple<mode, bool>);
  static tuple<mode, bool> getHiddenMode();
  static void setHiddenMode(tuple<mode, bool>);
  static tuple<mode, bool> getRmMode();
  static void setRmMode(tuple<mode, bool>);

private:
  static tuple<mode, bool> add;
  static tuple<mode, bool> hidden;
  static tuple<mode, bool> rm;
};

#endif

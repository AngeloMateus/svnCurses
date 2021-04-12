#ifndef STATE_H
#define STATE_H

#include <string>
#include <vector>

using namespace std;

class State {
protected:
  State();

public:
  static bool isRunning;
  static vector<string> statusItems;
  static vector<string> selectedItems;
  static int cursorPosition;
};

#endif
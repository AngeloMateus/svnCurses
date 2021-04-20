#include "state.h"

bool State::isRunning;
vector<string> State::statusItems;
vector<string> State::selectedItems;
int State::cursorPosition = 0, State::maxX = 0, State::maxY = 0,
    State::padPos = 0;

State::State() {}

#include "state.h"

State::State() {}

bool State::isRunning;
enum mode { ADD, HIDDEN, RM };
vector<string> State::statusItems;
vector<string> State::allStatusItems;
vector<string> State::selectedItems;
int State::cursorPosition = 0, State::maxX = 0, State::maxY = 0,
    State::padPos = 0;

tuple<State::mode, bool> State::add = make_tuple(State::ADD, false);
tuple<State::mode, bool> State::hidden = make_tuple(State::HIDDEN, false);
tuple<State::mode, bool> State::rm = make_tuple(State::RM, false);

tuple<State::mode, bool> State::getAddMode() { return State::add; }
void State::setAddMode(tuple<State::mode, bool> t) { State::add = t; }
tuple<State::mode, bool> State::getHiddenMode() { return State::hidden; }
void State::setHiddenMode(tuple<State::mode, bool> t) { State::hidden = t; }
tuple<State::mode, bool> State::getRmMode() { return State::rm; }
void State::setRmMode(tuple<State::mode, bool> t) { State::rm = t; }

bool State::getMode(State::mode m) {
  switch (m) {
  case State::ADD:
    return get<1>(State::getAddMode());
    break;
  case State::HIDDEN:
    return get<1>(State::getHiddenMode());
    break;
  case State::RM:
    return get<1>(State::getRmMode());
    break;
  default:
    return false;
  }
}

void State::toggleMode(State::mode m) {
  switch (m) {
  case State::ADD: {
    bool val = !get<1>(State::getAddMode());
    State::setAddMode(make_tuple(m, val));
    break;
  }
  case State::HIDDEN: {
    bool val = !get<1>(State::getHiddenMode());
    State::setHiddenMode(make_tuple(m, val));
    break;
  }
  case State::RM: {
    bool val = !get<1>(State::getRmMode());
    State::setRmMode(make_tuple(m, val));
    break;
  }
  }
}

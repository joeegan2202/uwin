#ifndef __SIGNAL_HPP__
#define __SIGNAL_HPP__

#include <stack>

enum SignalType {
  SIG_KILL,
};

struct Signal {
  enum SignalType type;
  char msg[256];
};

extern std::stack<Signal> signals;

#endif //__SIGNAL_HPP__

//
// Created by joe on 2/28/24.
//

#ifndef UWIN_WINDOWMESSAGE_HPP
#define UWIN_WINDOWMESSAGE_HPP

enum WindowMessageType {
  WINDOW_NEW = 1,
  WINDOW_CLOSE,
  WINDOW_RESIZE,
  INPUT,
};

enum InputType {
  INPUT_BUTTONCLICK = 1,
  INPUT_KEYPRESS,
};

enum Button {
  BUTTON_LEFT = 1,
  BUTTON_RIGHT,
  BUTTON_MIDDLE
};

enum Key {
  KEY_A = 1,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
};

struct NewWindow {
  char name[64];
  long id;
  int w;
  int h;
};

struct CloseWindow {
  long id;
};

struct ResizeWindow {
  long id;
  int w;
  int h;
};

struct Input {
  enum InputType type;
  union {
    enum Button button;
    enum Key key;
  } body;
};

struct WindowMessage {
  enum WindowMessageType type;
  union {
    struct NewWindow newWindow;
    struct CloseWindow closeWindow;
    struct ResizeWindow resizeWindow;
    struct Input input;
  } body;
};

#endif //UWIN_WINDOWMESSAGE_HPP

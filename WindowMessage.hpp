//
// Created by joe on 2/28/24.
//

#ifndef UWIN_WINDOWMESSAGE_HPP
#define UWIN_WINDOWMESSAGE_HPP

enum WindowMessageType {
    WINDOW_NEW = 1,
};

struct NewWindow {
char name[64];
long id;
int w;
int h;
};

struct WindowMessage {
    enum WindowMessageType type;
    union {
        struct NewWindow newWindow;
    } body;
};

#endif //UWIN_WINDOWMESSAGE_HPP

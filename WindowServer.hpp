//
// Created by joe on 2/26/24.
//

#ifndef UWIN_WINDOWSERVER_HPP
#define UWIN_WINDOWSERVER_HPP

#include "Window.hpp"
#include "WindowMessage.hpp"
#include "VideoDevice.hpp"

#include <unordered_map>

class WindowServer {
public:
    WindowServer(VideoDevice&, const Image&);
    ~WindowServer();
    void start();
private:
	VideoDevice& videoDevice;
    Framebuffer buffer;
    const Image &backgroundImage;
    std::unordered_map<int, Window *> activeWindows;
    void newWindow(std::string name, int width, int height);
    void sendMessageWithFileDescriptor(WindowMessage, int);
    void sendMessage(WindowMessage);
    void listen();
    int sock_fd;
};

#endif //UWIN_WINDOWSERVER_HPP

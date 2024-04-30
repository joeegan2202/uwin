//
// Created by joe on 2/26/24.
//

#ifndef UWIN_WINDOWSERVER_HPP
#define UWIN_WINDOWSERVER_HPP

#include "Window.hpp"
#include "WindowMessage.hpp"
#include "VideoDevice.hpp"
#include "Geometry.hpp"
#include "Launcher.hpp"

#include <unordered_map>
#include <list>
#include <atomic>
#include <mutex>

#include <sys/socket.h>
#include <sys/un.h>

class WindowServer {
  public:
    WindowServer(VideoDevice&, const Image&);
    void start();
  private:
    VideoDevice& videoDevice;
    Framebuffer buffer;
    const Image &backgroundImage;

    std::unordered_map<int, Window *> liveWindows;
    std::mutex orderedWindows_m;
    std::list<Window *> orderedWindows;
    Window *activeWindow;
    std::list<Launcher> launchers;
    void handleClickCallback(int button, Point location);
    void handleDragCallback(int button, bool still_dragging, Point location);

    void newWindow(std::string name, int width, int height, struct sockaddr_un *, socklen_t *);
    void sendMessageWithFileDescriptor(WindowMessage, int, struct sockaddr_un *, socklen_t *);
    void sendMessage(WindowMessage);

    std::atomic_bool should_run;
    void listen();
    int sock_fd;
};

#endif //UWIN_WINDOWSERVER_HPP

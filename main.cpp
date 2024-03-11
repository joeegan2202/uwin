#include <iostream>

#include "VideoDevice.hpp"
#include "Framebuffer.hpp"
#include "WindowServer.hpp"
#include <unistd.h>

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::string fb_name = "/dev/fb0";
    VideoDevice fb0(fb_name);

    std::string leaf_location = "leaf.jpg";
    Image leaf(leaf_location);

    WindowServer server(fb0, leaf);

    server.start();

    sleep(10);

    return 0;
}

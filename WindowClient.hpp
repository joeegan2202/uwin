//
// Created by joe on 2/28/24.
//

#ifndef UWIN_WINDOWCLIENT_HPP
#define UWIN_WINDOWCLIENT_HPP

#include "WindowMessage.hpp"
#include "Framebuffer.hpp"

namespace uwin {

    class WindowClient {
    public:
        WindowClient();
        ~WindowClient();
    	void establish();
        Framebuffer *getFramebuffer() {
            return fbuf;
        }
    private:
        WindowMessage sendMessage(WindowMessage);
        int sock_fd;
        Framebuffer *fbuf;
    };

}

#endif //UWIN_WINDOWCLIENT_HPP

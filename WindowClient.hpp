//
// Created by joe on 2/28/24.
//

#ifndef UWIN_WINDOWCLIENT_HPP
#define UWIN_WINDOWCLIENT_HPP

#include "WindowMessage.hpp"

namespace uwin {

    class WindowClient {
    public:
        WindowClient();
        ~WindowClient();
    private:
        WindowMessage sendMessage(WindowMessage);
        int sock_fd;
    };

}

#endif //UWIN_WINDOWCLIENT_HPP

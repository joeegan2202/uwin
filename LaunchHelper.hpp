#ifndef __LAUNCHER_HELPER_HPP__
#define __LAUNCHER_HELPER_HPP__

#include <vector>
#include <string>

class LauncherHelper {
  public:
    static void launch(std::string program_name, const std::vector<std::string> &arguments);
};

#endif //__LAUNCHER_HELPER_HPP__

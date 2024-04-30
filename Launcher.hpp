#ifndef __LAUNCHER_LAUNCHER_HPP__
#define __LAUNCHER_LAUNCHER_HPP__

#include "Geometry.hpp"
#include "Image.hpp"
#include "Framebuffer.hpp"

#include <string>
#include <vector>

class Launcher {
  Image icon;
  Framebuffer print_fb;
  const std::string name;
  const std::string exec;
  const std::vector<std::string> args;
  public:
    Launcher(std::string icon, std::string name, std::string exec, std::vector<std::string> args);
    Launcher(std::string icon, std::string name, std::string exec, std::vector<std::string> args, int x_pos, int y_pos);
    void launch();
    Rectangle bounds;
    const Framebuffer &getFramebuffer();
};

#endif //__LAUNCHER_LAUNCHER_HPP__

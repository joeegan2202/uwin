#include "Launcher.hpp"

#include "Font.hpp"
#include "LaunchHelper.hpp"

Launcher::Launcher(std::string icon_name, std::string name, std::string exec, std::vector<std::string> args) : Launcher(icon_name, name, exec, args, 800, 800) {}

Launcher::Launcher(std::string icon_name, std::string name, std::string exec, std::vector<std::string> args, int x_pos, int y_pos) :
  icon(icon_name),
  name(name),
  exec(exec),
  args(args),
  bounds(Rectangle::fromWidthHeight({.x = x_pos, .y = y_pos}, 32, 32)) {
  Framebuffer name_fb = FontManager::render(name);

  bounds = Rectangle::fromWidthHeight({.x = x_pos, .y = y_pos}, std::max(icon.getXres(), name_fb.getWidth()), icon.getYres() + name_fb.getHeight());
  print_fb = Framebuffer(bounds.getWidth(), bounds.getHeight(), 4);
  int icon_off = (name_fb.getWidth() - icon.getXres()) / 2;
  if(icon_off < 0) icon_off = 0;
  print_fb.blit(icon, icon_off, 0);

  int name_off = (icon.getXres() - name_fb.getWidth()) / 2;
  if(name_off < 0) name_off = 0;
  print_fb.blit(name_fb, name_off, icon.getYres());
}

const Framebuffer &Launcher::getFramebuffer() {
  return print_fb;
}

void Launcher::launch() {
  LauncherHelper::launch(exec, args);
}

#include "Panel.hpp"
#include "Font.hpp"

Panel::Panel() : bar("bar.bmp"), buttonLeft("buttonLeft.bmp"), buttonRight("buttonRight.bmp"), buttonFill("buttonFill.bmp") {

}

Framebuffer Panel::getPanel(const std::unordered_map<int, Window *> &windows) {
  Framebuffer framebuffer(1920, 24, 4);

  framebuffer.blit(bar, 0, 0);

  int windows_x_index = 1;
  for(auto [id, window] : windows) {
    Framebuffer window_name_fb = FontManager::render(window->name);
    Framebuffer button_fb(window_name_fb.getWidth() + buttonLeft.getXres() + buttonRight.getXres(), buttonFill.getYres(), 4);

    button_fb.blit(buttonLeft, 0, 0);

    int x_index = buttonLeft.getXres();
    for(;x_index < button_fb.getWidth() - buttonRight.getXres(); x_index++) {
      button_fb.blit(buttonFill, x_index, 0);
    }
    button_fb.blit(buttonRight, x_index, 0);

    button_fb.blit(window_name_fb, buttonLeft.getXres(), (buttonFill.getYres() - window_name_fb.getHeight()) / 2);

    framebuffer.blit(button_fb, windows_x_index, 4);
    windows_x_index += button_fb.getWidth();
  }

  return framebuffer;
}

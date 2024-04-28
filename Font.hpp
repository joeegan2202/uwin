#ifndef __FONT_FONT_HPP__
#define __FONT_FONT_HPP__

#include "Image.hpp"
#include "Framebuffer.hpp"

#include <vector>

class FontManager {
  static std::vector<Image> images;
  static inline std::vector<Image>::reference image(char c);
  public:
    static void ensureImages();
    static Framebuffer render(std::string text);
};

#endif //__FONT_FONT_HPP__

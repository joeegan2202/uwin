//
// Created by joe on 2/6/24.
//

#ifndef FRAMEBUFFER_FRAMEBUFFER_HPP
#define FRAMEBUFFER_FRAMEBUFFER_HPP

#include <string>
#include "Image.hpp"

class Framebuffer {
  public:
    Framebuffer() = default;
    Framebuffer(int f, int w, int h, int c);
    Framebuffer(int w, int h, int c);
    ~Framebuffer();

    void blit(const Image &image, ssize_t x_off, ssize_t y_off);
    void blit(const Framebuffer &image, ssize_t x_off, ssize_t y_off);
    int getWidth();
    int getHeight();
  private:
    int fd;
    int width, height, channels;
    uint8_t *framebuffer_map;
    long int screen_size;
};

#endif //FRAMEBUFFER_FRAMEBUFFER_HPP

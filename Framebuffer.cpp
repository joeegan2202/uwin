//
// Created by joe on 2/6/24.
//

#include "Framebuffer.hpp"
#include <iostream>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstring>

Framebuffer::Framebuffer(int f, int w, int h, int c) : fd(f), width(w), height(h), channels(c) {
    // Size of framebuffer in bytes
    screen_size = width * height * channels;

    framebuffer_map = (uint8_t *)mmap(nullptr, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if((long)framebuffer_map == -1) {
        std::cerr << "Error mapping framebuffer to memory!" << std::endl;
        exit(5);
    }
}

Framebuffer::Framebuffer(int w, int h, int c) : fd(0), width(w), height(h), channels(c), screen_size(width * height * channels) {
    framebuffer_map = (uint8_t *)malloc(screen_size);
}

Framebuffer::~Framebuffer() {
}

// FB: BGRT
void Framebuffer::blit(const Image &image, ssize_t x_off, ssize_t y_off) {
    if(x_off == 0 && y_off == 0 && image.getXres() == width && image.getYres() == height) {
        memcpy(framebuffer_map, image.memory, screen_size);
        return;
    }

    for(unsigned i = 0; i < image.getYres(); i++) {
        if(i + y_off >= height) break;
        for(unsigned j = 0; j < image.getXres(); j++) {
            if(j + x_off >= width) break;
            unsigned fb_offset = ((i + y_off) * width + j + x_off) * channels;
            unsigned img_offset = (i * image.getXres() + j) * channels;
            switch (image.memory[img_offset + 3])
            {
            case 0:
                break;
            case 255:
                framebuffer_map[fb_offset] = image.memory[img_offset];
                framebuffer_map[fb_offset + 1] = image.memory[img_offset + 1];
                framebuffer_map[fb_offset + 2] = image.memory[img_offset + 2];
                framebuffer_map[fb_offset + 3] = image.memory[img_offset + 3];
                break;
            default:
                // TODO: Implement alpha blitting
                break;
            }
        }
    }
}

// FB: BGRT
void Framebuffer::blit(const Framebuffer &f, ssize_t x_off, ssize_t y_off) {
    if(x_off == 0 && y_off == 0 && f.width == width && f.height == height) {
        memcpy(framebuffer_map, f.framebuffer_map, screen_size);
        return;
    }

    for(unsigned i = 0; i < f.height; i++) {
        if(i + y_off >= height) break;
        for(unsigned j = 0; j < f.width; j++) {
            if(j + x_off >= width) break;
            unsigned fb_offset = ((i + y_off) * width + j + x_off) * channels;
            unsigned img_offset = (i * f.width + j) * channels;
            framebuffer_map[fb_offset] = f.framebuffer_map[img_offset];
            framebuffer_map[fb_offset + 1] = f.framebuffer_map[img_offset + 1];
            framebuffer_map[fb_offset + 2] = f.framebuffer_map[img_offset + 2];
            framebuffer_map[fb_offset + 3] = f.framebuffer_map[img_offset + 3];
        }
    }
}

int Framebuffer::getWidth() {
  return width;
}

int Framebuffer::getHeight() {
  return height;
}

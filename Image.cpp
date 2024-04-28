//
// Created by joe on 2/6/24.
//

#include "Image.hpp"

#include <filesystem>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image::Image(std::string file_location) {
    std::filesystem::path real_location = std::filesystem::current_path().append(file_location);
    std::cout << "Loading " << real_location.c_str() << std::endl;
    memory = stbi_load(real_location.c_str(), &xres, &yres, &bpp, 4);

    if(memory == nullptr) {
        std::cout << stbi_failure_reason() << std::endl;
        return;
    }

    for(int i = 0; i < yres; i++) {
        for(int j = 0; j < xres; j++) {
            int offset = (i * xres + j) * 4;
            uint8_t tmp = memory[offset];
            memory[offset] = memory[offset + 2];
            memory[offset + 2] = tmp;
        }
    }
}

Image::~Image() {
    free(memory);
}

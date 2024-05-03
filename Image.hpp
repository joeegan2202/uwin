//
// Created by joe on 2/6/24.
//

#ifndef UWIN_IMAGE_HPP
#define UWIN_IMAGE_HPP

#include <cstdint>
#include <memory>
#include <string>

class Image {
public:
    Image();
    explicit Image(std::string file_location);
    [[nodiscard]] int getXres() const { return xres; }
    [[nodiscard]] int getYres() const { return yres; }
    [[nodiscard]] int getBpp() const { return bpp; }
    std::unique_ptr<uint8_t> memory;
private:
    int xres, yres, bpp;
};


#endif //UWIN_IMAGE_HPP

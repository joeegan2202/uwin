//
// Created by joe on 2/6/24.
//

#ifndef UWIN_IMAGE_HPP
#define UWIN_IMAGE_HPP

#include <string>

class Image {
public:
    explicit Image(const std::string& file_location);
    ~Image();
    [[nodiscard]] int getXres() const { return xres; }
    [[nodiscard]] int getYres() const { return yres; }
    [[nodiscard]] int getBpp() const { return bpp; }
    uint8_t *memory;
private:
    int xres, yres, bpp;
};


#endif //UWIN_IMAGE_HPP

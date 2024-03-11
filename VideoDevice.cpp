//
// Created by joe on 2/6/24.
//

#include "VideoDevice.hpp"

#include <iostream>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <linux/vt.h>
#include <linux/kd.h>

VideoDevice::VideoDevice(const std::string& d) : deviceFileName(d) {
    device_file_descriptor = open(deviceFileName.c_str(), O_RDWR);
    if(device_file_descriptor == -1) {
        std::cerr << "Failed to open video device!" << std::endl;
        exit(1);
    }
    std::cout << "Opened video device..." << std::endl;

    if(ioctl(device_file_descriptor, FBIOGET_FSCREENINFO, &finfo) == -1) {
        std::cerr << "Error reading fixed information from device \"" << deviceFileName << "\"!" << std::endl;
        exit(2);
    }

    if(ioctl(device_file_descriptor, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        std::cerr << "Error reading variable information from device \"" << deviceFileName << "\"!" << std::endl;
        exit(3);
    }

    std::cout << "Device \"" << deviceFileName << "\": " << vinfo.xres << "x" << vinfo.yres << " " << vinfo.bits_per_pixel << "bpp..." << std::endl;

		deviceFb = Framebuffer(device_file_descriptor, vinfo.xres, vinfo.yres, vinfo.bits_per_pixel >> 3);
}

VideoDevice::~VideoDevice() {
    // DANGER ioctl(ttyfd, KDSETMODE, KD_TEXT);
}

void VideoDevice::disableTty() {
    ttyfd = open(ttyname(STDIN_FILENO), O_RDONLY);

    std::cout << "Got ttyfd " << ttyfd << std::endl;

    if(ioctl(ttyfd, KDSETMODE, KD_GRAPHICS)) {
        std::cerr << "Error setting device to graphics mode! " << deviceFileName << " errno " << errno << std::endl;
        exit(4);
    }
}

void VideoDevice::blit(const Image &i, int w, int h) {
	deviceFb.blit(i, w, h);
}

void VideoDevice::blit(const Framebuffer &f) {
	deviceFb.blit(f, 0, 0);
}
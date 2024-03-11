//
// Created by joe on 2/6/24.
//

#ifndef FRAMEBUFFER_VIDEODEVICE_HPP
#define FRAMEBUFFER_VIDEODEVICE_HPP

#include <string>
#include <linux/fb.h>

#include "Framebuffer.hpp"
#include "Image.hpp"

class VideoDevice {
public:
	VideoDevice(const std::string&);
	~VideoDevice();

	void blit(const Image &, int, int);
	void blit(const Framebuffer &);

	unsigned getWidth() {
		return vinfo.xres;
	}
	unsigned getHeight() {
		return vinfo.yres;
	}
	unsigned getChannels() {
		return vinfo.bits_per_pixel >> 3;
	}
private:
	std::string deviceFileName;
  int device_file_descriptor;
  int ttyfd;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

	Framebuffer deviceFb;

	void disableTty();
};


#endif //FRAMEBUFFER_VIDEODEVICE_HPP

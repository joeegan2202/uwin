#include "Mouse.hpp"

#include <iostream>
#include <unistd.h>
#include <mutex>
#include "fcntl.h"

static Point mouse_location;
static std::mutex mouse_location_mutex;
static MouseMode mouse_mode;
static std::mutex mouse_mode_mutex;

static int fd;

void initializeMouse() {
	std::string file_name = "/dev/input/mice";
	fd = open(file_name.c_str(), O_RDONLY);
	if(fd == -1) {
		std::cerr << "Couldn't open mouse device!" << std::endl;
		exit(fd);
	}
	std::lock_guard<std::mutex> lg(mouse_location_mutex);
	std::lock_guard<std::mutex> lg1(mouse_mode_mutex);
	mouse_location.x = 100;
	mouse_location.y = 100;
	mouse_mode = MouseMode::CURSOR;
	std::cout << "Initialized mouse device!" << std::endl;
}

Point getLocation() {
	return mouse_location;
}

void setMode(MouseMode mode) {
	std::lock_guard<std::mutex> lg(mouse_mode_mutex);
	mouse_mode = mode;
}

void pollEvents(std::atomic_bool &should_run) {
	char buf[3];

	std::cout << "Input thread started..." << std::endl;
	while(should_run && read(fd, buf, 3) == 3) {
		//std::cout << "Left: " << (buf[0]&1) << " Right: " << (buf[0]&2) << " Middle: " << (buf[0]&4) << " X: " << (int)buf[1] << " Y: " << (int)buf[2] << std::endl;
		{
			std::lock_guard<std::mutex> lg(mouse_location_mutex);
			mouse_location.x += buf[1];
			if(mouse_location.x >= 1920) mouse_location.x = 1919;
			if(mouse_location.x < 0) mouse_location.x = 0;
			mouse_location.y -= buf[2];// DANGER 
			if(mouse_location.y >= 1080) mouse_location.y = 1079;
			if(mouse_location.y < 0) mouse_location.y = 0;
		}
	}
}
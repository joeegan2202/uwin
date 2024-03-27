#include <iostream>

#include "../WindowClient.hpp"

int main() {
	Image bear("bear.jpg");

	uwin::WindowClient client;

	client.establish();

	client.getFramebuffer()->blit(bear, 0, 0);

	return 0;
}
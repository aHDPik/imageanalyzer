#pragma once

namespace imagelib {

	struct Detection {
		int x;
		int y;
		int width;
		int height;
	};

	Detection detect(unsigned char* image, int width, int height);
	void noise(unsigned char* image, int width, int height, double percent);

	int index(int x, int y, int width);

}


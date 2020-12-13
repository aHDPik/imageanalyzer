}#pragma once
#include <vector>
#include <iostream>
#include <fstream>

namespace imagelib {

	struct Y1_Y2 {
		int y1 = -1;
		int y2 = -1;
	};

	struct X1_X2 {
		int x1 = -1;
		int x2 = -1;
	};

	struct Objects {
		std::vector<Y1_Y2> objectsY;
		std::vector<X1_X2> objectsX;
	};

	struct Detection {
		int x;
		int y;
		int width;
		int height;
	};

	std::vector<Detection> detect(unsigned char* image, int width, int height, int minCountPicsel, int minSquare);

	void binarization(unsigned char* image, int width, int height, std::vector<int> color);

	void noise(unsigned char* image, int width, int height, double percent);

	int index(int x, int y, int width);

	struct Matrix {
		std::string name;
		std::vector<double> arr1;
		std::vector<double> arr2;
		int width;
		int height;
	};

	unsigned char clamp(double val);

	void modify_image(unsigned char* image, int width, int height, Matrix M);

	void copy_edges(unsigned char* image, unsigned char* bigImage, std::uint32_t width, std::uint32_t height, Matrix M);
}


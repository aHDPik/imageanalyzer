#pragma once
#include <vector>
#include <iostream>
#include <fstream>

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

	struct Matrix {
		std::string name;
		std::vector<double> arr1;
		std::vector<double> arr2;
		int width;
		int height;
	};

	unsigned char clamp(double val);

	Matrix modify_image(unsigned char* image, int width, int height, Matrix M);
}


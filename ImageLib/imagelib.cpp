#include "imagelib.h"
#include <vector>
#include <iostream>

namespace imagelib {

    int index(int x, int y, int width) {
        return ((x + y * width) * 3);
    }

    //� �� �����, ��� �������� ����, �� ����� ���� ��� ���. ����� ����� �����, ����� ����� ������� ������ ��������
    void noise(/* ��� ������ ���� ����, */int width, int height, int percent) {
        srand(rand()%100);
        float noise = percent;
        int sizeImg = width * height;
        for (int i = 0; i < sizeImg * noise; i++) {
            int size = rand() % 4;
            int x = rand() % width, y = rand() % height;
            for (int j = 0; j < size; j++){
                ///*��� ����� �� ����*/.data[index(x, y, image.cols) + j] = rand() % 256;
            }
        }
    }

    Detection detect(unsigned char* image, int width, int height)
    {
            int blue=0, green=0, red=0;
            std::vector<int> ourColorMin = { blue - 10, green - 10, red - 10 };
            std::vector<int> ourColorMax = { blue + 10, green + 10, red + 10 };
            std::vector<int> thisColor;
            int x1_flag = -1, y1_flag = -1, x2_flag = -1, y2_flag = -1, i;
            for (int y = 0; y < height; y++)
                for (int x = 0; x < width; x++) {
                    int ind = index(x, y, width);
                    for (i = 0; i < 3; i++) {
                        if (((int)image[ind] + i) < ourColorMin[i] || (int)(image[ind] + i) > ourColorMax[i]) {
                            break;
                        }
                    }
                    //���� i=3, ������ ����� ������, ���� i<3, ������ ��������
                    if (i == 3) {
                        if (y1_flag == -1) {
                            y1_flag = y;
                            x1_flag = x;
                            x2_flag = x;
                        }
                        else
                            y2_flag = y;

                        if (x1_flag > x)
                            x1_flag = x;
                        if (x2_flag < x)
                            x2_flag = x;
                    }

                }
            Detection res = { x1_flag ,y1_flag, x2_flag - x1_flag, y2_flag - y1_flag };
            return res;
    }
}
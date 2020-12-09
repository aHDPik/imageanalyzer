#include "imagelib.h"

namespace imagelib {

    int index(int x, int y, int width) {
        return ((x + y * width) * 3);
    }

    void noise(unsigned char* image, int width, int height, double percent) {
        srand(rand()%100);
        double noise = percent;
        int sizeImg = width * height;
        for (int i = 0; i < sizeImg * noise; i++) {
            int size = rand() % 4;
            int x = rand() % width, y = rand() % height;
            for (int j = 0; j < size; j++){
                image[index(x, y, width) + j] = rand() % 256;
            }
        }
    }

    std::vector<Detection> detect(unsigned char* image, int width, int height)
    {
        std::vector<Detection> objects;
            int blue=0, green=0, red=0;
            std::vector<int> ourColorMin = { blue - 10, green - 10, red - 10 };
            std::vector<int> ourColorMax = { blue + 10, green + 10, red + 10 };
            std::vector<int> thisColor;
            int x1_flag = -1, y1_flag = -1, x2_flag = -1, y2_flag = -1, i;
            bool flag_str=false;
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
            objects.push_back(res);
           
            return objects;
    }

    void modify_image(unsigned char* image, int width, int height, Matrix M) {


            unsigned char* bigImage = new unsigned char [3*(height + 2 * (M.height / 2)) * (width + 2 * (M.width / 2))];
            for (int y = (M.height / 2); y < height + (M.height / 2); y++)
                for (int x = (M.width / 2); x < width + (M.width / 2); x++) {
                    int ind = index(x, y, width + 2 * int((M.width / 2)));
                    int ind1 = index(x - int((M.width / 2)), y - int((M.height / 2)), width);
                    for (int i = 0; i < 3; i++)
                        bigImage[ind + i] = image[ind1 + i];
                }
            copy_edges(image, bigImage, width, height, M);

        for (int y = (M.height / 2); y < height + (M.height / 2); y++)
            for (int x = (M.width / 2); x < width + (M.height / 2); x++) {
                int ind = index(x - (M.width / 2), y - (M.height / 2), width);
                for (int i = 0; i < 3; i++) {
                    int j = 0;
                    double r = 0;
                    image[ind + i] = 0;
                    for (int n = x - (M.width / 2), j = 0; n <= x + (M.width / 2); n++, j++)
                        if ((index(n, y, width + 2 * (M.width / 2)) + i) <= 3 * ((width + 2 * (M.width / 2)) * (height + 2 * (M.height / 2))))
                            r += M.arr1[j] * bigImage[index(n, y, width + 2 * (M.width / 2)) + i];
                    for (int m = y - (M.height / 2), j = 0; m <= y + (M.height / 2); m++, j++)
                        if ((index(x, m, width + 2 * (M.width / 2)) + i) <= 3 * ((width + 2 * (M.width / 2)) * (height + 2 * (M.height / 2))))
                            r += M.arr2[j] * bigImage[index(x, m, width + 2 * (M.width / 2)) + i];
                    image[ind + i] = clamp(r / 2);
                }
            }
    }

    void copy_edges(unsigned char* image, unsigned char* bigImage, std::uint32_t width, std::uint32_t height, Matrix M) {

        for (int x = M.width / 2; x < width + (M.width / 2); x++) {
            int ind1 = index(x - (M.width / 2), 0, width);
            for (int y = 0; y < (M.height / 2); y++) {
                int ind = index(x, y, width + 2 * (M.width / 2));
                for (int i = 0; i < 3; i++)
                    bigImage[ind + i] = image[ind1 + i];
            }
        }

        for (int x = M.width / 2; x < width + M.width / 2; x++) {
            int ind1 = index(x - (M.width / 2), height - 1, width);
            for (int y = height + M.height / 2; y < height + 2 * (M.height / 2); y++) {
                int ind = index(x, y, width + 2 * (M.width / 2));
                for (int i = 0; i < 3; i++)
                    bigImage[ind + i] = image[ind1 + i];
            }
        }

        for (int y = M.height / 2; y < height + (M.height / 2); y++) {
            int ind1 = index(0, y - (M.height / 2), width);
            for (int x = 0; x < (M.width / 2); x++) {
                int ind = index(x, y, width + 2 * (M.height / 2));
                for (int i = 0; i < 3; i++)
                    bigImage[ind + i] = image[ind1 + i];
            }
        }

        for (int y = M.height / 2; y < height + M.height / 2; y++) {
            int ind1 = index(width - 1, y - (M.height / 2), width);
            for (int x = width + M.width / 2; x < width + 2 * (M.width / 2); x++) {
                int ind = index(x, y, width + 2 * (M.width / 2));
                for (int i = 0; i < 3; i++)
                    bigImage[ind + i] = image[ind1 + i];
            }
        }
    }

    unsigned char clamp(double val) {
        if (val < 0)
            return 0;
        if (val > 255)
            return 255;
        return (unsigned char)val;
    }

}
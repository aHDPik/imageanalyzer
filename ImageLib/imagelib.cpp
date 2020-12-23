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

    void binarization(unsigned char* image, int width, int height, std::vector<int> color) {

        std::vector<int> ourColorMin = { color[0] - 50, color[1] - 50, color[2] - 50 };
        std::vector<int> ourColorMax = { color[0] + 50, color[1] + 50, color[2] + 50 };

        int i;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                int ind = index(x, y, width);
                for (i = 0; i < 3; i++) {
                    if ((image[ind + i]) < ourColorMin[i] || (image[ind + i]) > ourColorMax[i]) {
                        break;
                    }
                }
                if (i == 3) {
                    for (int j = 0; j < 3; j++) {
                        image[ind + j] = 0;
                    }
                }
                else {
                    for (int j = 0; j < 3; j++) {
                        image[ind + j] = 255;
                    }
                }
            }
        }
    }


    std::vector<Detection> detect(unsigned char* image, int width, int height, int minCountPicsel, int minSquare)
    {
        std::vector<Detection> result;
        std::vector<int> ourColorMin = { -1, -1, -1 };
        std::vector<int> ourColorMax = { 1, 1, 1 };
        bool flag = false;
        int i;
        Objects objects;
        X1_X2 coordX;
        Y1_Y2 coordY;

        for (int y = 0; y < height; y++) {
            if (!flag && coordY.y1 != -1 && coordY.y2 != -1 && (coordY.y1 != coordY.y2)) {
                objects.objectsY.push_back(coordY);
                coordY.y1 = -1;
                coordY.y2 = -1;
            }
            flag = false;
            for (int x = 0; x < width; x++) {
                int ind = index(x, y, width);
                for (i = 0; i < 3; i++)
                    if ((image[ind + i]) < ourColorMin[i] || (image[ind + i]) > ourColorMax[i])
                        break;
                if (i == 3) {
                    flag = true;
                    if (coordY.y1 == -1)
                        coordY.y1 = y;
                    else
                        coordY.y2 = y;
                }
            }
        }
        flag = false;
        for (int x = 0; x < width; x++) {
            if (!flag && coordX.x1 != -1 && coordX.x2 != -1 && (coordX.x1 != coordX.x2)) {
                objects.objectsX.push_back(coordX);
                coordX.x1 = -1;
                coordX.x2 = -1;
            }
            flag = false;
            for (int y = 0; y < height; y++) {
                int ind = index(x, y, width);
                for (i = 0; i < 3; i++)
                    if ((image[ind + i]) < ourColorMin[i] || (image[ind + i]) > ourColorMax[i])
                        break;
                if (i == 3) {
                    flag = true;
                    if (coordX.x1 == -1) {
                        coordX.x1 = x;
                    }
                    else
                        coordX.x2 = x;
                }
            }
        }

        int counter = 0;//счетчик кол-ва нужных пикселей в области
        int l;
        for (int j = 0; j < objects.objectsY.size(); j++)
            for (i = 0; i < objects.objectsX.size(); i++)
                if ((objects.objectsX[i].x2 - objects.objectsX[i].x1) * (objects.objectsY[j].y2 - objects.objectsY[j].y1) > minSquare) {
                    counter = 0;
                    for (int x = objects.objectsX[i].x1; x < objects.objectsX[i].x2; x++) {
                        for (int y = objects.objectsY[j].y1; y < objects.objectsY[j].y2; y++) {
                            int ind = index(x, y, width);
                            for (l = 0; l < 3; l++)
                                if ((image[ind + l]) < ourColorMin[l] || (image[ind + l]) > ourColorMax[l])
                                    break;
                            if (l == 3) {
                                counter++;
                                break;
                            }
                        }
                        if (counter == minCountPicsel) {
                            result.push_back({ objects.objectsX[i].x1, objects.objectsY[j].y1, objects.objectsX[i].x2 - objects.objectsX[i].x1, objects.objectsY[j].y2 - objects.objectsY[j].y1 });
                            counter = 0;
                            break;
                        }
                    }
                }
        return result;
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
        if (val <= 150)
            return 0;
        if (val > 150)
            return 255;
        return (unsigned char)val;
    }

}
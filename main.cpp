#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

TGAColor RandomColour() {
    return TGAColor(rand() % 256, rand() % 256, rand() % 256, 255);;
}

template <typename t>
void swap(t* a, t* b) {
    t temp = *b;
    *b = *a;
    *a = temp;
}

// Write the line method here
void line(int x0, int y0, int x1, int y1, TGAImage* image, TGAColor colour) {
    bool steep = false;

    if (abs(x0 - x1) < abs(y0 - y1)) {
        swap(&x0, &y0);
        swap(&x1, &y1);
        steep = true;
    }

    if (x0 > x1) {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }



    for (float x = x0; x <= x1; x++) {
        float t = (x - x0) / (x1 - x0);
        float y = y0 * (1 - t) + (y1 * t);

        if (steep) {
            image->set(y, x, colour);
        }
        else {
            image->set(x, y, colour);
        }

    }


}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage* image, TGAColor color) {
    if (t0.y > t1.y) { swap(&t0, &t1); }
    if (t0.y > t2.y) { swap(&t0, &t2); }
    if (t1.y > t2.y) { swap(&t1, &t2); }
    int totalHeight = t2.y - t0.y;
    for (int y = t0.y; y < t1.y; y++) {
        int segmentHeight = t1.y - t0.y + 1;
        int alpha = (y - t0.y) / totalHeight;
        int beta = (y - t0.y) / segmentHeight;
        Vec2i a = (t0 + (t2 - t0) * alpha);
        Vec2i b = (t0 + (t1 - t0) * beta);
        image->set(a.x, y, white);
        image->set(b.x, y, white);
    }
    line(t0.x, t0.y, t1.x, t1.y, image, color);
    line(t1.x, t1.y, t2.x, t2.y, image, color);
    line(t2.x, t2.y, t0.x, t0.y, image, color);
}



int main(int argc, char** argv) {
    srand(time(0));
    Vec2i t0[3] = { Vec2i(10, 70), Vec2i(50, 160), Vec2i(70, 80) };
    Vec2i t1[3] = { Vec2i(180, 50), Vec2i(150, 1), Vec2i(70, 180) };
    Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

    TGAImage image(width, height, TGAImage::RGB);
    triangle(t0[0], t0[1], t0[2], &image, RandomColour());
    triangle(t1[0], t1[1], t1[2], &image, RandomColour());
    triangle(t2[0], t2[1], t2[2], &image, RandomColour());
    image.flip_vertically(); // we want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}


#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width = 1080;
const int height = 1080;

Model* model = nullptr;

TGAColor RandomColour(float i) {
	float b = i * (rand() % 256);
	return TGAColor(b, b, b, 255);
}

void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color) {
	bool steep = false;
	if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
		std::swap(p0.x, p0.y);
		std::swap(p1.x, p1.y);
		steep = true;
	}
	if (p0.x > p1.x) {
		std::swap(p0, p1);
	}

	for (int x = p0.x; x <= p1.x; x++) {
		float t = (x - p0.x) / (float)(p1.x - p0.x);
		int y = p0.y * (1. - t) + p1.y * t;
		if (steep) {
			image.set(y, x, color);
		}
		else {
			image.set(x, y, color);
		}
	}
}

void triangleOld(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	if (t0.y == t1.y && t0.y == t2.y) { return; }

	if (t0.y > t1.y) { std::swap(t0, t1); }
	if (t0.y > t2.y) { std::swap(t0, t2); }
	if (t1.y > t2.y) { std::swap(t1, t2); }
	float total_height = t2.y - t0.y;
	for (float y = t0.y; y < t1.y; y++) {
		float segment_height = t1.y - t0.y + 1;
		float alpha = (y - t0.y) / total_height;
		float beta = (y - t0.y) / segment_height;
		Vec2i a = t0 + (t2 - t0) * alpha;
		Vec2i b = t0 + (t1 - t0) * beta;

		if (a.x > b.x) { std::swap(a, b); }

		for (float j = a.x; j < b.x; j++) {
			image.set(j, y, color);
		}
		//image.set(a.x, y, red);
		//image.set(b.x, y, green);
	}
	for (float y = t1.y; y < t2.y; y++) {
		float segment_height = t2.y - t1.y + 1;
		float alpha = (y - t0.y) / total_height;
		float beta = (y - t1.y) / segment_height;
		Vec2i a = t0 + (t2 - t0) * alpha;
		Vec2i b = t1 + (t2 - t1) * beta;
		
		if (a.x > b.x) { std::swap(a, b); }

		for (float j = a.x; j < b.x; j++) {
			image.set(j, y, color);
		}
		//image.set(b.x, y, green);
		//image.set(a.x, y, red);
	}
	line(t0, t1, image, color);
	line(t1, t2, image, color);
	line(t2, t0, image, color);
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	
}

int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);

	if (2 == argc) {
		model = new Model(argv[1]);
	}
	else {
		model = new Model("cc.obj");
	}

	Vec3f lightDir = Vec3f(0., 0., -1.);
	for (int i = 0; i < model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		Vec2i screen_coords[3];
		Vec3f world_cords[3];// = model->vert(face[j]);
		for (int j = 0; j < 3; j++) {
			Vec3f v = model->vert(face[j]);
			screen_coords[j] = Vec2i((v.x + 1) * width / 2.,
				(v.y + 1.) * height / 2.);
			world_cords[j] = v;
		}

		Vec3f n = ((world_cords[2] - world_cords[0]) ^ (world_cords[1]-world_cords[0]));
		n.normalize();

		float intensity = n * lightDir;
		triangleOld(screen_coords[0], screen_coords[1], screen_coords[2], image, RandomColour(intensity));
	}
	

	image.flip_vertically(); // Origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}


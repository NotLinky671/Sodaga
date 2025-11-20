#include "tgaimage.h"

#include <cmath>

void DrawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
  bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);

  // 检查线是不是太陡
  if (steep) {
    std::swap(x0, y0);
    std::swap(x1, y1);
    steep = true;
  }

  // 检查是不是从右往左画
  if (x0 > x1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }

  int dx = x1 - x0;
  int dy = y1 - y0;

  float derror = std::abs(dy / (float)dx);
  float error = 0;

  int y = y0;
  for (int x = x0; x <= x1; x++) {
    if (steep)
      image.set(y, x, color);
    else
      image.set(x, y, color);

    error += derror;

    if (error > 0.5) {
      y += (y1 > y0 ? 1 : -1);
      error -= 1.0;
    }
  }
}

const TGAColor white = TGAColor{255, 255, 255, 255};
const TGAColor red = TGAColor{255, 0, 0, 255};
const TGAColor green = TGAColor{0, 255, 0, 255};
const TGAColor blue = TGAColor{0, 0, 255, 255};

int main(int argc, char** argv) {
  TGAImage image(100, 100, TGAImage::RGB);

  // 画三条线测试一下
  DrawLine(7, 3, 12, 37, image, red);
  DrawLine(12, 37, 62, 53, image, green);
  DrawLine(62, 53, 7, 3, image, blue);

  image.flip_vertically();
  image.write_tga_file("output.tga");
  return 0;
}
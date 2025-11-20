#include "tgaimage.h"

void drawLine(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color) {
  for (int x = x0; x <= x1; x++) {
    // t 代表当前画到了百分之几
    float t = (x - x0) / (float)(x1 - x0);

    // 根据比例算出当前的 y 在哪里
    int y = y0 + (y1 - y0) * t;

    image.set(x, y, color);
  }
}

const TGAColor white = TGAColor{255, 255, 255, 255};
const TGAColor red = TGAColor{0, 0, 255, 255};  // 注意：TGA颜色顺序是BGRA

int main(int argc, char** argv) {
  TGAImage image(100, 100, TGAImage::RGB);

  drawLine(13, 20, 80, 40, image, white);

  image.flip_vertically();  // 这一行是为了让坐标原点在左下角（符合数学习惯）
  image.write_tga_file("output.tga");

  return 0;
}
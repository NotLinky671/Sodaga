#include <cmath>
#include <cstdlib>
#include <tuple>
#include "geometry.h"
#include "model.h"
#include "tgaimage.h"

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

  // derror 理论上y要加几
  float derror = std::abs(dy) * 2;
  float error = 0;

  int y = y0;
  for (int x = x0; x <= x1; x++) {
    if (steep)
      image.set(y, x, color);
    else
      image.set(x, y, color);

    error += derror;

    if (error > dx) {
      y += (y1 > y0 ? 1 : -1);
      error -= dx * 2;
    }
  }
}

void DrawTriangle_Fill(int x0, int y0, int x1, int y1, int x2, int y2,
                       TGAImage& image, TGAColor color) {
  // 排序 y0 <= y1 <= y2
  if (y0 > y1) {
    std::swap(x0, x1);
    std::swap(y0, y1);
  }
  if (y0 > y2) {
    std::swap(x0, x2);
    std::swap(y0, y2);
  }
  if (y1 > y2) {
    std::swap(x1, x2);
    std::swap(y1, y2);
  }

  // 计算三角形总高
  int Total_Height = y2 - y0;
  if (Total_Height == 0)
    return;

  for (int i = 0; i < Total_Height; i++) {
    // y: 当前是第几行
    int y = y0 + i;

    // 上部分判断
    bool Second_Half = i > y1 - y0 || y0 == y1;

    // alpha: 在整个三角形的百分之多少
    float alpha = (float)i / Total_Height;

    // 各部分高度
    int Segment_Height = Second_Half ? y2 - y1 : y1 - y0;

    // beta: 在当前三角形的百分之多少
    float beta = (float)(i - (Second_Half ? y1 - y0 : 0)) / Segment_Height;

    // A 点
    int xA, yA;
    xA = (int)(x0 + (x2 - x0) * alpha);
    yA = (int)(y0 + (y2 - y0) * alpha);

    // B 点
    int xB, yB;
    if (!Second_Half) {
      xB = (int)(x0 + (x1 - x0) * beta);
      yB = (int)(y0 + (y1 - y0) * beta);
    } else {
      xB = (int)(x1 + (x2 - x1) * beta);
      yB = (int)(y1 + (y2 - y1) * beta);
    }

    if (xA > xB) {
      std::swap(xA, xB);
      std::swap(yB, yB);
    }

    for (int j = xA; j <= xB; j++) {
      image.set(j, y, color);
    }
  }
}

const TGAColor white = TGAColor{255, 255, 255, 255};
const TGAColor red = TGAColor{255, 0, 0, 255};
const TGAColor green = TGAColor{0, 255, 0, 255};
const TGAColor blue = TGAColor{0, 0, 255, 255};

const int width = 128;
const int height = 128;

// 3D to 2D
std::tuple<int, int> project(vec3 v) {
  return {(v.x + 1.) * width / 2, (v.y + 1.) * height / 2};
}

int main(int argc, char** argv) {
  TGAImage framebuffer(width, height, TGAImage::RGB);

  DrawTriangle_Fill(7, 45, 35, 100, 45, 60, framebuffer, red);
  DrawTriangle_Fill(120, 35, 90, 5, 45, 110, framebuffer, white);
  DrawTriangle_Fill(115, 83, 80, 90, 85, 120, framebuffer, green);

  framebuffer.flip_vertically();
  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
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

const int width = 800;
const int height = 800;

// 3D to 2D
std::tuple<int,int> project(vec3 v) { 
    return { (v.x + 1.) *  width / 2,
             (v.y + 1.) * height / 2 };
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " obj/model.obj" << std::endl;
        return 1;
    }

    Model model(argv[1]);
    TGAImage framebuffer(width, height, TGAImage::RGB);

    for (int i=0; i<model.nfaces(); i++) {
        auto [ax, ay] = project(model.vert(i, 0));
        auto [bx, by] = project(model.vert(i, 1));
        auto [cx, cy] = project(model.vert(i, 2));
        DrawLine(ax, ay, bx, by, framebuffer, red);
        DrawLine(bx, by, cx, cy, framebuffer, red);
        DrawLine(cx, cy, ax, ay, framebuffer, red);
    }

    for (int i=0; i<model.nverts(); i++) {
        vec3 v = model.vert(i);
        auto [x, y] = project(v);
        framebuffer.set(x, y, white);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}
#include <iostream>
#include <cmath>
#include "targa.h"

// ARGB
int32_t Black = 0xFF000000;
int32_t White = 0xFFFFFFFF;
int32_t Red = 0xFFFF0000;
int32_t Green = 0xFF00FF00;
int32_t Blue = 0xFF0000FF;

struct GridTexture {
    GridTexture() = default;

    ~GridTexture() = default;

    GridTexture(int32_t width, int32_t height, int32_t color) : width_(width), height_(height), color_(color) {
    }

    int32_t Sample(float x, float y) {
      if (y < 0.5) {
        if (x < 0.5) {
          return White;
        } else {
          return color_;
        }
      } else {
        if (x < 0.5) {
          return color_;
        } else {
          return White;
        }
      }

    }

    int32_t width_;
    int32_t height_;
    int32_t color_;

};

struct TextureCoordinate {
    float u;
    float v;
};

struct Point {
    int32_t x;
    int32_t y;
    float z;

    TextureCoordinate tex;
};

int comparPoints(const void * a, const void * b){
  Point* p1 = (Point*)a;
  Point* p2 = (Point*)b;
  return p1->y > p2->y;
}

struct Triangle {
    Triangle() = default;

    ~Triangle() = default;

    Triangle(Point p1, Point p2, Point p3, GridTexture t) : t_(t) {
      points[0] = p1;
      points[1] = p2;
      points[2] = p3;
      qsort(points, 3, sizeof(Point), comparPoints);
    }

    Point points[3];
    GridTexture t_;
};

struct Quad {
    Quad(Point &p1, Point &p2, Point &p3, Point &p4, GridTexture &t) {
      triangles_[0] = {p1, p2, p3, t};
      triangles_[1] = {p3, p4, p1, t};
      t_ = t;
    }

    Triangle triangles_[2];
    GridTexture t_;
};

const int32_t W = 320;
const int32_t H = 168;
int32_t framebuffer[W * H];

void Plot(Point& p, int32_t color) {
  framebuffer[(H - p.y) * W + p.x] = color;
}

void DrawLine(Point &a, Point &b, GridTexture &tex) {
  float dx = (b.x - a.x);
  float dy = (b.y - a.y);
  float steps;
  if (fabs(dx) >= fabs(dy))
    steps = fabs(dx);
  else
    steps = fabs(dy);
  dx = dx / steps;
  dy = dy / steps;
  float cursor_x = a.x;
  float cursor_y = a.y;
  int i = 1;
  while (i <= steps) {
    Point p;
    p.x = cursor_x;
    p.y = cursor_y;
    Plot(p, tex.Sample(p.tex.u,p.tex.v));
    cursor_x += dx;
    cursor_y += dy;
    i = i + 1;
  }
}


//           v3
//         *   *
//       *      *
//     *         *
//   *            *
//  v2*************v1
void FillBottomFlatTriangle(Point &v1, Point &v2, Point &v3, GridTexture &t) {
  float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
  float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

  float curx1 = v3.x;
  float curx2 = v3.x;

  for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
    Point p1;
    p1.x = curx1;
    p1.y = scanlineY;
    Point p2;
    p2.x = curx2;
    p2.y = scanlineY;
    DrawLine(p1, p2, t);

    curx1 -= invslope1;
    curx2 -= invslope2;
  }

}

//  v3*************v2
//   *            *
//     *         *
//       *      *
//         *   *
//           v1
void FillTopFlatTriangle(Point &v1, Point &v2, Point &v3, GridTexture &t) {
  float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
  float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

  float curx1 = v1.x;
  float curx2 = v1.x;

  for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
    Point p1;
    p1.x = curx1;
    p1.y = scanlineY;
    Point p2;
    p2.x = curx2;
    p2.y = scanlineY;
    DrawLine(p1, p2, t);
    curx1 += invslope1;
    curx2 += invslope2;
  }
}



void DrawTriangle(Triangle& t) {
  /* at first sort the three vertices by y-coordinate ascending so v3 is the topmost vertice */
//  sortVerticesAscendingByY();
  Point& v1 = t.points[0];
  Point& v2 = t.points[1];
  Point& v3 = t.points[2];

  /* here we know that v1.y <= v2.y <= v3.y */
  /* check for trivial case of bottom-flat triangle */
  if (v1.y == v2.y) {
    FillBottomFlatTriangle(v1, v2, v3, t.t_);
  } else if (v3.y == v2.y) {
    /* check for trivial case of top-flat triangle */
    FillTopFlatTriangle(v1, v2, v3, t.t_);
  } else {
    /* general case - split the triangle in a topflat and bottom-flat one */
    Point v4;
    v4.x = (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)),
    v4.y = v2.y;
    FillBottomFlatTriangle(v1, v2, v4, t.t_);
    FillTopFlatTriangle(v2, v4, v3, t.t_);
  }
}

void renderAffineQuad(Quad& quad) {
  DrawTriangle(quad.triangles_[0]);
  DrawTriangle(quad.triangles_[1]);
}

void RenderAffineTriangle(Triangle &triangle) {
   DrawTriangle(triangle);
}

int main() {
  for (size_t i = 0; i < W * H; i++) {
    framebuffer[i] = Green;
  }

  GridTexture redTexture = {10, 10, Red};
  Point p1;
  p1.x = 0;
  p1.y = 0;
  p1.z = 0;
  p1.tex.u = 0;
  p1.tex.v = 0;

  Point p2;
  p2.x = 10;
  p2.y = 10;
  p2.z = 0;
  p2.tex.u = 0;
  p2.tex.v = 1;

  Point p3;
  p3.x = 10;
  p3.y = 0;
  p3.z = 0;
  p3.tex.u = 1;
  p3.tex.v = 0;

  Triangle redTriangle(p1, p2, p3, redTexture);
  RenderAffineTriangle(redTriangle);


  GridTexture blueTexture = {10, 10, Blue};
  Point p4;
  p4.x = 30;
  p4.y = 30;
  p4.z = 0;
  p4.tex.u = 0;
  p4.tex.v = 0;

  Point p5;
  p5.x = 40;
  p5.y = 30;
  p5.z = 0;
  p5.tex.u = 0;
  p5.tex.v = 1;

  Point p6;
  p6.x = 30;
  p6.y = 40;
  p6.z = 0;
  p6.tex.u = 1;
  p6.tex.v = 0;

  Triangle blueTriangle(p4, p5, p6, blueTexture);
  RenderAffineTriangle(blueTriangle);



  Point p7;  p7.x = 100; p7.y = 100;
  Point p8;  p8.x = 100; p8.y = 150;
  Point p9;  p9.x = 150; p9.y = 150;
  Point p10; p10.x = 150; p10.y = 100;
  Quad quad(p7, p8, p9, p10, redTexture);

  GridTexture blackTexture(1, 1, Black);
  renderAffineQuad(quad);
  DrawLine(p7, p9, blackTexture);
  DrawLine(p7, p8, blackTexture);
  DrawLine(p8, p9, blackTexture);
  DrawLine(p9, p10, blackTexture);
  DrawLine(p10, p7, blackTexture);




  tga_write_bgr("test.tga", (uint8_t *) framebuffer, W, H, 32);
  return 0;
}
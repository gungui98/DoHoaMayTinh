#include "rasterizer.h"

using namespace std;

namespace CGL {

  RasterizerImp::RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
    size_t width, size_t height,
    unsigned int sample_rate) {
    this->psm = psm;
    this->lsm = lsm;
    this->width = width;
    this->height = height;
    this->sample_rate = sample_rate;

    sample_buffer.resize(width * height * sample_rate, Color::White);
  }

  // Used by rasterize_point and rasterize_line
  void RasterizerImp::fill_pixel(size_t x, size_t y, Color c, size_t dx, size_t dy) {
    // Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
    // NOTE: You are not required to implement proper supersampling for points and lines
    // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)
    size_t sqrt_rate = sqrt(sample_rate);
    sample_buffer[((dx * sqrt_rate + dy) * height + y) * width + x] = c;
  }

  // Rasterize a point: simple example to help you start familiarizing
  // yourself with the starter code.
  //
  void RasterizerImp::rasterize_point(float x, float y, Color color) {
    // fill in the nearest pixel
    int sx = (int)floor(x);
    int sy = (int)floor(y);

    // check bounds
    if (sx < 0 || sx >= width) return;
    if (sy < 0 || sy >= height) return;

    size_t sqrt_rate = sqrt(sample_rate);
    for (size_t dx = 0; dx < sqrt_rate; dx++)
      for (size_t dy = 0; dy < sqrt_rate; dy++)
        fill_pixel(sx, sy, color, dx, dy);
  }

  // Rasterize a line.
  void RasterizerImp::rasterize_line(float x0, float y0,
    float x1, float y1,
    Color color) {
    if (x0 > x1) {
      swap(x0, x1); swap(y0, y1);
    }

    float pt[] = { x0,y0 };
    float m = (y1 - y0) / (x1 - x0);
    float dpt[] = { 1,m };
    int steep = abs(m) > 1;
    if (steep) {
      dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
      dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
    }

    while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0)) {
      rasterize_point(pt[0], pt[1], color);
      pt[0] += dpt[0]; pt[1] += dpt[1];
    }
  }

  bool is_in_triangle(float x, float y,
    float x0, float y0,
    float x1, float y1,
    float x2, float y2) {
    float l0 = (x-x0)*(y1-y0) - (y-y0)*(x1-x0);
    float l1 = (x-x1)*(y2-y1) - (y-y1)*(x2-x1);
    float l2 = (x-x2)*(y0-y2) - (y-y2)*(x0-x2);
    return (l0 >= 0 && l1 >= 0 && l2 >= 0) || (l0 <= 0 && l1 <= 0 && l2 <= 0);
  }

  // Rasterize a triangle.
  void RasterizerImp::rasterize_triangle(float x0, float y0,
    float x1, float y1,
    float x2, float y2,
    Color color) {
    // Task 1: Implement basic triangle rasterization here, no supersampling
    float xmin = floor(fmin(x0, fmin(x1, x2))), xmax = floor(fmax(x0, fmax(x1, x2)));
    float ymin = floor(fmin(y0, fmin(y1, y2))), ymax = floor(fmax(y0, fmax(y1, y2)));
    // Task 2: Update to implement super-sampled rasterization
    float x, y;
    size_t sqrt_rate = sqrt(sample_rate);
    float sample_size = 1.0f / sqrt_rate;
    for (x = xmin; floor(x) <= xmax; x += 1.0f) {
      for (y = ymin; floor(y) <= ymax; y += 1.0f) {
        for (size_t dx = 0; dx < sqrt_rate; dx++)
          for (size_t dy = 0; dy < sqrt_rate; dy++) {
            if (is_in_triangle(x + (dx+.5f)*sample_size, y + (dy+.5f)*sample_size, x0, y0, x1, y1, x2, y2)) {
              fill_pixel(x, y, color, dx, dy);
            }
          }
      }
    }
  }


  void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
    float x1, float y1, Color c1,
    float x2, float y2, Color c2)
  {
    // Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
    float xmin = floor(fmin(x0, fmin(x1, x2))), xmax = floor(fmax(x0, fmax(x1, x2)));
    float ymin = floor(fmin(y0, fmin(y1, y2))), ymax = floor(fmax(y0, fmax(y1, y2)));
    float x, y, alpha, beta, gamma;
    size_t sqrt_rate = sqrt(sample_rate);
    float sample_size = 1.0f / sqrt_rate;
    for (x = xmin; floor(x) <= xmax; x += 1.0f)
      for (y = ymin; floor(y) <= ymax; y += 1.0f)
        for (size_t dx = 0; dx < sqrt_rate; dx++)
          for (size_t dy = 0; dy < sqrt_rate; dy++)
            if (is_in_triangle(x + (dx+.5f)*sample_size, y + (dy+.5f)*sample_size, x0, y0, x1, y1, x2, y2)) {
              // (x, y) = alpha*(x0, y0) + beta*(x1, y1) + gamma*(x2, y2)
              alpha = ((y-y1)*(x2-x1) - (x-x1)*(y2-y1)) / ((y0-y1)*(x2-x1) - (y2-y1)*(x0-x1));
              beta = ((y-y2)*(x0-x2) - (x-x2)*(y0-y2)) / ((y1-y2)*(x0-x2) - (y0-y2)*(x1-x2));
              gamma = 1.f - alpha - beta;
              Color color(
                alpha*c0.r + beta*c1.r + gamma*c2.r,
                alpha*c0.g + beta*c1.g + gamma*c2.g,
                alpha*c0.b + beta*c1.b + gamma*c2.b
              );
              fill_pixel(x, y, color);
            }
  }


  void RasterizerImp::rasterize_textured_triangle(float x0, float y0, float u0, float v0,
    float x1, float y1, float u1, float v1,
    float x2, float y2, float u2, float v2,
    Texture& tex)
  {
    // TODO: Task 5: Fill in the SampleParams struct and pass it to the tex.sample function.
    // TODO: Task 6: Set the correct barycentric differentials in the SampleParams struct.
    // Hint: You can reuse code from rasterize_triangle/rasterize_interpolated_color_triangle




  }

  void RasterizerImp::set_sample_rate(unsigned int rate) {
    // Task 2: You may want to update this function for supersampling support

    this->sample_rate = rate;


    this->sample_buffer.resize(width * height * sample_rate, Color::White);
  }


  void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
    size_t width, size_t height)
  {
    // Task 2: You may want to update this function for supersampling support

    this->width = width;
    this->height = height;
    this->rgb_framebuffer_target = rgb_framebuffer;


    this->sample_buffer.resize(width * height * sample_rate, Color::White);
  }


  void RasterizerImp::clear_buffers() {
    std::fill(rgb_framebuffer_target, rgb_framebuffer_target + 3 * width * height, 255);
    std::fill(sample_buffer.begin(), sample_buffer.end(), Color::White);
  }


  // This function is called at the end of rasterizing all elements of the
  // SVG file.  If you use a supersample buffer to rasterize SVG elements
  // for antialising, you could use this call to fill the target framebuffer
  // pixels from the supersample buffer data.
  //
  void RasterizerImp::resolve_to_framebuffer() {
    // Task 2: You will likely want to update this function for supersampling support


    int sqrt_rate = sqrt(sample_rate);
    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        Color col(0,0,0);
        for (int dx = 0; dx < sqrt_rate; dx++)
          for (int dy = 0; dy < sqrt_rate; dy++) {
            col += sample_buffer[((dx * sqrt_rate + dy) * height + y) * width + x];
          }
        col *= 1.0f / sample_rate;
        for (int k = 0; k < 3; ++k) {
          this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
        }
      }
    }

  }

  Rasterizer::~Rasterizer() { }


}// CGL

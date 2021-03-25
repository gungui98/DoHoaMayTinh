#pragma once

#include "CGL/CGL.h"
#include "CGL/color.h"
#include "CGL/vector3D.h"
#include <vector>
#include "svg.h"

namespace CGL {

  class Rasterizer {
  public:
    virtual ~Rasterizer() = 0;

    virtual unsigned int get_sample_rate() = 0;
    virtual void set_sample_rate(unsigned int rate) = 0;
    virtual void set_psm(PixelSampleMethod p) = 0;
    virtual void set_lsm(LevelSampleMethod l) = 0;

    // Rasterize a point
    virtual void rasterize_point(float x, float y, Color color) = 0;

    // Rasterize a line
    virtual void rasterize_line(float x0, float y0,
      float x1, float y1,
      Color color) = 0;

    // Rasterize a triangle
    // P0 = (x0, y0)
    // P1 = (x1, y1)
    // P2 = (x2, y2)
    virtual void rasterize_triangle(float x0, float y0,
      float x1, float y1,
      float x2, float y2,
      Color color) = 0;

    virtual void rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
      float x1, float y1, Color c1,
      float x2, float y2, Color c2) = 0;

    virtual void rasterize_textured_triangle(float x0, float y0, float u0, float v0,
      float x1, float y1, float u1, float v1,
      float x2, float y2, float u2, float v2,
      Texture& tex) = 0;

    // This function sets the framebuffer target.  The block of memory
    // for the framebuffer contains 3 * width * height values for an RGB
    // pixel framebuffer with 8-bits per color channel.
    virtual void set_framebuffer_target(unsigned char* rgb_framebuffer,
      size_t width, size_t height) = 0;

    virtual void clear_buffers() = 0;

    // This function fills the target framebuffer with the
    // rasterized samples (including subpixel super-samples, if relevant)
    // in preparation for posting pixels to the screen.
    virtual void resolve_to_framebuffer() = 0;
  };

  class RasterizerImp : public Rasterizer {
  private:
    // The total number of samples
    unsigned int sample_rate;

    // Constants indicating which sampling methods are used
    PixelSampleMethod psm;
    LevelSampleMethod lsm;

    // Width & Height of the image and the output
    size_t width, height;

    // The target pixel framebuffer connected to display. There are
    // 3 x width * height values in this RGB pixel array.
    unsigned char* rgb_framebuffer_target;

    // The internal color sample buffer, contains *all samples*
    // Organized in a matrix, stored in a 1-d vector
    // For example, Position [x,y] = [width * y + x]
    // The number of elements in buffer = width * height * sample_rate
    std::vector<Color> sample_buffer;

  public:

    RasterizerImp(PixelSampleMethod psm, LevelSampleMethod lsm,
      size_t width, size_t height, unsigned int sample_rate);


    // Rasterize a point
    // P0 = (x, y)
    void rasterize_point(float x, float y, Color color);

    // Rasterize a line
    // From P0 = (x0, y0)
    // To   P1 = (x1, y1)
    void rasterize_line(float x0, float y0,
      float x1, float y1,
      Color color);

    // Rasterize a triangle
    // P0 = (x0, y0)
    // P1 = (x1, y1)
    // P2 = (x2, y2)
    void rasterize_triangle(float x0, float y0,
      float x1, float y1,
      float x2, float y2,
      Color color);

    void rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
      float x1, float y1, Color c1,
      float x2, float y2, Color c2);

    void rasterize_textured_triangle(float x0, float y0, float u0, float v0,
      float x1, float y1, float u1, float v1,
      float x2, float y2, float u2, float v2,
      Texture& tex);

    unsigned int get_sample_rate() { return sample_rate; }

    void set_sample_rate(unsigned int rate);

    void set_psm(PixelSampleMethod p) { psm = p; }
    void set_lsm(LevelSampleMethod l) { lsm = l; }

    // Fill a pixel, which may contain multiple samples
    void fill_pixel(size_t x, size_t y, Color c);

    // This function sets the framebuffer target.  The block of memory
    // for the framebuffer contains 3 * width * height values for an RGB
    // pixel framebuffer with 8-bits per color channel.
    virtual void set_framebuffer_target(unsigned char* rgb_framebuffer,
      size_t width, size_t height);

    virtual void clear_buffers();

    // This function fills the target framebuffer with the
    // rasterized samples (including subpixel super-samples, if relevant)
    // in preparation for posting pixels to the screen.
    virtual void resolve_to_framebuffer();

  };


}

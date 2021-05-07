#include "rasterizer.h"
using namespace std;

const double eps = 1e-9;

float progress(float curr, float start, float end) {
    return abs(curr - start) / abs(end - start);
}

float sign(float x0, float y0, float x1, float y1, float x2, float y2) {
    return (x0 - x2) * (y1 - y2) - (x1 - x2) * (y0 - y2);
}

float L(float x, float y, float xP, float yP, float xQ, float yQ) {
    return -(x - xP) * (yQ - yP) + (y - yP) * (xQ - xP);
}

bool isInTriangle (float xpt, float ypt, float x0, float y0, float x1, float y1, float x2, float y2)
{
    float d0, d1, d2;
    bool has_neg, has_pos;

    d0 = sign(xpt, ypt, x0, y0, x1, y1);
    d1 = sign(xpt, ypt, x1, y1, x2, y2);
    d2 = sign(xpt, ypt, x2, y2, x0, y0);

    has_neg = (d0 <= 0) || (d1 <= 0) || (d2 <= 0);
    has_pos = (d0 >= 0) || (d1 >= 0) || (d2 >= 0);
    return !(has_neg && has_pos);
}

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
    void RasterizerImp::fill_pixel(size_t x, size_t y, size_t dx, size_t dy, Color c) {
        // TODO: Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
        // NOTE: You are not required to implement proper supersampling for points and lines
        // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)
        size_t sqrt_rate = sqrt(sample_rate);
        size_t start_point = (width * height) * (dx * sqrt_rate + dy);

        sample_buffer[start_point + y * width + x] = c;
    }




    // Rasterize a point: simple example to help you start familiarizing
    // yourself with the starter code.
    //
    void RasterizerImp::rasterize_point(float x, float y, Color color) {
        // fill in the nearest pixel
        int sx = (int) floor(x);
        int sy = (int) floor(y);

        // check bounds
        if (sx < 0 || sx >= width) return;
        if (sy < 0 || sy >= height) return;
        size_t sqrt_rate = sqrt(sample_rate);
        for (size_t dx = 0; dx < sqrt_rate; dx++) {
            for (size_t dy = 0; dy < sqrt_rate; dy++) {
                fill_pixel(sx, sy, dx, dy, color);

            }
        }
    }

    // Rasterize a line.
    void RasterizerImp::rasterize_line(float x0, float y0,
                                       float x1, float y1,
                                       Color color) {
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }

        float pt[] = {x0, y0};
        float m = (y1 - y0) / (x1 - x0);
        float dpt[] = {1, m};
        int steep = abs(m) > 1;
        if (steep) {
            dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
            dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
        }

        while (floor(pt[0]) <= floor(x1) && abs(pt[1] - y0) <= abs(y1 - y0)) {
            rasterize_point(pt[0], pt[1], color);
            pt[0] += dpt[0];
            pt[1] += dpt[1];
        }
    }
    // Rasterize a triangle.
    void RasterizerImp::rasterize_triangle(float x0, float y0,
                                           float x1, float y1,
                                           float x2, float y2,
                                           Color color) {
        // TODO: Task 1: Implement basic triangle rasterization here, no supersampling
        float xmin = min(x0, min(x1, x2));
        float ymin = min(y0, min(y1, y2));
        float xmax = max(x0, max(x1, x2));
        float ymax = max(y0, max(y1, y2));
        // TODO: Task 2: Update to implement super-sampled rasterization
        for (float x = floor(xmin); floor(x) <= xmax; x += 1.0f) {
            for (float y = floor(ymin); floor(y) <= ymax; y += 1.0f) {
                int sqrt_rate = sqrt(sample_rate);
                float sample_size = 1.0f / float(sqrt_rate);
                int number_in_triangle = 0;
                for (int dx = 0; dx < sqrt_rate; dx++) {
                    for (int dy = 0; dy < sqrt_rate; dy++) {
                        if (isInTriangle(x + (dx + 0.5f) * sample_size, y +(dy + 0.5) * sample_size, x0, y0, x1, y1, x2, y2)) {
                            RasterizerImp::fill_pixel(x, y, dx, dy, color);
                        }
                    }
                }
            }
        }



    }


    void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
                                                              float x1, float y1, Color c1,
                                                              float x2, float y2, Color c2)
    {
        // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
        // Hint: You can reuse code from rasterize_triangle
        float xmin = min(x0, min(x1, x2));
        float ymin = min(y0, min(y1, y2));
        float xmax = max(x0, max(x1, x2));
        float ymax = max(y0, max(y1, y2));
        // TODO: Task 2: Update to implement super-sampled rasterization
        for (float x = floor(xmin); floor(x) <= xmax + 1.0f; x += 1.0f) {
            for (float y = floor(ymin); floor(y) <= ymax + 1.0f; y += 1.0f) {
                int sqrt_rate = sqrt(sample_rate);
                float sample_size = 1.0f / float(sqrt_rate);
                int number_in_triangle = 0;
                for (int dx = 0; dx < sqrt_rate; dx++) {
                    for (int dy = 0; dy < sqrt_rate; dy++) {
                        if (isInTriangle(x + (dx + 0.5f) * sample_size, y + (dy + 0.5) * sample_size, x0, y0, x1, y1, x2, y2)) {
                            float X = x + dx + 0.5;
                            float Y = y + dy + 0.5;
                            float alpha = L(X, Y, x1, y1, x2, y2) / L(x0, y0, x1, y1, x2, y2);
                            float beta = L(X, Y, x0, y0, x2, y2) / L(x1, y1, x0, y0, x2, y2);
                            float gamma = 1 - alpha - beta;
                            Color color = alpha * c0 + beta * c1 + gamma * c2;
                            RasterizerImp::fill_pixel(x, y, dx, dy, color);
                        }
                    }
                }
            }
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
        // TODO: Task 2: You may want to update this function for supersampling support

        this->sample_rate = rate;


        this->sample_buffer.resize(width * height * rate, Color::White);
    }


    void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
                                               size_t width, size_t height)
    {
        // TODO: Task 2: You may want to update this function for supersampling support

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
        // TODO: Task 2: You will likely want to update this function for supersampling support
        for (int x = 0; x < width; ++x) {
            for (int y = 0; y < height; ++y) {
                Color totalCol = Color(0, 0, 0);
                size_t sqrt_rate = sqrt(sample_rate);
                for (int dx = 0; dx < sqrt_rate; ++dx) {
                    for (int dy = 0; dy < sqrt_rate; ++dy) {
                        size_t start_point = (width * height) * (dx * sqrt_rate + dy);

                        totalCol += sample_buffer[start_point + y * width + x];
                    }
                }
                totalCol *= 1.0f / sample_rate;
                for (int k = 0; k < 3; ++k) {
                    this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&totalCol.r)[k] * 255;
                }
            }
        }
    }


    Rasterizer::~Rasterizer() { }


}// CGL


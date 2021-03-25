#include "rasterizer.h"
using namespace std;

const double eps = 1e-9;

float progress(float curr, float start, float end) {
    return abs(curr - start) / abs(end - start);
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
    void RasterizerImp::fill_pixel(size_t x, size_t y, Color c) {
        // TODO: Task 2: You might need to this function to fix points and lines (such as the black rectangle border in test4.svg)
        // NOTE: You are not required to implement proper supersampling for points and lines
        // It is sufficient to use the same color for all supersamples of a pixel for points and lines (not triangles)


        sample_buffer[y * width + x] = c;
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

        fill_pixel(sx, sy, color);
        return;
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
    void RasterizerImp::get_rasterize_line_attributes(float x0, float y0, float x1, float y1, float dpt[2]) {
        float m = (y1 - y0) / (x1 - x0);
        float pt[] = {x0, y0};
        dpt[0] = 1;
        dpt[1] = m;
        int steep = abs(m) > 1;
        if (steep) {
            dpt[0] = x1 == x0 ? 0 : 1 / abs(m);
            dpt[1] = x1 == x0 ? (y1 - y0) / abs(y1 - y0) : m / abs(m);
        }
    }
    // Rasterize a triangle.
    void RasterizerImp::rasterize_triangle(float x0, float y0,
                                           float x1, float y1,
                                           float x2, float y2,
                                           Color color) {
        //printf("DRAW TRIANGLE x0 = %f y0 = %f x1 = %f y1 = %f x2 = %f y2 = %f\n", x0, y0, x1, y1, x2, y2);
        if (x0 > x1) {
            swap(x0, x1);
            swap(y0, y1);
        }
        if (x0 > x2) {
            swap(x0, x2);
            swap(y0, y2);
        }
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }
        // TODO: Task 1: Implement basic triangle rasterization here, no supersampling
        float points[][2] = {{x0, y0}, {x1, y1}, {x2, y2}};

        for (int i = 0; i < 3; i++) {
            int i_next = (i + 1) % 3;
            RasterizerImp::rasterize_line(points[i][0], points[i][1], points[i_next][0], points[i_next][1], color);
        }

        float pt1[] = {points[0][0], points[0][1]};
        float dpt1[2];
        int dim1 = (points[1][0] == points[0][0]) ? 1 : 0;
        get_rasterize_line_attributes(points[0][0], points[0][1], points[1][0], points[1][1], dpt1);

        float pt2[] = {points[0][0], points[0][1]};
        float dpt2[2];
        int dim2 = (points[2][0] == points[0][0]) ? 1 : 0;
        get_rasterize_line_attributes(points[0][0], points[0][1], points[2][0], points[2][1], dpt2);


        while (true) {
            float progress1 = progress(pt1[dim1], points[0][dim1], points[1][dim1]);
            float progress2 = progress(pt2[dim2], points[0][dim2], points[2][dim2]);
            if (progress1 + eps >= 1 &&
                progress2 + eps >= 1) {
                break;
            }
            if (progress1 < progress2) {
                pt1[0] += dpt1[0];
                pt1[1] += dpt1[1];
                RasterizerImp::rasterize_line(pt1[0], pt1[1], points[2][0], points[2][1], color);
            } else {
                pt2[0] += dpt2[0];
                pt2[1] += dpt2[1];
                RasterizerImp::rasterize_line(pt2[0], pt2[1], points[1][0], points[1][1], color);
            }
            RasterizerImp::rasterize_line(pt1[0], pt1[1], pt2[0], pt2[1], color);
        }
        // TODO: Task 2: Update to implement super-sampled rasterization



    }


    void RasterizerImp::rasterize_interpolated_color_triangle(float x0, float y0, Color c0,
                                                              float x1, float y1, Color c1,
                                                              float x2, float y2, Color c2)
    {
        // TODO: Task 4: Rasterize the triangle, calculating barycentric coordinates and using them to interpolate vertex colors across the triangle
        // Hint: You can reuse code from rasterize_triangle



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


        this->sample_buffer.resize(width * height, Color::White);
    }


    void RasterizerImp::set_framebuffer_target(unsigned char* rgb_framebuffer,
                                               size_t width, size_t height)
    {
        // TODO: Task 2: You may want to update this function for supersampling support

        this->width = width;
        this->height = height;
        this->rgb_framebuffer_target = rgb_framebuffer;


        this->sample_buffer.resize(width * height, Color::White);
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
                Color col = sample_buffer[y * width + x];

                for (int k = 0; k < 3; ++k) {
                    this->rgb_framebuffer_target[3 * (y * width + x) + k] = (&col.r)[k] * 255;
                }
            }
        }

    }

    Rasterizer::~Rasterizer() { }


}// CGL


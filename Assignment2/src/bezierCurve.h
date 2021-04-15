#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "CGL/CGL.h"
#include "CGL/renderer.h"

using namespace std;

namespace CGL {

  class BezierCurve : public Renderer {
  public:

    explicit BezierCurve(int numControlPoints);

    void loadControlPoints(FILE *file);

    /**
     * Evaluates one step of the de Casteljau's algorithm using the given points and
     * the scalar parameter t (class member)
     *
     * @param points A vector of points in 2D
     * @return A vector containing intermediate points or the final interpolated vector
     */
    vector<Vector2D> evaluateStep(std::vector<Vector2D> const &points);

    void drawCurve();

    // inherited Renderer interface functions
    void render() override;

    void init() override {}

    void resize(size_t w, size_t h) override {
        width = w;
        height = h;
    }

    string name() override { return "Bezier Curve"; }

    string info() override { return ""; }

    void key_event(char key) override;

    void cursor_event(float x, float y, unsigned char keys) override;

    void scroll_event(float offset_x, float offset_y) override;

  protected:

    /**
     * The original control points of the Bezier curve
     */
    std::vector<Vector2D> controlPoints;

    /**
     * Value between 0 and 1 to evaluate the Bezier curve at
     */
    float t;

    int numControlPoints;

    size_t width, height;

    int selected = -1;

    int eval_level = 0;

    int drawingCurve = 0;

  };

}

#endif /* BEZIERCURVE_H */

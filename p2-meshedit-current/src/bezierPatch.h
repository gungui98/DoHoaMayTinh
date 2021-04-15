#ifndef BEZIERPATCH_H
#define BEZIERPATCH_H

#include "CGL/CGL.h"
#include "mesh.h"

namespace CGL {

  class BezierPatch {
  public:

    BezierPatch();

    void add2mesh(Polymesh *mesh) const;

    void loadControlPoints(FILE *file);

  protected:

    /**
     * The original control points of the Bezier patch
     */
    std::vector< std::vector<Vector3D> > controlPoints;

    /**
     * Evaluates one step of the de Casteljau's algorithm using the given points and the scalar parameter t
     *
     * @param points    A vector of points in 3D
     * @param t         Scalar interpolation parameter
     * @return A vector containing intermediate points or the final interpolated vector
     */
    std::vector<Vector3D> evaluateStep(std::vector<Vector3D> const &points, double t) const;

    /**
     * Fully evaluates de Casteljau's algorithm for a vector of points at scalar parameter t
     *
     * @param points    A vector of points in 3D
     * @param t         Scalar interpolation parameter
     * @return Final interpolated vector
     */
    Vector3D evaluate1D(std::vector<Vector3D> const &points, double t) const;

    /**
     * Evaluates the Bezier patch at parameter (u, v)
     *
     * @param u         Scalar interpolation parameter
     * @param v         Scalar interpolation parameter (along the other axis)
     * @return Final interpolated vector
     */
    Vector3D evaluate(double u, double v) const;

    void addTriangle(Polymesh *mesh, const Vector3D &v0, const Vector3D &v1, const Vector3D &v2) const;

  };

}

#endif /* BEZIERPATCH_H */

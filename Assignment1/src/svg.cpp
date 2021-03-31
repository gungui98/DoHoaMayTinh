// Original file Copyright CMU462 Fall 2015:
// Kayvon Fatahalian, Keenan Crane,
// Sky Gao, Bryce Summers, Michael Choquette.
#include "svg.h"
//#include "CGL/lodepng.h"

#include "drawrend.h"
#include "transforms.h"
#include "triangulation.h"
#include <iostream>

#include "CGL/lodepng.h"

namespace CGL {

Group::~Group() {
  for (size_t i = 0; i < elements.size(); i++) {
    delete elements[i];
  } elements.clear();
}

SVG::~SVG() {
  for (size_t i = 0; i < elements.size(); i++) {
    delete elements[i];
  } elements.clear();
}

// Draw routines //

void Triangle::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D p0_scr = global_transform * p0_svg;
  Vector2D p1_scr = global_transform * p1_svg;
  Vector2D p2_scr = global_transform * p2_svg;

  // draw fill. Here the color field is empty, since children
  // export their own more sophisticated color() method.
  dr->rasterize_triangle( p0_scr.x, p0_scr.y, p1_scr.x, p1_scr.y, p2_scr.x, p2_scr.y, Color());
}

void InterpolatedColorTriangle::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D p0_scr = global_transform * p0_svg;
  Vector2D p1_scr = global_transform * p1_svg;
  Vector2D p2_scr = global_transform * p2_svg;

  // draw fill. Here the color field is empty, since children
  // export their own more sophisticated color() method.
  dr->rasterize_interpolated_color_triangle(p0_scr.x, p0_scr.y, this->p0_col,
                                            p1_scr.x, p1_scr.y, this->p1_col,
                                            p2_scr.x, p2_scr.y, this->p2_col);
}

void TexturedTriangle::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D p0_scr = global_transform * p0_svg;
  Vector2D p1_scr = global_transform * p1_svg;
  Vector2D p2_scr = global_transform * p2_svg;

  // draw fill. Here the color field is empty, since children
  // export their own more sophisticated color() method.
  dr->rasterize_textured_triangle(p0_scr.x, p0_scr.y, this->p0_uv.x, this->p0_uv.y,
                                  p1_scr.x, p1_scr.y, this->p1_uv.x, this->p1_uv.y,
                                  p2_scr.x, p2_scr.y, this->p2_uv.x, this->p2_uv.y,
                                  *this->tex);
}

void Group::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  for (int i = 0; i < elements.size(); ++i)
    elements[i]->draw(dr, global_transform);
}

void Point::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;
  Vector2D p = global_transform * position;
  dr->rasterize_point(p.x, p.y, style.fillColor);
}

void Line::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Vector2D f = global_transform * from, t = global_transform * to;
  if (style.strokeVisible) {
    dr->rasterize_line(f.x, f.y, t.x, t.y, style.strokeColor);
  }
}

void Polyline::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c = style.strokeColor;

  int nPoints = points.size();
  for( int i = 0; i < nPoints - 1; i++ ) {
    Vector2D p0 = global_transform * points[(i+0) % nPoints];
    Vector2D p1 = global_transform * points[(i+1) % nPoints];
    dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
  }
}

void Rect::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c;

  // draw as two triangles
  float x =  position.x, y =  position.y;
  float w = dimension.x, h = dimension.y;

  Vector2D p0 = global_transform * Vector2D(   x   ,   y   );
  Vector2D p1 = global_transform * Vector2D( x + w ,   y   );
  Vector2D p2 = global_transform * Vector2D(   x   , y + h );
  Vector2D p3 = global_transform * Vector2D( x + w , y + h );

  // draw fill
  c = style.fillColor;
  dr->rasterize_triangle( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
  dr->rasterize_triangle( p2.x, p2.y, p1.x, p1.y, p3.x, p3.y, c );

  // draw outline
  if (style.strokeVisible) {
    c = style.strokeColor;
    dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    dr->rasterize_line( p1.x, p1.y, p3.x, p3.y, c );
    dr->rasterize_line( p3.x, p3.y, p2.x, p2.y, c );
    dr->rasterize_line( p2.x, p2.y, p0.x, p0.y, c );
  }
}

void Polygon::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;

  Color c;

  // draw fill
  c = style.fillColor;

  // triangulate
  std::vector<Vector2D> triangles;
  triangulate( *this, triangles );

  // draw as triangles
  for (size_t i = 0; i < triangles.size(); i += 3) {
    Vector2D p0 = global_transform * triangles[i + 0];
    Vector2D p1 = global_transform * triangles[i + 1];
    Vector2D p2 = global_transform * triangles[i + 2];
    dr->rasterize_triangle( p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, c );
  }

  // draw outline
  if (style.strokeVisible) {
    c = style.strokeColor;
    int nPoints = points.size();
    for( int i = 0; i < nPoints; i++ ) {
      Vector2D p0 = global_transform * points[(i+0) % nPoints];
      Vector2D p1 = global_transform * points[(i+1) % nPoints];
      dr->rasterize_line( p0.x, p0.y, p1.x, p1.y, c );
    }
  }
}

void Image::draw(Rasterizer*dr, Matrix3x3 global_transform) {
  global_transform = global_transform * transform;
  Vector2D p0 = global_transform * position;
  Vector2D p1 = global_transform * (position + dimension);

  for (int x = floor(p0.x); x <= floor(p1.x); ++x) {
    for (int y = floor(p0.y); y <= floor(p1.y); ++y) {
      Color col = tex.sample_bilinear(Vector2D((x+.5-p0.x)/(p1.x-p0.x+1), (y+.5-p0.y)/(p1.y-p0.y+1)));
      dr->rasterize_point(x,y,col);
    }
  }
}

} // namespace CGL

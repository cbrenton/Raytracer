/**
 * Chris Brenton
 * Half sphere object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __SEMI_H__
#define __SEMI_H__

#include "Geometry.h"
#include "Plane.h"

class Semi : public Geometry {
   public:
      Semi() : Geometry() {};
      Semi(std::istream& input);
      Semi(vec3_t _loc, float _rad);
      ~Semi();
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      vec3_t getNormal(vec3_t point);
      void debug();
      float radius;
   protected:
      bool isTop;
      Plane *bisect;
};

#endif

/**
 * Sterling Hirsh
 * Triangle object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Geometry.h"

class Triangle : public Geometry {
   public:
      Triangle(std::istream& input);
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      void debug();
   private:
      // Corner1 is Geometry::location
      vec3_t corner2;
      vec3_t corner3;
};

#endif



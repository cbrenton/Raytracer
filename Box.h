/**
 * Sterling Hirsh
 * Box object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __BOX_H__
#define __BOX_H__

#include "Geometry.h"
#include "Plane.h"

class Box : public Geometry {
   public:
      Box(std::istream& input);
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      void debug();
   private:
      // Corner1 is Geometry::location
      Vector3D corner2;
      Plane pLeft, pRight, pBottom, pTop, pFront, pBack;
};

#endif


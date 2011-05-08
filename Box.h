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
      ~Box();
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      inline void debug()
      {
         printf("Box: { <%f, %f, %f>, <%f, %f, %f>\n",
               location.x(), location.y(), location.z(),
               corner2.x(), corner2.y(), corner2.z());
      }
   protected:
      // Corner1 is Geometry::location
      vec3_t corner2;
      Plane *pLeft, *pRight, *pBottom, *pTop, *pFront, *pBack;
};

#endif


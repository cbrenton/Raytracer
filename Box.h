/**
 * Sterling Hirsh
 * Box object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __BOX_H__
#define __BOX_H__

#include "Geometry.h"

class Plane;

class Box : public Geometry {
   public:
      Box() {};
      Box(vec3_t c1, vec3_t c2);
      Box(std::istream& input);
      ~Box();
      Box *bBox();
      Box *combine(Box *b1, Box *b2);
      vec3_t getNormal(vec3_t point);
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
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


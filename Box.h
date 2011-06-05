/**
 * Sterling Hirsh
 * Box object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __BOX_H__
#define __BOX_H__

#include "Geometry.h"

#define EPSILON 0.001f

class Plane;

class Box : public Geometry {
   public:
      Box() {thisBBox = this;};
      Box(vec3_t c1, vec3_t c2);
      Box(std::istream& input);
      ~Box();
      Box *bBox();
      void combine(Box *b1, Box *b2);
      bool closeEnough(vec_t a, vec_t b);
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


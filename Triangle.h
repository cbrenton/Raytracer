/**
 * Sterling Hirsh
 * Triangle object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Geometry.h"
#include "cuda/Intersection_K.h"
#include "Box.h"

class Triangle : public Geometry
{
   public:
      Triangle(std::istream& input);
      Triangle(vec3_t c1, vec3_t c2, vec3_t c3);
      Box *bBox();
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      vec3_t getNormal(vec3_t point = vec3_t(0, 0, 0));
      bool contains(vec3_t pt);
      bool isNeighbor(Triangle *other);
      triangle_dev_t getStruct();
      void debug();
   protected:
      // Corner1 is Geometry::location
      bool matSet;
      vec3_t corner2;
      vec3_t corner3;
};

#endif

/**
 * Sterling Hirsh
 * Plane object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __Plane_H__
#define __Plane_H__

#include "Geometry.h"
#include "Box.h"

class Plane : public Geometry {
   public:
      Plane() {};
      Plane(vec3_t normal, float d);
      Plane(std::istream& input);
      ~Plane() {};
      bool hitBVH(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      vec3_t getNormal(vec3_t point);
      void debug();
   protected:
      // location is the plane's normal vector.
      float planeOffset; // This is D in the plane equasion.
};

#endif

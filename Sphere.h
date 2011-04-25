/**
 * Sterling Hirsh
 * Sphere object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Geometry.h"

class Sphere : public Geometry {
   public:
      Sphere() : Geometry() {};
      Sphere(std::istream& input);
      Sphere(Vector3D _loc, float _rad);
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      Vector3D getNormal(Vector3D point);
      void debug();
      float radius;
   private:
};

#endif

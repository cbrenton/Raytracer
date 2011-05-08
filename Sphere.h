/**
 * * Sterling Hirsh
 * * Sphere object for raytracing.
 * * 473
 * * 4/5/11
 * */

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Geometry.h"
#include "Intersection_K.h"

class Sphere : public Geometry
{
   public:
      Sphere() : Geometry() {};
      Sphere(std::istream& input);
      Sphere(vec3_t _loc, float _rad);
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      vec3_t getNormal(vec3_t point);
      sphere_dev_t getStruct();
      void debug();
      float radius;
};

#endif



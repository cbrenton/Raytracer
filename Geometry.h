/**
 * Sterling Hirsh
 * Geometry Base class
 * All objects in scenes will extend this.
 */

#ifndef __GEOM_H__
#define __GEOM_H__

#include "vector.h"
//#include "box.h"
//#include "cone.h"
#include "plane.h"
//#include "semi.h"
#include "sphere.h"
//#include "triangle.h"

#define SPHERE_TYPE 1
#define PLANE_TYPE 2
#define OTHER_TYPE 0

typedef struct geom
{
   int type;
   sphere_t *sphere;
   plane_t *plane;
   geom() :
      type(OTHER_TYPE)
   {
   }
   inline mat_t getMat();
   inline matrix4_t getTransform();
} geom_t;

inline mat_t geom::getMat()
{
   switch (type)
   {
   case (OTHER_TYPE):
      return mat_t();
      break;
   case (SPHERE_TYPE):
      return sphere->mat;
      break;
   case (PLANE_TYPE):
      return plane->mat;
      break;
   default:
      return mat_t();
      break;
   }
   return mat_t();
}

inline matrix4_t geom::getTransform()
{
   switch (type)
   {
   case (OTHER_TYPE):
      return matrix4_t();
      break;
   case (SPHERE_TYPE):
      return sphere->transform;
      break;
   case (PLANE_TYPE):
      return plane->transform;
      break;
   default:
      return matrix4_t();
      break;
   }
   return matrix4_t();
}

inline float dot(vec3_t v1, vec3_t v2)
{
   return v1.v[0] * v2.v[0] + v1.v[1] * v2.v[1] + v1.v[2] * v2.v[2];
}

inline float hit(ray_t ray, plane_t *plane)
{
   return -1.0f;
}

inline float hit(ray_t ray, sphere_t *sphere)
{
   vec3_t oMinusC = ray.point - sphere->location;
   float _b = dot(ray.dir, oMinusC);
   float _c = dot(oMinusC, oMinusC) - (sphere->radius * sphere->radius);
   float det = _b * _b - _c;
   if (det < 0)
   {
      return -1.0f;
   }
   float t0 = -_b - (float)sqrt(det);
   float t1 = -_b + (float)sqrt(det);
   if (t0 >= 0 && t1 >= 0)
   {
      if (t0 < t1)
      {
         printf("t0 [0]: %f\n", t0);
         return t0;
      }
      else if (t1 < t0)
      {
         printf("t1 [0]: %f\n", t1);
         return t1;
      }
   }
   else if (t0 >= 0)
   {
      printf("t0 [1]: %f\n", t0);
      return t0;
   }
   else if (t1 >= 0)
   {
      printf("t1 [1]: %f\n", t1);
      return t1;
   }
   return -1.0f;
}

inline float hit(ray_t ray, geom_t obj)
{
   switch (obj.type)
   {
   case (OTHER_TYPE):
      return -1.0f;
      break;
   case (SPHERE_TYPE):
      return hit(ray, obj.sphere);
      break;
   case (PLANE_TYPE):
      return hit(ray, obj.plane);
      break;
   default:
      return -1.0f;
      break;
   }
   return -1.0f;
}

#endif

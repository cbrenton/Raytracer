/**
 * Chris Brenton
 * Sphere object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Sphere.h"

#define EXP_ARGS 4

Sphere::Sphere(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &radius);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &radius);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid sphere format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   readOptions(input);
   boundingBox = bBox();
}

Sphere::Sphere(vec3_t _loc, float _rad) :
   Geometry(),
   radius(_rad) {
      location = _loc;
   }

Box* Sphere::bBox()
{
   vec3_t c1 = vec3_t(location.x() - radius,
         location.y() - radius,
         location.z() - radius);
   vec3_t c2 = vec3_t(location.x() + radius,
         location.y() + radius,
         location.z() + radius);
   return new Box(c1, c2);
}

bool Sphere::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   //Ray ray = Ray();
   //Ray *ray;
   //ray = rayIn.transform(getInvM());
   vec3_t oMinusC = ray.point - location;
   float _b = ray.dir.dot(oMinusC);
   float _c = oMinusC.dot(oMinusC) - (radius * radius);
   float det = _b * _b - _c;
   if (det < 0)
   {
      return false;
   }
   float t0 = -_b - (float)sqrt(det);
   float t1 = -_b + (float)sqrt(det);
   if (t0 >= 0 && t1 >= 0.0)
   {
      if (t0 < t1)
      {
         *t = t0;

         data->hit = true;
         data->point = ray.dir * (*t);
         data->point += ray.point;
         data->t = (*t);
         data->object = this;
         return true;
      }
      else if (t1 < t0)
      {
         *t = t1;

         data->hit = true;
         data->point = ray.dir * (*t);
         data->point += ray.point;
         data->t = (*t);
         data->object = this;
         return true;
      }
   }
   else if (t0 >= 0)
   {
      *t = t0;

      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
      return true;
   }
   else if (t1 >= 0)
   {
      *t = t1;

      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
      return true;
   }
   return false;
}

vec3_t Sphere::getNormal(vec3_t point)
{
   return (point - location) * radius;
}

sphere_dev_t Sphere::getStruct()
{
   sphere_dev_t sp;
   sp.point[0] = location.v[0];
   sp.point[1] = location.v[1];
   sp.point[2] = location.v[2];
   sp.rad = radius;
   return sp;
}

void Sphere::debug()
{
   printf("Sphere: {<%f, %f, %f>, %f\n",
         location.x(), location.y(), location.z(), radius);
}

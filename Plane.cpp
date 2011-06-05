/**
 * Chris Brenton
 * Plane object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Plane.h"

#define EXP_ARGS 4

Plane::Plane(vec3_t normal, float d) : Geometry()
{
   location = normal;
   planeOffset = d;
}

Plane::Plane(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &planeOffset);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &planeOffset);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid plane format: expected %d, found %d.\n", EXP_ARGS, scan);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   readOptions(input);
}

/*
   Box* Plane::bBox()
   {
   return new Box();
   }
   */

bool Plane::hitBVH(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   return hit(ray, t, data, minT, maxT);
}

bool Plane::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   float denominator = ray.dir.dot(location);
   if (denominator == 0.0)
   {
      return false;
   }
   vec3_t p = location * planeOffset;
   vec3_t pMinusL = p - ray.point;
   float numerator = pMinusL.dot(location);
   *t = numerator / denominator;
   if (*t >= minT && *t <= maxT)
   {
      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
      return true;
   }
   return false;
}

vec3_t Plane::getNormal(vec3_t point)
{
   return location;
}

void Plane::debug()
{
   printf("Plane: { <%f, %f, %f>, %f\n",
         location.x(), location.y(), location.z(), planeOffset);
}

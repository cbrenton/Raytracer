/**
 * Chris Brenton
 * Plane object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Plane.h"

Plane::Plane(vec3_t normal, float d)
{
   location = normal;
   planeOffset = d;
}

Plane::Plane(std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
         &location.v[0], &location.v[1], &location.v[2], &planeOffset);
   readOptions(input);
}

bool Plane::hit(Ray ray, float *t, float minT, float maxT)
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
   //printf("Plane: { <%f, %f, %f>, %f\n",
         //location.x, location.y, location.z, planeOffset);
}

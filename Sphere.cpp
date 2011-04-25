/**
 * Chris Brenton
 * Sphere object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Sphere.h"

Sphere::Sphere(std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
         &location.x, &location.y, &location.z, &radius);
   readOptions(input);
}

Sphere::Sphere(Vector3D _loc, float _rad) :
   radius(_rad) {
      location = _loc;
   }

bool Sphere::hit(Ray ray, float *t, float minT, float maxT)
{
   Vector3D oMinusC = ray.point.subtract(location);
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
         return true;
      }
      else if (t1 < t0)
      {
         *t = t1;
         return true;
      }
   }
   else if (t0 >= 0)
   {
      *t = t0;
      return true;
   }
   else if (t1 >= 0)
   {
      *t = t1;
      return true;
   }
   return false;
}

Vector3D Sphere::getNormal(Vector3D point)
{
   return (point.subtract(location)).scalarMultiply(radius);
}

void Sphere::debug()
{
   printf("Sphere: {<%f, %f, %f>, %f\n",
         location.x, location.y, location.z, radius);
}

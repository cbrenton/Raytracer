/**
 * Chris Brenton
 * Cone object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Cone.h"

Cone::Cone(std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f, <%f, %f, %f>, %f",
         &location.x, &location.y, &location.z, &radius1,
         &center2.x, &center2.y, &center2.z, &radius2);
   readOptions(input);
}

bool Cone::hit(Ray ray, float *t, float minT, float maxT)
{
   return false;
}

void Cone::debug()
{
   printf("Cone: { <%f, %f, %f>, %f, <%f, %f, %f>, %f\n",
         location.x, location.y, location.z, radius1,
         center2.x, center2.y, center2.z, radius2);
}

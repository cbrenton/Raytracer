/**
 * Chris Brenton
 * Box object for raytracing.
 * 473
 * 4/8/11
 */

#include <string>
#include "Box.h"

Box::Box(std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, <%f, %f, %f>",
         &location.x, &location.y, &location.z,
         &corner2.x, &corner2.y, &corner2.z);
   readOptions(input);
   pLeft = Plane(Vector3D(1, 0, 0), location.x);
   pRight = Plane(Vector3D(-1, 0, 0), corner2.x);
   pBottom = Plane(Vector3D(0, 1, 0), location.y);
   pTop = Plane(Vector3D(0, -1, 0), corner2.y);
   pFront = Plane(Vector3D(0, 0, 1), location.z);
   pBack = Plane(Vector3D(0, 0, -1), corner2.z);
}

bool Box::hit(Ray ray, float *t, float minT, float maxT)
{

   return false;
}

void Box::debug()
{
   printf("Box: { <%f, %f, %f>, <%f, %f, %f>\n",
         location.x, location.y, location.z,
         corner2.x, corner2.y, corner2.z);
}

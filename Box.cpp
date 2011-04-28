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
         &location.v[0], &location.v[1], &location.v[2],
         &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   readOptions(input);
   pLeft = new Plane(vec3_t(1, 0, 0), location.x());
   pRight = new Plane(vec3_t(-1, 0, 0), corner2.x());
   pBottom = new Plane(vec3_t(0, 1, 0), location.y());
   pTop = new Plane(vec3_t(0, -1, 0), corner2.y());
   pFront = new Plane(vec3_t(0, 0, 1), location.z());
   pBack = new Plane(vec3_t(0, 0, -1), corner2.z());
}

Box::~Box()
{
   delete pLeft;
   delete pRight;
   delete pBottom;
   delete pTop;
   delete pFront;
   delete pBack;
}

bool Box::hit(Ray ray, float *t, float minT, float maxT)
{

   return false;
}

void Box::debug()
{
   //printf("Box: { <%f, %f, %f>, <%f, %f, %f>\n",
         //location.x, location.y, location.z,
         //corner2.x, corner2.y, corner2.z);
}

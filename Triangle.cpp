/**
 * Chris Brenton
 * Triangle object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Triangle.h"

Triangle::Triangle(std::istream& input)
{
   std::string line;
   // Skip the line with only an opening curly brace.
   getline(input, line);
   // Get the good stuff.
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &location.x, &location.y, &location.z);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &corner2.x, &corner2.y, &corner2.z);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>",
         &corner3.x, &corner3.y, &corner3.z);
   readOptions(input);
}

bool Triangle::hit(Ray ray, float *t, float minT, float maxT)
{
   return false;
}

void Triangle::debug()
{
   printf("Triangle: {\n");
   printf("  <%f, %f, %f>,\n",
         location.x, location.y, location.z);
   printf("  <%f, %f, %f>,\n",
         corner2.x, corner2.y, corner2.z);
   printf("  <%f, %f, %f>\n",
         corner3.x, corner3.y, corner3.z);
}

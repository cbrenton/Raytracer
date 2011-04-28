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
         &location.v[0], &location.v[1], &location.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>",
         &corner3.v[0], &corner3.v[1], &corner3.v[2]);
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
         location.v[0], location.v[1], location.v[2]);
   printf("  <%f, %f, %f>,\n",
         corner2.v[0], corner2.v[1], corner2.v[2]);
   printf("  <%f, %f, %f>\n",
         corner3.v[0], corner3.v[1], corner3.v[2]);
}

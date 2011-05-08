/**
 * Chris Brenton
 * Box object for raytracing.
 * 473
 * 4/8/11
 */

#include <cstdlib>
#include <string>
#include "Box.h"

#define EXP_ARGS 6

Box::Box(std::istream& input)
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid box format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
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

/**
 * Chris Brenton
 * Cone object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Cone.h"

#define EXP_ARGS 8

Cone::Cone(std::istream& input)
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , %f , < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &radius1,
            &center2.v[0], &center2.v[1], &center2.v[2], &radius2);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , %f , < %f , %f , %f > , %f",
            &location.v[0], &location.v[1], &location.v[2], &radius1,
            &center2.v[0], &center2.v[1], &center2.v[2], &radius2);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid cone format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   readOptions(input);
   thisBBox = bBox();
}

Box* Cone::bBox()
{
   return new Box();
}

bool Cone::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   return false;
}

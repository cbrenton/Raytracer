/**
 * Sterling Hirsh
 * Light Class
 * 4/3/11
 */

#include <cstdlib>
#include <string>
#include "Light.h"

#define EXP_ARGS 6

Light::Light(std::istream& input) {
   std::string line;
   // Get the rest of the line.
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > color rgb < %f , %f , %f > }",
            &location.v[0], &location.v[1], &location.v[2],
            &r, &g, &b);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > color rgb < %f , %f , %f > }",
            &location.v[0], &location.v[1], &location.v[2],
            &r, &g, &b);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid light format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
}

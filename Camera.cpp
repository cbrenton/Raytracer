/**
 * Chris Brenton
 * Camera Class
 * 4/11/11
 */

#include <cstdlib>
#include <stdio.h>
#include "Camera.h"

#define EXP_ARGS 12

Camera::Camera(std::istream& input) {
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      getline(input, line);
      scan += sscanf(line.c_str(), " location <%f, %f, %f>",
            &location.v[0], &location.v[1], &location.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " up <%f, %f, %f>",
            &up.v[0], &up.v[1], &up.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " right <%f, %f, %f>",
            &right.v[0], &right.v[1], &right.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " look_at <%f, %f, %f>",
            &look_at.v[0], &look_at.v[1], &look_at.v[2]);
   }
   else
   {
      scan += sscanf(line.c_str(), " { location <%f, %f, %f>",
            &location.v[0], &location.v[1], &location.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " up <%f, %f, %f>",
            &up.v[0], &up.v[1], &up.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " right <%f, %f, %f>",
            &right.v[0], &right.v[1], &right.v[2]);
      getline(input, line);
      scan += sscanf(line.c_str(), " look_at <%f, %f, %f>",
            &look_at.v[0], &look_at.v[1], &look_at.v[2]);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid camera format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
}

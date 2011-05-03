/**
 * Light - not very heavy.
 * Sterling Hirsh
 * 4/5/11
 */

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <iostream>
#include <stdio.h>
#include <string>
#include "vector.h"

typedef struct light
{
   vec3_t location;
   float r;
   float g;
   float b;
   light()
   {
      r = g = b = 0;
   }
   light(const light& other)
   {
      r = other.r;
      g = other.g;
      b = other.b;
   }
   light(std::istream& input)
   {
      // Get the rest of the line.
      // sscanf to get specific values.
      std::string line;
      getline(input, line);
      sscanf(line.c_str(), "{<%f, %f, %f> color rgb <%f, %f, %f>}",
            &location.v[0], &location.v[0], &location.v[2],
            &r, &g, &b);
   }
} light_t;

inline void makeLight(light_t cur, std::istream& input)
{
   // Get the rest of the line.
   // sscanf to get specific values.
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{<%f, %f, %f> color rgb <%f, %f, %f>}",
         &cur.location.v[0], &cur.location.v[0], &cur.location.v[2],
         &cur.r, &cur.g, &cur.b);
}

#endif

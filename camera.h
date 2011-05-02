/**
 * Sterling Hirsh
 * Camera Class
 * 4/3/11
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>
#include <stdio.h>
#include "vector.h"

typedef struct camera
{
      vec3_t location;
      vec3_t up;
      vec3_t right;
      vec3_t look_at;
} camera_t;

inline makeCamera(camera_t cur, std::istream&cur. input) {
   std::string line;
   // Skip the first blank line
   getline(input, line);
   getline(input, line);
   sscanf(line.c_str(), " location <%f, %f, %f>",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " up <%f, %f, %f>",
         &cur.up.v[0], &cur.up.v[1], &cur.up.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " right <%f, %f, %f>",
         &cur.right.v[0], &cur.right.v[1], &cur.right.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " look_at <%f, %f, %f>",
         &cur.look_at.v[0], &cur.look_at.v[1], &cur.look_at.v[2]);
}

#endif

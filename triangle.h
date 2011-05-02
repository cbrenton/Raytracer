/**
 * Sterling Hirsh
 * Triangle object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include <stdio.h>
#include "vector.h"
#include "geom.h"

typedef struct tri
{
   // Corner1 is Geometry::location
   vec3_t location;
   vec3_t corner2;
   vec3_t corner3;
   mat_t mat;
   matrix4_t transform;
} tri_t;

inline void makeTriangle(tri_t cur, std::istream& input)
{
   std::string line;
   // Skip the line with only an opening curly brace.
   getline(input, line);
   // Get the good stuff.
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &cur.corner2.v[0], &cur.corner2.v[1], &cur.corner2.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>",
         &cur.corner3.v[0], &cur.corner3.v[1], &cur.corner3.v[2]);
   readOptions(cur.mat, cur.transform, input);
}

#endif



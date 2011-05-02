/**
 * Sterling Hirsh
 * Sphere object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __SPHERE_H__
#define __SPHERE_H__

#include <stdio.h>
#include "vector.h"
#include "geom.h"

typedef struct sphere
{
   vec3_t location;
   float radius;
   mat_t mat;
   matrix4_t transform;
} sphere_t;

inline void makeSphere(sphere_t cur, std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2], &cur.radius);
   readOptions(cur.mat, cur.transform, input);
}

#endif

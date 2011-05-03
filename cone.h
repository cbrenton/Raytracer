/**
 * Sterling Hirsh
 * Cone object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __CONE_H__
#define __CONE_H__

#include <stdio.h>
#include "mat.h"

typedef struct cone
{
   // Center1 is location
   vec3_t location;
   vec3_t center2;
   float radius1;
   float radius2;
   mat_t mat;
   matrix4_t transform;
} cone_t;

inline makeCone(cone_t cur, std::istream&cur. input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f, <%f, %f, %f>, %f",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2], &cur.radius1,
         &cur.center2.v[0], &cur.center2.v[1], &cur.center2.v[2], &cur.radius2);
   readOptions(cur.mat, cur.transform, input);
}

#endif



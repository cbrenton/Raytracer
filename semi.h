/**
 * Chris Brenton
 * Half sphere object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __SEMI_H__
#define __SEMI_H__

#include <stdio.h>
#include "vector.h"
#include "mat.h"
#include "plane.h"

typedef struct semi
{
   vec3_t location;
   float radius;
   bool isTop;
   plane_t bisect;
   mat_t mat;
   matrix4_t transform;
} semi_t;

inline void makeSemi(semi_t cur, std::istream& input)
{
   std::string line;
   getline(input, line);
   int isFacingUp;
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f, %d",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2], &cur.radius, &cur.isFacingUp);
   isTop = (isFacingUp == 0);
   readOptions(cur.mat, cur.transform, input);
   vec3_t bisectNormal;
   if (isTop)
   {
      bisectNormal = vec3_t(0, 1, 0);
   }
   else
   {
      bisectNormal = vec3_t(0, -1, 0);
   }
   cur.bisect = plane_t(*bisectNormal, location.y());
}

#endif

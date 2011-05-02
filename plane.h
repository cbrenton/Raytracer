/**
 * Sterling Hirsh
 * Plane object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __Plane_H__
#define __Plane_H__

#include <stdio.h>
#include <string>
#include "vector.h"
#include "geom.h"

typedef struct plane
{
   vec3_t location;
   vec3_t normal;
   float planeOffset; // This is D in the plane equasion.
   mat_t mat;
   matrix4_t transform;
} plane_t;


inline void makePlane(plane_t cur, std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
         &cur.location.v[0], &cur.location.v[1], &cur.location.v[2], &cur.planeOffset);
   readOptions(cur.mat, cur.transform, input);
}

#endif

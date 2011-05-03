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
#include "mat.h"

typedef struct plane
{
   vec3_t normal;
   float planeOffset; // This is D in the plane equasion.
   mat_t mat;
   matrix4_t transform;
   plane()
   {
      normal = vec3_t(0, 1, 0);
      planeOffset = 0;
   }
   plane(const plane& other)
   {
      normal.v[0] = other.normal.v[0];
      normal.v[1] = other.normal.v[1];
      normal.v[2] = other.normal.v[2];
      planeOffset = other.planeOffset;
      mat = other.mat;
      transform = other.transform;
   }
   inline plane(std::istream& input);
} plane_t;


inline plane::plane(std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
         &normal.v[0], &normal.v[1], &normal.v[2], &planeOffset);
   readOptions(&mat, &transform, input);
}

#endif

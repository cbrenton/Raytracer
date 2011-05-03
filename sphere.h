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
#include "mat.h"

typedef struct sphere
{
   vec3_t location;
   float radius;
   mat_t mat;
   matrix4_t transform;
   sphere()
   {
      location = vec3_t(0, 0, 0);
      radius = 1;
      //mat = mat_t();
      //transform = matrix4_t();
      printf("empty mat: <%f, %f, %f>\n", mat.r, mat.g, mat.b);
   }
   sphere(const sphere& other)
   {
      location.v[0] = other.location.v[0];
      location.v[1] = other.location.v[1];
      location.v[2] = other.location.v[2];
      radius = other.radius;
      mat.r = other.mat.r;
      mat.g = other.mat.g;
      mat.b = other.mat.b;
      transform = other.transform;
      printf("mat: <%f, %f, %f>\n", mat.r, mat.g, mat.b);
   }
   //inline sphere(std::istream& input);
} sphere_t;

//inline sphere::sphere(std::istream& input)
inline void makeSphere(sphere_t *cur, std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f",
   /*
         &location.v[0], &location.v[1], &location.v[2], &radius);
   readOptions(&mat, &transform, input);
   printf("sphere finish: <%f, %f, %f>\n", mat.r, mat.g, mat.b);
   */
         &cur->location.v[0], &cur->location.v[1], &cur->location.v[2], &cur->radius);
   printf("x: %f\n", cur->location.v[0]);
   readOptions(&cur->mat, &cur->transform, input);
   printf("sphere finish: <%f, %f, %f>\n", cur->mat.r, cur->mat.g, cur->mat.b);
}

#endif

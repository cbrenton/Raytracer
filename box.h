/**
 * Sterling Hirsh
 * Box object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __BOX_H__
#define __BOX_H__

//#include "plane.h"
#include <stdio.h>
#include "geom.h"

typedef struct box
{
   // Corner1 is location
   vec3_t location;
   vec3_t corner2;
   mat_t mat;
   matrix4_t transform;
} box_t;

inline makeBox(box_t cur, std::istream& input)
{
   std::string line;
   getline(input, line);
   sscanf(line.c_str(), "{ <%f, %f, %f>, <%f, %f, %f>",
         &box.location.v[0], &box.location.v[1], &box.location.v[2],
         &box.corner2.v[0], &box.corner2.v[1], &box.corner2.v[2]);
   readOptions(cur.mat, cur.transform, input);
   /*
      pLeft = new Plane(vec3_t(1, 0, 0), location.x());
      pRight = new Plane(vec3_t(-1, 0, 0), corner2.x());
      pBottom = new Plane(vec3_t(0, 1, 0), location.y());
      pTop = new Plane(vec3_t(0, -1, 0), corner2.y());
      pFront = new Plane(vec3_t(0, 0, 1), location.z());
      pBack = new Plane(vec3_t(0, 0, -1), corner2.z());
      */
}

#endif


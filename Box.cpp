/**
 * Chris Brenton
 * Box object for raytracing.
 * 473
 * 4/8/11
 */

#include <cstdlib>
#include <string>
#include <algorithm>
#include "Box.h"
#include "Plane.h"

#define EXP_ARGS 6

Box::Box(vec3_t c1, vec3_t c2) : Geometry()
{
   location = c1;
   corner2 = c2;
   pLeft = new Plane(vec3_t(1, 0, 0), location.x());
   pRight = new Plane(vec3_t(-1, 0, 0), -corner2.x());
   //pRight = new Plane(vec3_t(1, 0, 0), corner2.x());
   pBottom = new Plane(vec3_t(0, 1, 0), location.y());
   pTop = new Plane(vec3_t(0, -1, 0), -corner2.y());
   //pTop = new Plane(vec3_t(0, 1, 0), corner2.y());
   pFront = new Plane(vec3_t(0, 0, 1), location.z());
   pBack = new Plane(vec3_t(0, 0, -1), -corner2.z());
   //pBack = new Plane(vec3_t(0, 0, 1), corner2.z());
   //thisBBox = bBox();
   thisBBox = this;
}

Box::Box(std::istream& input)
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid box format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   readOptions(input);
   pLeft = new Plane(vec3_t(1, 0, 0), location.x());
   pRight = new Plane(vec3_t(-1, 0, 0), -corner2.x());
   //pRight = new Plane(vec3_t(1, 0, 0), corner2.x());
   pBottom = new Plane(vec3_t(0, 1, 0), location.y());
   pTop = new Plane(vec3_t(0, -1, 0), -corner2.y());
   //pTop = new Plane(vec3_t(0, 1, 0), corner2.y());
   pFront = new Plane(vec3_t(0, 0, 1), location.z());
   pBack = new Plane(vec3_t(0, 0, -1), -corner2.z());
   //pBack = new Plane(vec3_t(0, 0, 1), corner2.z());
   thisBBox = this;
}

Box::~Box()
{
   delete pLeft;
   delete pRight;
   delete pBottom;
   delete pTop;
   delete pFront;
   delete pBack;
}

Box* Box::bBox()
{
   return new Box(location, corner2);
   //return this;
}

void Box::combine(Box *b1, Box *b2)
{
   float corners[3][2];
   for (int i = 0; i < 3; i++)
   {
      corners[i][0] = min(min(b1->location.v[i], b1->corner2.v[i]),
            min(b2->location.v[i], b2->corner2.v[i]));
      corners[i][1] = max(max(b1->location.v[i], b1->corner2.v[i]),
            max(b2->location.v[i], b2->corner2.v[i]));
   }
   vec3_t c1 = vec3_t(corners[0][0], corners[1][0], corners[2][0]);
   vec3_t c2 = vec3_t(corners[0][1], corners[1][1], corners[2][1]);
   location = c1;
   corner2 = c2;
}

bool Box::closeEnough(vec_t a, vec_t b)
{
   return (max(a - b, b - a) < EPSILON);
}

vec3_t Box::getNormal(vec3_t point)
{
   if (closeEnough(point.x(), location.x()))
      return pLeft->location;
   else if (closeEnough(point.x(), corner2.x()))
      return pRight->location;
   else if (closeEnough(point.y(), location.y()))
      return pBottom->location;
   else if (closeEnough(point.y(), corner2.y()))
      return pTop->location;
   else if (closeEnough(point.z(), location.z()))
      return pBack->location;
   else if (closeEnough(point.z(), corner2.z()))
      return pFront->location;
   else
   {
      cerr << "Point not on box." << endl;
      return vec3_t(0, 0, 0);
   }
}

bool Box::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   float tNear = minT - 1;
   float tFar = maxT + 1;

   // For the pair of planes in each dimension.
   for (int i = 0; i < 3; i++)
   {
      // Component of the ray's direction in the current dimension.
      float xD = ray.dir.v[i];
      // Component of the ray's origin in the current dimension.
      float xO = ray.point.v[i];
      // Component of the mininum plane location in the current dimension.
      float xL = min(location.v[i], corner2.v[i]);
      // Component of the maxinum plane location in the current dimension.
      float xH = max(location.v[i], corner2.v[i]);
      // If direction in current dimension is 0, ray is parallel to planes.
      if (xD == 0)
      {
         // If ray origin is not between the planes.
         if (xO < xL || xO > xH)
         {
            return false;
         }
      }
      // Else the ray is not parallel to the planes.
      else
      {
         // Calculate tMin and tMax.
         float t1 = (xL - xO) / xD;
         float t2 = (xH - xO) / xD;
         if (t1 > t2)
         {
            swap(t1, t2);
         }
         if (t1 > tNear)
         {
            tNear = t1;
         }
         if (t2 < tFar)
         {
            tFar = t2;
         }
         if (tNear > tFar)
         {
            return false;
         }
         if (tFar < 0)
         {
            return false;
         }
      }
   }
   *t = tNear;

   data->hit = true;
   data->point = ray.dir * (*t);
   data->point += ray.point;
   data->t = (*t);
   data->object = this;
   return true;
}

/**
 * Chris Brenton
 * Triangle object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Triangle.h"
#include "Matrix4.h"

#define EXP_ARGS 9

Triangle::Triangle(std::istream& input)
{
   std::string line;
   getline(input, line);
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      getline(input, line);
      scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2],
            &corner3.v[0], &corner3.v[1], &corner3.v[2]);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2],
            &corner3.v[0], &corner3.v[1], &corner3.v[2]);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid triangle format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   readOptions(input);
}

Triangle::Triangle(vec3_t c1, vec3_t c2, vec3_t c3)
{
   location = c1;
   corner2 = c2;
   corner3 = c3;
}

bool Triangle::hit(Ray ray, float *t, float minT, float maxT)
{
   float result = -1;
   float bBeta, bGamma, bT;

   Matrix4 A = Matrix4(
         location.x()-corner2.x(), location.x()-corner3.x(),
         ray.dir.x(), 0,
         location.y()-corner2.y(), location.y()-corner3.y(),
         ray.dir.y(), 0,
         location.z()-corner2.z(), location.z()-corner3.z(),
         ray.dir.z(), 0,
         0, 0, 0, 1);
   Matrix4 baryT = Matrix4(
         location.x()-corner2.x(), location.x()-corner3.x(),
         location.x()-ray.point.x(), 0,
         location.y()-corner2.y(), location.y()-corner3.y(),
         location.y()-ray.point.y(), 0,
         location.z()-corner2.z(), location.z()-corner3.z(),
         location.z()-ray.point.z(), 0,
         0, 0, 0, 1);

   bT = baryT.det3() / A.det3();
   //if (bT < 0 || bT > F_MAX)
   if (bT < 0)
   {
      result = 0;
   }
   else
   {
      Matrix4 baryGamma = Matrix4(
            location.x()-corner2.x(), location.x()-ray.point.x(),
            ray.dir.x(), 0,
            location.y()-corner2.y(), location.y()-ray.point.y(),
            ray.dir.y(), 0,
            location.z()-corner2.z(), location.z()-ray.point.z(),
            ray.dir.z(), 0,
            0, 0, 0, 1);

      bGamma = baryGamma.det3() / A.det3();
      if (bGamma < 0 || bGamma > 1)
      {
         result = 0;
      }
      else
      {
         Matrix4 baryBeta = Matrix4(
               location.x()-ray.point.x(), location.x()-corner3.x(),
               ray.dir.x(), 0,
               location.y()-ray.point.y(), location.y()-corner3.y(),
               ray.dir.y(), 0,
               location.z()-ray.point.z(), location.z()-corner3.z(),
               ray.dir.z(), 0,
               0, 0, 0, 1);

         bBeta = baryBeta.det3() / A.det3();
         if (bBeta < 0 || bBeta > 1 - bGamma)
         {
            result = 0;
         }
      }
   }

   if (result != 0)
   {
      result = bT;
   }
   *t = result;
   if (result > 0)
   {
      //printf("hit: %f!\n", result);
   }
   return (result > 0);
}

vec3_t Triangle::getNormal(vec3_t point)
{
   vec3_t s1 = corner2 - location;
   vec3_t s2 = corner3 - location;
   vec3_t normal;
   normal.cross(s1, s2);
   return normal;
}

triangle_dev_t Triangle::getStruct()
{
   float c1[3] = {location.v[0], location.v[1], location.v[2]};
   float c2[3] = {corner2.v[0], corner2.v[1], corner2.v[2]};
   float c3[3] = {corner3.v[0], corner3.v[1], corner3.v[2]};
   //vec3_t c2 = corner2;
   //vec3_t c3 = corner3;
   triangle_dev_t ret;
   ret.c1[0] = c1[0];
   ret.c1[1] = c1[1];
   ret.c1[2] = c1[2];
   ret.c2[0] = c2[0];
   ret.c2[1] = c2[1];
   ret.c2[2] = c2[2];
   ret.c3[0] = c3[0];
   ret.c3[1] = c3[1];
   ret.c3[2] = c3[2];
   return ret;
}

void Triangle::debug()
{
   printf("Triangle: {\n");
   printf("  <%f, %f, %f>,\n",
         location.v[0], location.v[1], location.v[2]);
   printf("  <%f, %f, %f>,\n",
         corner2.v[0], corner2.v[1], corner2.v[2]);
   printf("  <%f, %f, %f>\n",
         corner3.v[0], corner3.v[1], corner3.v[2]);
}

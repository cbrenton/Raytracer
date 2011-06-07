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

Triangle::Triangle(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   int scan = 0;
   bool isDone = false;
   int braceCount = 0;
   //for (unsigned i = 0; i < line.size(); i++)
   unsigned start = line.find("triangle {");
   if (start == string::npos)
   {
      if (line[0] == '{')
      {
         start = 1;
      }
      else
      {
         start = 0;
      }
   }
   for (unsigned i = start; i < line.size(); i++)
   {
      if (line[i] == '{')
      {
         braceCount++;
      }
      else if (line[i] == '}')
      {
         braceCount--;
      }
   }
   isDone = (braceCount < 0);
   //while (line == "")
   while (line.empty())
   {
      getline(input, line);
   }
   // If the line is only an opening curly brace, skip it.
   if (line == "{" || line == " {")
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
      /*
      if (scan == 0)
      {
            scan = sscanf(line.c_str(), " < %f , %f , %f > , < %f , %f , %f > , < %f , %f , %f >",
            &location.v[0], &location.v[1], &location.v[2],
            &corner2.v[0], &corner2.v[1], &corner2.v[2],
            &corner3.v[0], &corner3.v[1], &corner3.v[2]);
      }
      */
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid triangle format: expected %d, found %d.\n", EXP_ARGS, scan);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   /*
   if (!isDone)
   {
      //matSet = readOptions(input);
      readOptions(input);
   }
   */
   readOptions(input);
   boundingBox = bBox();
   facePt = getFacePoint();
}

Triangle::Triangle(vec3_t c1, vec3_t c2, vec3_t c3) : Geometry()
{
   location = c1;
   corner2 = c2;
   corner3 = c3;
   facePt = getFacePoint();
}

Box* Triangle::bBox()
{
   float minX, minY, minZ, maxX, maxY, maxZ;
   minX = min(min(location.x(), corner2.x()), corner3.x());
   maxX = max(max(location.x(), corner2.x()), corner3.x());
   minY = min(min(location.y(), corner2.y()), corner3.y());
   maxY = max(max(location.y(), corner2.y()), corner3.y());
   minZ = min(min(location.z(), corner2.z()), corner3.z());
   maxZ = max(max(location.z(), corner2.z()), corner3.z());
   vec3_t c1 = vec3_t(minX, minY, minZ);
   vec3_t c2 = vec3_t(maxX, maxY, maxZ);
   return new Box(c1, c2);
}

bool Triangle::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
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
   float detA = A.det3();

   Matrix4 baryT = Matrix4(
         location.x()-corner2.x(), location.x()-corner3.x(),
         location.x()-ray.point.x(), 0,
         location.y()-corner2.y(), location.y()-corner3.y(),
         location.y()-ray.point.y(), 0,
         location.z()-corner2.z(), location.z()-corner3.z(),
         location.z()-ray.point.z(), 0,
         0, 0, 0, 1);

   bT = baryT.det3() / detA;
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

      bGamma = baryGamma.det3() / detA;
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

         bBeta = baryBeta.det3() / detA;
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
   if (result > 0.01)
   {
      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
   }
   return (result > 0.01);
}

bool Triangle::hit(vec3_t pt)
{
   Ray *tmp = new Ray(pt, vec3_t(0, 0, 0));
   float t = 0;
   bool ret = hit(*tmp, &t);
   delete tmp;
   return ret;
}

vec3_t Triangle::getNormal(vec3_t point)
{
   vec3_t s1 = corner2 - location;
   vec3_t s2 = corner3 - location;
   vec3_t normal;
   normal.cross(s1, s2);
   return normal;
}

bool Triangle::contains(vec3_t pt)
{
   return (location == pt || corner2 == pt || corner3 == pt);
}

bool Triangle::isNeighbor(Triangle *other)
{
   int samePts = 0;
   if (other->contains(location))
   {
      samePts++;
   }
   if (other->contains(corner2))
   {
      samePts++;
   }
   if (other->contains(corner3))
   {
      samePts++;
   }
   return (samePts == 2);
}

bool Triangle::isNeighbor(vec3_t c1, vec3_t c2)
{
   return (contains(c1) && contains(c2));
}

vec3_t Triangle::getFacePoint()
{
   vec3_t ret;
   ret = location + corner2 + corner3;
   ret /= 3;
   return ret;
}

triangle_dev_t Triangle::getStruct()
{
   float c1[3] = {location.v[0], location.v[1], location.v[2]};
   float c2[3] = {corner2.v[0], corner2.v[1], corner2.v[2]};
   float c3[3] = {corner3.v[0], corner3.v[1], corner3.v[2]};
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

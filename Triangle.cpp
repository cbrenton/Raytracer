/**
 * Chris Brenton
 * Triangle object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Triangle.h"
#include "Matrix.h"

Triangle::Triangle(std::istream& input)
{
   std::string line;
   // Skip the line with only an opening curly brace.
   getline(input, line);
   // Get the good stuff.
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &location.v[0], &location.v[1], &location.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>,",
         &corner2.v[0], &corner2.v[1], &corner2.v[2]);
   getline(input, line);
   sscanf(line.c_str(), " <%f, %f, %f>",
         &corner3.v[0], &corner3.v[1], &corner3.v[2]);
   readOptions(input);
}

bool Triangle::hit(Ray ray, float *t, float minT, float maxT)
{
   Matrix4 A = new Matrix4(
         location.x() - corner2.x(), location.x() - corner3.x(), ray.dir.x(),
         location.y() - corner2.y(), location.y() - corner3.y(), ray.dir.y(),
         location.z() - corner2.z(), location.z() - corner3.z(), ray.dir.z());
   Matrix4 baryT = new Matrix4(
         location.x() - corner2.x(), location.x() - corner3.x(), location.x() - ray.point.x(),
         location.y() - corner2.y(), location.y() - corner3.y(), location.y() - ray.point.y(),
         location.z() - corner2.z(), location.z() - corner3.z(), location.z() - ray.point.z();
   Matrix4 baryBeta = new Matrix4(
         location.x() - ray.point.x(), location.x() - corner3.x(), ray.dir.x(),
         location.y() - ray.point.y(), location.y() - corner3.y(), ray.dir.y(),
         location.z() - ray.point.z(), location.z() - corner3.z(), ray.dir.z());
   Matrix4 baryGamma = new Matrix4(
         location.x() - corner2.x(), location.x() - ray.point.x(), ray.dir.x(),
         location.y() - corner2.y(), location.y() - ray.point.y(), ray.dir.y(),
         location.z() - corner2.z(), location.z() - ray.point.z(), ray.dir.z());
   float bAlpha = baryAlpha.det() / A.det();
   float bGamma = baryGamma.det() / A.det();
   float bT = baryT.det() / A.det();
   if (bT < 0 || bT > F_MAX)
   {
   ;
   }
   delete A;
   delete baryT;
   delete baryBeta;
   delete baryGamma;
   return false;
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

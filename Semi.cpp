/**
 * Chris Brenton
 * Semi object for raytracing.
 * 473
 * 4/7/11
 */

#include <string>
#include "Semi.h"

Semi::Semi(std::istream& input)
{
   std::string line;
   getline(input, line);
   int isFacingUp;
   sscanf(line.c_str(), "{ <%f, %f, %f>, %f, %d",
         &location.x, &location.y, &location.z, &radius, &isFacingUp);
   isTop = (isFacingUp == 0);
   readOptions(input);
   Vector3D *bisectNormal;
   if (isTop)
   {
      bisectNormal = new Vector3D(0, 1, 0);
   }
   else
   {
      bisectNormal = new Vector3D(0, -1, 0);
   }
   bisect = new Plane(*bisectNormal, location.y);
   delete bisectNormal;
}

Semi::Semi(Vector3D _loc, float _rad) :
   radius(_rad) {
      location = _loc;
   }

Semi::~Semi()
{
   delete bisect;
}

bool Semi::hit(Ray ray, float *t, float minT, float maxT)
{
   Vector3D oMinusC = ray.point.subtract(location);
   float _b = ray.dir.dot(oMinusC);
   float _c = oMinusC.dot(oMinusC) - (radius * radius);
   float det = _b * _b - _c;
   if (det < 0)
   {
      return false;
   }
   float t0 = -_b - (float)sqrt(det);
   float t1 = -_b + (float)sqrt(det);
   Vector3D intersect0 = ray.point.add(ray.dir.scalarMultiply(t0));
   Vector3D intersect1 = ray.point.add(ray.dir.scalarMultiply(t1));
   float tP = -1.0;
   //bool planeIntersect = bisect.hit(ray, &tP);
   bool canBeT0, canBeT1;
   if (isTop)
   {
      canBeT0 = intersect0.y < location.y;
      canBeT1 = intersect1.y < location.y;
   }
   else
   {
      canBeT0 = intersect0.y > location.y;
      canBeT1 = intersect1.y > location.y;
   }


   //bisect->hit(ray, &tP, tmp);
   if(bisect->hit(ray, &tP))
   {
      if (tP >= minT && tP <= maxT)
      {
         if (t0 <= tP && tP <= t1)
         {
            if (!canBeT0)
            {
               //printf("plane intersect at tP\n");
               //printf("t1->tP->t0: %f->%f->%f\n", t1, tP, t0);
               *t = tP;
               return true;
            }
         }
         else if (t1 <= tP && tP <= t0)
         {
            if (!canBeT1)
            {
               //printf("plane intersect at tP\n");
               //printf("t0->tP->t1: %f->%f->%f\n", t0, tP, t1);
               *t = tP;
               return true;
            }
         }
      }
   }


   if (t0 >= minT && t0 <= maxT && t1 >= minT && t1 <= maxT)
   {
      if (t0 < t1 && canBeT0)
         //if (t0 < t1)
      {
         *t = t0;
         return true;
      }
      else if (t1 < t0 && canBeT1)
         //else if (t1 < t0)
      {
         *t = t1;
         return true;
      }
   }
   else if (t0 >= minT && t0 <= maxT && canBeT0)
   {
      *t = t0;
      return true;
   }
   else if (t1 >= minT && t1 <= maxT && canBeT1)
   {
      *t = t1;
      return true;
   }
   return false;
}

Vector3D Semi::getNormal(Vector3D point)
{
   if ((point.y - location.y) < 0.00001 && (point.y - location.y) > -0.00001)
   {
      return bisect->getNormal(point);
   }
   return (point.subtract(location)).scalarMultiply(radius);
}

void Semi::debug()
{
   printf("Semi: {<%f, %f, %f>, %f\n",
         location.x, location.y, location.z, radius);
}

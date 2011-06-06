/**
 * Chris Brenton
 * Semi object for raytracing.
 * 473
 * 4/7/11
 */

#include <cstdlib>
#include <string>
#include "Semi.h"

#define EXP_ARGS 5

Semi::Semi(std::istream& input) : Geometry()
{
   std::string line;
   getline(input, line);
   int isFacingUp;
   int scan = 0;
   // If the line is only an opening curly brace, skip it.
   if (line == "{")
   {
      // Get the good stuff.
      scan = sscanf(line.c_str(), " < %f , %f , %f > , %f , %d",
            &location.v[0], &location.v[1], &location.v[2], &radius, &isFacingUp);
   }
   else
   {
      scan = sscanf(line.c_str(), " { < %f , %f , %f > , %f , %d",
            &location.v[0], &location.v[1], &location.v[2], &radius, &isFacingUp);
   }
   if (scan < EXP_ARGS)
   {
      printf("Invalid hemisphere format: expected %d, found %d.\n", scan, EXP_ARGS);
      std::cout << "\tline: " << line << std::endl;
      exit(EXIT_FAILURE);
   }
   isTop = (isFacingUp == 0);
   readOptions(input);
   vec3_t *bisectNormal;
   if (isTop)
   {
      bisectNormal = new vec3_t(0, 1, 0);
   }
   else
   {
      bisectNormal = new vec3_t(0, -1, 0);
   }
   bisect = new Plane(*bisectNormal, location.y());
   delete bisectNormal;
   boundingBox = bBox();
}

Semi::Semi(vec3_t _loc, float _rad) :
   Geometry(),
   radius(_rad) {
      location = _loc;
   }

Semi::~Semi()
{
   delete bisect;
}

Box* Semi::bBox()
{
   return new Box();
}

bool Semi::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   vec3_t oMinusC = ray.point - location;
   float _b = ray.dir.dot(oMinusC);
   float _c = oMinusC.dot(oMinusC) - (radius * radius);
   float det = _b * _b - _c;
   if (det < 0)
   {
      return false;
   }
   float t0 = -_b - (float)sqrt(det);
   float t1 = -_b + (float)sqrt(det);
   vec3_t t0Disp = ray.dir * t0;
   vec3_t intersect0 = ray.point + t0Disp;
   vec3_t t1Disp = ray.dir * t1;
   vec3_t intersect1 = ray.point + t1Disp;
   float tP = -1.0;
   //bool planeIntersect = bisect.hit(ray, &tP);
   bool canBeT0, canBeT1;
   if (isTop)
   {
      canBeT0 = intersect0.y() < location.y();
      canBeT1 = intersect1.y() < location.y();
   }
   else
   {
      canBeT0 = intersect0.y() > location.y();
      canBeT1 = intersect1.y() > location.y();
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
               data->hit = true;
               data->point = ray.dir * (*t);
               data->point += ray.point;
               data->t = (*t);
               data->object = this;
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
               data->hit = true;
               data->point = ray.dir * (*t);
               data->point += ray.point;
               data->t = (*t);
               data->object = this;
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
         data->hit = true;
         data->point = ray.dir * (*t);
         data->point += ray.point;
         data->t = (*t);
         data->object = this;
         return true;
      }
      else if (t1 < t0 && canBeT1)
         //else if (t1 < t0)
      {
         *t = t1;
         data->hit = true;
         data->point = ray.dir * (*t);
         data->point += ray.point;
         data->t = (*t);
         data->object = this;
         return true;
      }
   }
   else if (t0 >= minT && t0 <= maxT && canBeT0)
   {
      *t = t0;
      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
      return true;
   }
   else if (t1 >= minT && t1 <= maxT && canBeT1)
   {
      *t = t1;
      data->hit = true;
      data->point = ray.dir * (*t);
      data->point += ray.point;
      data->t = (*t);
      data->object = this;
      return true;
   }
   return false;
}

vec3_t Semi::getNormal(vec3_t point)
{
   if ((point.y() - location.y()) < 0.00001 && (point.y() - location.y()) > -0.00001)
   {
      return bisect->getNormal(point);
   }
   return (point - location) * radius;
}

void Semi::debug()
{
   //printf("Semi: {<%f, %f, %f>, %f\n",
   //location.x, location.y, location.z, radius);
}

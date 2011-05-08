// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "vector.h"

// includes, project
#include <cutil.h>

#include "Intersection_K.h"

__device__ float dot_product(float *v1, float *v2)
{
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

__device__ float hit(ray_dev_t ray, float *t, sphere_dev_t *sp)
{
   float oMinusC[3];
   oMinusC[0] = ray.point[0] - sp->point[0];
   oMinusC[1] = ray.point[1] - sp->point[1];
   oMinusC[2] = ray.point[2] - sp->point[2];   
  
   float _b = dot_product(ray.dir,oMinusC);
   float _c = dot_product(oMinusC,oMinusC) - (sp->rad * sp->rad);
   float det = _b * _b - _c;

   if (det < 0)
   {
      return -1;
   }

   float t0 = -_b - (float)sqrt(det);
   float t1 = -_b + (float)sqrt(det);

   if (t0 >= 0 && t1 >= 0.0)
   {
      if (t0 < t1)
      {
         return t0;
      }
      else if (t1 < t0)
      {
         return t1;
      }
   }
   else if (t0 >= 0)
   {
      return t0;
   }
   else if (t1 >= 0)
   {
      return t1;
   }

   return -1;
}

__global__ void Intersection_Test(ray_dev_t *rays, sphere_dev_t *spheres, intersection_t *intersects, int size, int width, int height)
{
   float t;
   bool hitFound = false;
   float curDepth = -1.0;
   int x = blockIdx.x * BLOCK_SIZE + threadIdx.x;
   int y = blockIdx.y * BLOCK_SIZE + threadIdx.y;

   if(x > width || y > height)
       return;

   ray_dev_t *ray = rays + x * height + y;   
   intersection_t *result = intersects + x * height + y;

   result->index = -1;

   for (int i = 0; i < size; i++)
   {
      sphere_dev_t *curObject = &spheres[i];
      t = -1.0;
      t = hit(*ray, &t, curObject);

      if (t >= 0)
      {
         if (t >= 0 && (!hitFound || (hitFound && t < curDepth)))
         {
            curDepth = t;
            result->index = i;
         }
      }
      hitFound |= (t >= 0 && curDepth > 0.0);
   }
   
   //Add intersection
   result->t = curDepth;
}


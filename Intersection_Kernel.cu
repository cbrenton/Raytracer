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
<<<<<<< HEAD
   return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

__device__ float det3(float **v1)
{
   return v1[0][0] * v1[1][1] * v1[2][2] + v1[0][1] * v1[1][2] * v1[2][0] + v1[0][2] * v1[1][0] * v1[2][1] -
      v1[0][2] * v1[1][1] * v1[2][0] - v1[0][0] * v1[1][2] * v1[2][1] - v1[0][1] * v1[1][0] * v1[2][2];
   ;
}

__device__ float hit(ray_dev_t ray, triangle_dev_t *tri);
{
   float result = -1;
   float bBeta, bGamma, bT;
   float detA;

   float A[3][3];
   A[0][0] = location.x()-corner2.x();
   A[0][1] = location.x()-corner3.x();
   A[0][2] = ray.dir.x();
   A[1][0] = location.y()-corner2.y();
   A[1][1] = location.y()-corner3.y();
   A[1][2] = ray.dir.y();
   A[2][0] = location.z()-corner2.z();
   A[2][1] = location.z()-corner3.z();
   A[2][2] = ray.dir.z();

   detA = det3(A);

   float baryT[3][3];
   baryT[0][0] = location.x()-corner2.x();
   baryT[0][1] = location.x()-corner3.x();
   baryT[0][2] = location.x()-ray.point.x();
   baryT[1][0] = location.y()-corner2.y();
   baryT[1][1] = location.y()-corner3.y();
   baryT[1][2] = location.y()-ray.point.y();
   baryT[2][0] = location.z()-corner2.z();
   baryT[2][1] = location.z()-corner3.z();
   baryT[2][2] = location.z()-ray.point.z();

   bT = det3(baryT) / detA;
   if (bT < 0)
   {
      result = 0;
   }
   else
   {
      float baryGamma[3][3];

      baryGamma[0][0] = location.x()-corner2.x();
      baryGamma[0][1] = location.x()-ray.point.x();
      baryGamma[0][2] = ray.dir.x();
      baryGamma[1][0] = location.y()-corner2.y();
      baryGamma[1][1] = location.y()-ray.point.y();
      baryGamma[1][2] = ray.dir.y();
      baryGamma[2][0] = location.z()-corner2.z();
      baryGamma[2][2] = location.z()-ray.point.z();
      baryGamma[2][3] = ray.dir.z();

      bGamma = det3(baryGamma) / detA;
      if (bGamma < 0 || bGamma > 1)
      {
         result = 0;
      }
      else
      {
         float baryBeta[3][3];

         baryBeta[0][0] = location.x()-ray.point.x();
         baryBeta[0][1] = location.x()-corner3.x();
         baryBeta[0][2] = ray.dir.x();
         baryBeta[1][0] = location.y()-ray.point.y();
         baryBeta[1][1] = location.y()-corner3.y();
         baryBeta[1][2] = ray.dir.y();
         baryBeta[2][0] = location.z()-ray.point.z();
         baryBeta[2][1] = location.z()-corner3.z();
         baryBeta[2][2] = ray.dir.z();

         bBeta = det3(baryBeta) / detA;
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
   return (result);
}

__device__ float hit(ray_dev_t ray, sphere_dev_t *sp)
=======
    return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

__device__ float hit(ray_dev_t ray, float *t, sphere_dev_t *sp)
>>>>>>> 4fabfe5fce695248b5da2724ded1529aa2c51cdb
{
   float oMinusC[3];
   oMinusC[0] = ray.point[0] - sp->point[0];
   oMinusC[1] = ray.point[1] - sp->point[1];
<<<<<<< HEAD
   oMinusC[2] = ray.point[2] - sp->point[2];

=======
   oMinusC[2] = ray.point[2] - sp->point[2];   
  
>>>>>>> 4fabfe5fce695248b5da2724ded1529aa2c51cdb
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
<<<<<<< HEAD
      return;

   ray_dev_t *ray = rays + x * height + y;
=======
       return;

   ray_dev_t *ray = rays + x * height + y;   
>>>>>>> 4fabfe5fce695248b5da2724ded1529aa2c51cdb
   intersection_t *result = intersects + x * height + y;

   result->index = -1;

   for (int i = 0; i < size; i++)
   {
      sphere_dev_t *curObject = &spheres[i];
      t = -1.0;
<<<<<<< HEAD
      t = hit(*ray, curObject);
=======
      t = hit(*ray, &t, curObject);
>>>>>>> 4fabfe5fce695248b5da2724ded1529aa2c51cdb

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
<<<<<<< HEAD

=======
   
>>>>>>> 4fabfe5fce695248b5da2724ded1529aa2c51cdb
   //Add intersection
   result->t = curDepth;
}


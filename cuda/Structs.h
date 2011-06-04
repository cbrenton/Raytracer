/**
 * Structs and non-virtual methods for using CUDA.
 * @author Chris Brenton
 * @date 5/8/2011
 */

#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef struct sphere_dev
{
   float point[3];
   float rad;
} sphere_dev_t;

typedef struct triangle_dev
{
   float c1[3];
   float c2[3];
   float c3[3];
} triangle_dev_t;

typedef struct intersection
{
   float t;
   int index;
} intersection_t;

__global__ void Intersection_Test(ray_dev_t *rays, sphere_dev_t *spheres,
      intersection_t *intersects, int size, int width, int height);

#define BLOCK_SIZE 8
#endif

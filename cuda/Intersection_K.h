#ifndef Intersection_K_H_
#define Intersection_K_H_

//#include "Ray.h"
//#include "Sphere.h"
//#include "Triangle.h"
//#include "Scene.h"

typedef struct matrix3_dev
{
   float **mat;
} matrix3_dev_t;

typedef struct sphere_dev
{
   float point[3];
   float rad;
} sphere_dev_t;

typedef struct ray_dev
{
   float dir[3], point[3];
} ray_dev_t;

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

//__global__ void Intersection_Test(ray_dev_t *rays, sphere_dev_t *spheres,
void Intersection_Test(ray_dev_t *rays, sphere_dev_t *spheres,
      intersection_t *intersects, int size, int width, int height);

#define BLOCK_SIZE 8
#endif

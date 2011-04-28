#include "vector.h"
#include "Sphere.h"
#include "Ray.h"
#include <stdio.h>

int main (int argc, char **argv) {
   vec3_t testLoc = vec3_t(0.0, 0.0, 0.0);
   vec3_t testDir = vec3_t(0.0, 0.0, -1.0);
   Ray testRay = Ray(testLoc, testDir);
   Sphere testSphere;
   testSphere.location = vec3_t(0.0, 0.0, -3.0);
   testSphere.radius = 2.0;
   float t = testSphere.hit(testRay);
   printf("t: %f\n", t);
   return 0;
}

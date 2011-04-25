#include "Vector3D.h"
#include "Sphere.h"
#include "Ray.h"
#include <stdio.h>

int main (int argc, char **argv) {
   Vector3D testLoc = Vector3D(0.0, 0.0, 0.0);
   Vector3D testDir = Vector3D(0.0, 0.0, -1.0);
   Ray testRay = Ray(testLoc, testDir);
   Sphere testSphere;
   testSphere.location = Vector3D(0.0, 0.0, -3.0);
   testSphere.radius = 2.0;
   float t = testSphere.hit(testRay);
   printf("t: %f\n", t);
   return 0;
}

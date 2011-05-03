#include "Ray.h"

Ray::Ray(vec3_t _pt, vec3_t _dir) :
   point(_pt), dir(_dir) {
   }

void Ray::debug()
{
   //point.print();
   //dir.print();
}

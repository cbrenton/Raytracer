#include "Ray.h"

Ray::Ray(Vector3D _pt, Vector3D _dir) :
   point(_pt), dir(_dir) {
   }

void Ray::debug()
{
   point.print();
   dir.print();
}

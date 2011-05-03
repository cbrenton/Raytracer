#ifndef __RAY_H__
#define __RAY_H__

#include "vector.h"

struct Ray
{
   vec3_t point;
   vec3_t dir;
   Ray()
   {
      point = vec3_t(0, 0, 0);
      dir = vec3_t(0, 1, 0);
   }
   Ray(vec3_t _pt, vec3_t _dir) :
      point(_pt), dir(_dir)
   {
   }
};

#endif

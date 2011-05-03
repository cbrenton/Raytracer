/**
 * Ray object.
 * @author Chris Brenton
 * @date 5/2/2011
 */

#ifndef __RAY_H__
#define __RAY_H__

#include "vector.h"

typedef struct ray
{
   vec3_t point;
   vec3_t dir;
   ray()
   {
      point = vec3_t();
      dir = vec3_t();
   }
   ray(vec3_t _pt, vec3_t _dir) :
      point(_pt), dir(_dir)
   {
   }
} ray_t;

#endif

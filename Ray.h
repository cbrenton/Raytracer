#ifndef __RAY_H__
#define __RAY_H__

#include "vector.h"
#include "Matrix4.h"

class Ray
{
   public:
      Ray();
      Ray(vec3_t _pt, vec3_t _dir);
      virtual ~Ray() {};
      void debug();
      Ray* transform(Matrix4 inv);

      vec3_t point;
      vec3_t dir;
};

#endif

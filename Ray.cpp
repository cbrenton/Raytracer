#include "Ray.h"

Ray::Ray()
{
   point = vec3_t(0, 0, 0);
   dir = vec3_t(0, 1, 0);
}

Ray::Ray(vec3_t _pt, vec3_t _dir) :
   point(_pt), dir(_dir) {
   }

void Ray::debug()
{
   printf("<%f, %f, %f>, <%f, %f, %f>\n",
         point.x(), point.y(), point.z(),
         dir.x(), dir.y(), dir.z());
}

Ray* Ray::transform(Matrix4 inv)
{
   Ray* ret = new Ray();
   *ret = *this;
   /*
   vec4_t d_in = vec4_t(ret->dir, 0.0);
   vec4_t p_in = vec4_t(ret->point, 1.0);
   vec4_t d_t = inv * d_in;
   vec4_t p_t = inv * p_in;
   */
   return ret;
   //return this;
}

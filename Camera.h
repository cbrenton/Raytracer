/**
 * Sterling Hirsh
 * Camera Class
 * 4/3/11
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>

#include "vector.h"

class Camera {
   public:
      Camera(std::istream& input);
      vec3_t location;
      vec3_t up;
      vec3_t right;
      vec3_t look_at;
   private:
};

#endif

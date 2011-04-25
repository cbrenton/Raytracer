/**
 * Sterling Hirsh
 * Camera Class
 * 4/3/11
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>

#include "Vector3D.h"

class Camera {
   public:
      Camera(std::istream& input);
      Vector3D location;
      Vector3D up;
      Vector3D right;
      Vector3D look_at;
   private:
};

#endif

/**
 * Light - not very heavy.
 * Sterling Hirsh
 * 4/5/11
 */

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <iostream>

#include "Vector3D.h"

class Light {
   public:
      Light(std::istream& input);
      Vector3D location;
      float r;
      float g;
      float b;
};

#endif

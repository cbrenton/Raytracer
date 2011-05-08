/**
 * Light - not very heavy.
 * Sterling Hirsh
 * 4/5/11
 */

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include <iostream>
#include <stdio.h>
#include "vector.h"

class Light {
   public:
      Light(std::istream& input);
      vec3_t location;
      float r;
      float g;
      float b;
      inline void debug()
      {
         printf("Light: (r,g,b) (%f, %f, %f)\n", r, g, b);
      }
};

#endif

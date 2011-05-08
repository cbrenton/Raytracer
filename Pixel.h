#ifndef __PIXEL_H
#define __PIXEL_H

#include <iostream>
#include <fstream>

class Pixel {
   public:
      Pixel(float _r = 0.0, float _g = 0.0, float _b = 0.0, float _a = 0.0);
      ~Pixel();
      void setColor(float _r, float _g, float _b, float _a = 1.0);
      void add(Pixel *other);
      void multiply(float scale);
      void print(std::ofstream& out);
      float r, g, b, a;
      void debug();
};

#endif

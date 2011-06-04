#ifndef __PIXEL_H
#define __PIXEL_H

#include <iostream>
#include <fstream>

class Pixel {
   public:
      //Pixel(float _r = 0.33f, float _g = 0.38f, float _b = 0.44f, float _a = 1.0);
      Pixel(float _r = 0, float _g = 0, float _b = 0, float _a = 0);
      ~Pixel();
      void setColor(float _r, float _g, float _b, float _a = 1.0);
      void add(Pixel other);
      void multiply(float scale);
      void clamp();
      void setClear();
      void print(std::ofstream& out);
      float r, g, b, a;
      void debug();
};

#endif

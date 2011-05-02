#ifndef __PIXEL_H
#define __PIXEL_H

#include <iostream>
#include <fstream>

typedef struct pixel
{
   float r;
   float g;
   float b;
   float a;
} pixel_t;

inline void printPixel(pixel_t pix, std::ofstream& out)
{
   out << (char)(pix.b * 255)
      << (char)(pix.g * 255)
      << (char)(pix.r * 255)
      << (char)(pix.a * 255);
}

#endif

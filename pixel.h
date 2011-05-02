#ifndef __PIXEL_H
#define __PIXEL_H

typedef struct pixel
{
   float r;
   float g;
   float b;
   float a;
} pixel_t;

inline void printPixel(pixel_t pix, std::ofstream& out)
{
   char b = (char)(pix.b * 255);
   char g = (char)(pix.g * 255);
   char r = (char)(pix.r * 255);
   char a = (char)(pix.a * 255);
   out << b << g << r << a;
   /*
   out << (char)(pix.b * 255)
      << (char)(pix.g * 255)
      << (char)(pix.r * 255)
      << (char)(pix.a * 255);
   */
}

#endif

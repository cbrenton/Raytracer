#include "Pixel.h"

Pixel::Pixel(float _r, float _g, float _b, float _a)
{
   setColor(_r, _g, _b, _a);
}

Pixel::~Pixel()
{
}

void Pixel::setColor(float _r, float _g, float _b, float _a)
{
   r = _r;
   g = _g;
   b = _b;
   a = _a;
}

void Pixel::print(std::ofstream& out)
{
   out << (char)(b * 255)
      << (char)(g * 255)
      << (char)(r * 255)
      << (char)(a * 255);
}

void Pixel::debug()
{
   printf("Pixel: %f, %f, %f, %f\n", r, g, b, a);
}

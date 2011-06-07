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

void Pixel::add(Pixel other)
{
   r += other.r;
   g += other.g;
   b += other.b;
   //a += other.a;
   a = 1;
}

void Pixel::multiply(float scale)
{
   r *= scale;
   g *= scale;
   b *= scale;
   a *= scale;
}

void Pixel::clamp()
{
   r = std::max(r, 0.0f);
   g = std::max(g, 0.0f);
   b = std::max(b, 0.0f);
   r = std::min(r, 1.0f);
   g = std::min(g, 1.0f);
   b = std::min(b, 1.0f);
}

void Pixel::setClear()
{
   r = 0.74f;
   g = 0.74f;
   b = 0.96f;
   a = 1;
}

void Pixel::print(std::ofstream& out)
{
   clamp();
   out << (char)(b * 255)
      << (char)(g * 255)
      << (char)(r * 255)
      << (char)(a * 255);
}

void Pixel::debug()
{
   printf("Pixel: %f, %f, %f, %f\n", r, g, b, a);
}

#ifndef __IMAGE_H
#define __IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "Pixel.h"

class Image {
   public:
      Image() {};
      Image(int w, int h);
      ~Image();
      Pixel ***data;
      void setSize(int w, int h);
      void setPixel(int x, int y, Pixel *pIn);
      void testPattern();
      void init();
      void writeHeader(std::ofstream& out);
      void write();
      int width;
      int height;
      std::string filename;
   //protected:
      //std::ofstream myfile;
};

#endif

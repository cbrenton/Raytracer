#ifndef __IMAGE_H
#define __IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include "pixel.h"

typedef struct image
{
      int width;
      int height;
      pixel_t **data;
      std::string filename;
} image_t;

inline void init(image_t cur)
{
   if (cur.width <= 0 || cur.height <= 0)
   {
      fprintf(stderr, "Invalid image dimensions.\n");
      exit(EXIT_FAILURE);
   }
   cur.data = new pixel_t*[width];
   for (int i = 0; i < cur.width; ++i) {
      cur.data[i] = new pixel_t[cur.height];
      for (int j = 0; j < cur.height; ++j) {
         cur.data[i][j] = pixel_t(0, 0, 0, 0);
      }
   }
}

inline void setPixel(image_t cur, int x, int y, pixel_t *pIn)
{
   cur.data[x][y]->r = pIn->r;
   cur.data[x][y]->g = pIn->g;
   cur.data[x][y]->b = pIn->b;
   cur.data[x][y]->a = pIn->a;
}

inline void testPattern(image_t cur)
{
   for (int i = 0; i < cur.width; i++)
   {
      for (int j = 0; j < cur.height; j++)
      {
         // Write image data.
         //float color = (float)(j % 2);
         float xStep = (float)i / (float)cur.width;
         float yStep = (float)j / (float)cur.height;
         //printf("yStep: %f at %d\n", yStep, j);
         float r = (float)(ceil(xStep * 10.0) / 10.0);
         float g = (float)(1.0 - ceil(yStep * 10.0) / 10.0);
         float b = (float)(1.0 - ceil(xStep * 10.0) / 10.0);
         float a = 1.0;
         pixel_t testPixel = pixel_t(r, g, b, a);
         //Pixel testPixel = Pixel(0.0, 0.0, color, a);
         setPixel(cur, i, j, &testPixel);
      }
   }
}

inline void writeHeader(image_t cur, ofstream& out)
{
   out << '\0'
      << '\0'
      << '\2' // Uncompressed RGB
      << '\0' << '\0'
      << '\0' << '\0'
      << '\0'
      << '\0' << '\0' // X origin
      << '\0' << '\0' // Y origin
      << (char) (cur.width & 0xFF)
      << (char) ((cur.width >> 8) & 0xFF)
      << (char) (cur.height & 0xFF)
      << (char) ((cur.height >> 8) & 0xFF)
      << (char) 32 // 32 bit bitmap
      << '\0';
}

inline void writeImage(image_t cur)
{
   char *name = (char*)cur.filename.c_str();
   ofstream myfile;
   myfile.open(name);
   if (!myfile)
   {
      cerr << "Error: unable to open " << name << endl;
      exit(EXIT_FAILURE);
   }
   else
   {
      cout << "Writing to file " << name << endl;
   }

   writeHeader(cur, myfile);

   for (int i = 0; i < cur.height; i++)
   {
      for (int j = 0; j < cur.width; j++)
      {
         printPixel(cur.data[j][i], myfile);
      }
   }
   
   myfile.close();
}


#endif

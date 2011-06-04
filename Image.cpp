#include "Image.h"

using namespace std;

Image::Image(int w, int h):
   width(w), height(h)
{
   init();
   filename = "../output/out.tga";
}

Image::~Image()
{
   for (int i = 0; i < width; ++i) {
      delete[] data[i];
   }
   delete[] data;
}

void Image::init()
{
   if (width <= 0 || height <= 0)
   {
      fprintf(stderr, "Invalid image dimensions.\n");
      exit(EXIT_FAILURE);
   }
   data = new Pixel*[width];
   for (int i = 0; i < width; ++i) {
      data[i] = new Pixel[height];
      for (int j = 0; j < height; ++j) {
         data[i][j] = Pixel(0, 0, 0, 0);
      }
   }
}

void Image::setPixel(int x, int y, Pixel *pIn)
{
   data[x][y].r = pIn->r;
   data[x][y].g = pIn->g;
   data[x][y].b = pIn->b;
   data[x][y].a = pIn->a;
}

void Image::testPattern()
{
   for (int i = 0; i < width; i++)
   {
      for (int j = 0; j < height; j++)
      {
         // Write image data.
         float xStep = (float)i / (float)width;
         float yStep = (float)j / (float)height;
         float r = (float)(ceil(xStep * 10.0) / 10.0);
         float g = (float)(1.0 - ceil(yStep * 10.0) / 10.0);
         float b = (float)(1.0 - ceil(xStep * 10.0) / 10.0);
         float a = 1.0;
         Pixel testPixel = Pixel(r, g, b, a);
         setPixel(i, j, &testPixel);
      }
   }
}

void Image::writeHeader(ofstream& out)
{
   out << '\0'
      << '\0'
      << '\2' // Uncompressed RGB
      << '\0' << '\0'
      << '\0' << '\0'
      << '\0'
      << '\0' << '\0' // X origin
      << '\0' << '\0' // Y origin
      << (char) (width & 0xFF)
      << (char) ((width >> 8) & 0xFF)
      << (char) (height & 0xFF)
      << (char) ((height >> 8) & 0xFF)
      << (char) 32 // 32 bit bitmap
      << '\0';
}

void Image::write()
{
   char *name = (char*)filename.c_str();
   ofstream myfile;
   myfile.open(name);
   if (!myfile)
   {
      cerr << "Error: unable to open " << name << endl;
      exit(EXIT_FAILURE);
   }
   else
   {
      cout << "Writing to file " << name << "...";
   }

   writeHeader(myfile);

   for (int i = 0; i < height; i++)
   {
      for (int j = 0; j < width; j++)
      {
         data[j][i].print(myfile);
      }
   }

   cout << "done" << endl;

   myfile.close();
}

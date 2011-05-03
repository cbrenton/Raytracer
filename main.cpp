#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "pixel.h"
#include "image.h"
#include "Scene.h"
#include "Ray.h"

#define POV_EXT ".pov"
#define DEFAULT_W 256
#define DEFAULT_H 256

image_t image;
string inputFileName;
string filename;
int width = DEFAULT_W;
int height = DEFAULT_H;

void setWidth(char* strIn)
{
   if (strIn[0] == '=')
   {
      width = atoi(strIn + 1);
   }
   else
   {
      width = atoi(strIn);
   }
   if (width <= 0)
   {
      cerr << "Invalid width.\n";
      exit(EXIT_FAILURE);
   }
}

void setHeight(char* strIn)
{
   if (strIn[0] == '=')
   {
      height = atoi(strIn + 1);
   }
   else
   {
      height = atoi(strIn);
   }
   if (height <= 0)
   {
      cerr << "Invalid height: " << height << endl;
      exit(EXIT_FAILURE);
   }
}

void setFilename(char* strIn)
{
   string name = "";
   if (strIn[0] == '=')
   {
      name = strIn[1];
   }
   else
   {
      name = strIn;
   }
   inputFileName = name;
   int extIndex = (int)name.rfind(POV_EXT);
   filename = name.substr(0, extIndex);
   filename.append(".tga");
}

int main(int argc, char **argv)
{
   int c;
   while ((c = getopt(argc, argv, "w:W:h:H:i:I:")) != -1)
   {
      switch (c)
      {
      case 'w': case 'W':
         setWidth(optarg);
         break;
      case 'h': case 'H':
         setHeight(optarg);
         break;
      case 'i': case 'I':
         setFilename(optarg);
         break;
      default:
         cerr << "Invalid command-line argument -" << c << endl;
         exit(EXIT_FAILURE);
         break;
      }
   }
   for (int i = 1; i < argc; i++)
   {
      if (argv[i][0] == '+')
      {
         switch(argv[i][1])
         {
         case 'w': case 'W':
            setWidth(argv[i] + 2);
            break;
         case 'h': case 'H':
            setHeight(argv[i] + 2);
            break;
         case 'i': case 'I':
            setFilename(argv[i] + 2);
            break;
         default:
            cerr << "Invalid command-line argument -" << c << endl;
            exit(EXIT_FAILURE);
            break;
         }
      }
   }

   ifstream inputFileStream(inputFileName.c_str());

   Scene* scene = Scene::read(inputFileStream);

   image.width = width;
   image.height = height;
   image.filename = filename;
   initImage(&image);

   ray_t aRayArray [width][height];

   float l = -scene->camera.right.length() / 2;
   float r = scene->camera.right.length() / 2;
   float b = -scene->camera.up.length() / 2;
   float t = scene->camera.up.length() / 2;

   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         float uScale = (float)(l + (r - l) * ((i + 0.5) / image.width));
         float vScale = (float)(b + (t - b) * ((j + 0.5) / image.height));
         float wScale = -1;
         vec3_t sVector = scene->camera.location;
         vec3_t uVector = scene->camera.right;
         vec3_t vVector = scene->camera.up;
         vec3_t wVector = scene->camera.look_at -
            scene->camera.location;
         uVector.normalize();
         vVector.normalize();
         wVector.normalize();
         // Left-handed.
         wVector *= -1;
         uVector *= uScale;
         vVector *= vScale;
         wVector *= wScale;
         sVector += uVector;
         sVector += vVector;
         sVector += wVector;
         vec3_t rayDir = uVector + vVector + wVector;
         rayDir.normalize();
         vec3_t curPoint = vec3_t(scene->camera.location);
         ray_t curRay;
         curRay.point = curPoint;
         curRay.dir = rayDir;
         //Pixel *result = scene->getIntersect(*curRay);
         aRayArray[i][j] = curRay;
         //image.setPixel(i, j, result);
      }
   }

   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         //Pixel *result = scene->getIntersect(aRayArray[i][j]);
         HitData *data = scene->getIntersect(aRayArray[i][j]);
         pixel_t *result = new pixel_t();
         result->init();
         if (data->hit)
         {
            result = scene->seekLight(data, aRayArray[i][j].dir);
         }
         setPixel(image, i, j, result);
         delete data;
      }
   }

   writeImage(image);

   return 0;
}

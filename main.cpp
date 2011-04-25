#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "Pixel.h"
#include "Image.h"
#include "Scene.h"

#define POV_EXT ".pov"
#define DEFAULT_W 256
#define DEFAULT_H 256

using namespace std;

Image image;
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

   image = Image(width, height);
   image.filename = filename;
   image.init();

   //Ray aRayArray [width][height];

   float l = -scene->camera->right.getLength() / 2;
   float r = scene->camera->right.getLength() / 2;
   float b = -scene->camera->up.getLength() / 2;
   float t = scene->camera->up.getLength() / 2;

   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         float uScale = (float)(l + (r - l) * ((i + 0.5) / image.width));
         float vScale = (float)(b + (t - b) * ((j + 0.5) / image.height));
         float wScale = -1;
         Vector3D sVector = scene->camera->location;
         Vector3D uVector = scene->camera->right;
         Vector3D vVector = scene->camera->up;
         Vector3D wVector = scene->camera->look_at.subtract(
               scene->camera->location);
         uVector.normalize();
         vVector.normalize();
         wVector.normalize();
         // Left-handed.
         wVector.scalarMultiplyUpdate(-1);
         uVector.scalarMultiplyUpdate(uScale);
         vVector.scalarMultiplyUpdate(vScale);
         wVector.scalarMultiplyUpdate(wScale);
         sVector.addUpdate(uVector);
         sVector.addUpdate(vVector);
         sVector.addUpdate(wVector);
         Vector3D rayDir = uVector.add(vVector).add(wVector);
         rayDir.subtract(scene->camera->location);
         rayDir.normalize();
         Ray curRay = Ray(scene->camera->location, rayDir);
         //aRayArray[i][j] = curRay;
         Pixel *result = scene->getIntersect(curRay);
         image.setPixel(i, j, result);
      }
   }

   /*
   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         Pixel *result = scene->hit(aRayArray[i][j]);
         image.setPixel(i, j, result);
      }
   }
   */

   image.write();

   return 0;
}

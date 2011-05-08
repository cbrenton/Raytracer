#include <stdio.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <float.h>

#include "Pixel.h"
#include "Image.h"
#include "Scene.h"

#define POV_EXT ".pov"
#define DEFAULT_W 256
#define DEFAULT_H 256
#define AA_RAYS 8

// Set to 0 to turn off progress tracking.
#define SHOW_PROGRESS 1

// Determines the length of the progress bar. If your terminal is being overrun, try decreasing this.
#define BAR_LEN 20

using namespace std;

Image image;
Scene* scene;
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

// Testing triangle intersection. Should hit at 3.
void goodTest()
{
   vec3_t c1 = vec3_t(-1, -1, 3);
   vec3_t c2 = vec3_t(0, 1, 3);
   vec3_t c3 = vec3_t(1, -1, 3);
   Triangle *tri = new Triangle(c1, c2, c3);
   Ray *ray = new Ray(vec3_t(0, 0, 0), vec3_t(0, 0, 1));
   float t = -1.0f;
   bool hit = tri->hit(*ray, &t);
   if (hit)
   {
      std::cout << "hit at " << t << std::endl;
   }
   else
   {
      std::cout << "no hit: " << t << std::endl;
   }

   // Should not hit.
}

// Testing triangle intersection. Should not hit.
void badTest()
{
   vec3_t c1 = vec3_t(-1, 0.1f, 3);
   vec3_t c2 = vec3_t(0, 1, 3);
   vec3_t c3 = vec3_t(1, 0.1f, 3);
   Triangle *tri = new Triangle(c1, c2, c3);
   Ray *ray = new Ray(vec3_t(0, 0, 0), vec3_t(0, 0, 1));
   float t = -1.0f;
   bool hit = tri->hit(*ray, &t);
   if (hit)
   {
      std::cout << "hit at " << t << std::endl;
   }
   else
   {
      std::cout << "no hit: " << t << std::endl;
   }
}

float randFloat()
{
   float maxJitter = 0.5f;
   float minJitter = -0.5f;
   //float maxJitter = scene->camera->right.length() / (float)image.width;
   //float minJitter = scene->camera->up.length() / (float)image.height;
   return (float)rand() / (float)FLT_MAX * (maxJitter - minJitter) - minJitter;
}

void printProgress(struct timeval startTime, int d, int total, int freq)
{
   // Initialize timekeeping variables.
   float timeLeft;
   float dt = 0;
   int seconds, useconds;
   int min, sec, ms;
   int dMin, dSec, dMs;
   min = sec = ms = dMin = dSec = dMs = 0;

   // Set padding for strings to their length (minus one for null
   // terminating character) plus a specified value.
   int strPad = 3;
   int pad = 4;

   // Get terminal width.
   struct winsize w;
   ioctl(0, TIOCGWINSZ, &w);
   int termW = w.ws_col;

   // Length of time string.
   int timeLen = 8;
   // Length of percent string.
   int percentLen = 7;

   int maxBarLen = (pad * 2 + strPad * 2) + (strlen("elapsed:") - 1)
      + (strlen("eta:") - 1) + (timeLen * 2) + (percentLen + 1)
      + (BAR_LEN + 2) + 1;
   int midBarLen = (pad + strlen("eta:") - 1 + strPad + timeLen
         + (percentLen + 1) + 1);
   int minBarLen = percentLen + 1;

   //bool fullProgressEnabled = maxBarLen > BAR_LEN;
   bool fullProgressEnabled = maxBarLen < termW;
   bool midProgressEnabled = midBarLen < termW;
   bool minProgressEnabled = minBarLen < termW;

   if (d % freq == 0 || d == total - 1)
   {
      // Get time.
      struct timeval curTime;
      gettimeofday(&curTime, NULL);
      seconds = (int)curTime.tv_sec - startTime.tv_sec;
      useconds = (int)curTime.tv_usec - startTime.tv_usec;
      dt = (float)(((seconds) * 1000 + useconds/1000.0) + 0.5);
      float percent = (float)(d + 1) / (float)total;

      timeLeft = ((float)dt / percent - (float)dt) / 1000.0f;

      // Calculate time data;
      min = (int)timeLeft / 60;
      sec = (int)timeLeft % 60;
      ms = (int)(timeLeft * 100) % 60;

      dMin = (int)(dt / 1000) / 60;
      dSec = (int)(dt / 1000) % 60;
      dMs = (int)(dt / 10) % 60;

      if (fullProgressEnabled)
      {
         // Print everything.
         std::string progress;
         // Fill progress bar.
         progress += "[";
         for (int j = 0; j < BAR_LEN; j++)
         {
            float j_percent = (float)j / (float)BAR_LEN;
            if (j_percent <= percent)
            {
               progress += "=";
            }
            else
            {
               progress += "-";
            }
         }
         progress += "]";

         // Print data.
         printf("\r%s%*s%02d:%02d:%02d",
               "elapsed:", strPad, "", dMin, dSec, dMs);
         printf("%*s%s%*s%02d:%02d:%02d",
               pad, "", "eta:", strPad, "", min, sec, ms);
         // Display progress bar.
         printf("%*s%*.2f%% %s",
               pad, "", percentLen - 2, percent * 100.0f, progress.c_str());
      }
      else if (midProgressEnabled)
      {
         // Print the percent and the ETA.
         printf("\r%-*s %02d:%02d:%02d",
               strlen("eta:") - 1 + strPad, "eta:", min, sec, ms);
         printf("%*s%.2f%%",
               pad, "", percent * 100.0f);
      }
      else if (minProgressEnabled)
      {
         // Print only the percent.
         printf("\r%.2f%%", percent * 100.0f);
      }

      /*
         else
         {
         printf("Warning: terminal must be at least %d characters wide. Data will not be displayed.\n", minBarLen);
         }
         printf("terminal width: %d (%d)", termW, maxBarLen);
         */

      // Flush stdout to print stats.
      fflush(stdout);
   }
}

int main(int argc, char **argv)
{
   /*
   // Triangle testing.
   goodTest();
   badTest();
   exit(0);
   */

   srand(time(NULL));

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

   scene = Scene::read(inputFileStream);

   image = Image(width, height);
   image.filename = filename;
   image.init();

   // Make AA_RAYS subpixels per pixel.
   Ray aRayArray [width][height];

   float l = -scene->camera->right.length() / 2;
   float r = scene->camera->right.length() / 2;
   float b = -scene->camera->up.length() / 2;
   float t = scene->camera->up.length() / 2;

   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         //Ray *curRay = new Ray();
         //for (int k = 0; k < AA_RAYS; k++)
         //{
         //float xJitter = randFloat();
         //float yJitter = randFloat();
         float xJitter = 0.0f;
         float yJitter = 0.0f;
         float uScale = (float)(l + (r - l) * (((float)i + xJitter + 0.5f)
                  / (float)image.width));
         //float uScale = (float)(l + (r - l) * (((float)i + 0.5f)
         float vScale = (float)(b + (t - b) * (((float)j + yJitter + 0.5f)
                  / (float)image.height));
         //float vScale = (float)(b + (t - b) * (((float)j + 0.5f)
         float wScale = -1;
         vec3_t sVector = scene->camera->location;
         vec3_t uVector = scene->camera->right;
         vec3_t vVector = scene->camera->up;
         vec3_t wVector = scene->camera->look_at - scene->camera->location;
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
         vec3_t curPoint = vec3_t(scene->camera->location);
         //Ray *curRay = new Ray(curRay->point, curRay->dir);
         //curRay = new Ray(curRay->point, curRay->dir);
         Ray *curRay = new Ray(curPoint, rayDir);
         aRayArray[i][j] = *curRay;
         /*
            if (k == 0)
            {
            aRayArray[i][j] = *curRay;
            }
            else
            {
         //aRayArray[i][j].dirs[k] = curRay->dir;
         //aRayArray[i][j].point = curRay->point;
         }
         */
         delete curRay;
      }
      //}
   }

   // Initialize variables for timekeeping.
   struct timeval startTime;
   gettimeofday(&startTime, NULL);

   std::cout << "starting intersection tests." << std::endl;
   for (int i = 0; i < image.width; i++)
   {
      for (int j = 0; j < image.height; j++)
      {
         Pixel *result = new Pixel();
         for (int k = 0; k < AA_RAYS; k++)
            //for (int k = 0; k < 1; k++)
         {
            // Find intersections and get pixel data.
            //Ray *tmpRay = new Ray(aRayArray[i][j].point, aRayArray[i][j].dirs[k]);
            /*
               Ray tmpRay = aRayArray[i][j];
               HitData *data = scene->getIntersect(tmpRay);
               if (data->hit)
               {
            //result->add(scene->seekLight(data, aRayArray[i][j].dirs[k]));
            //result->add(scene->seekLight(data, tmpRay.dir));
            result->add(scene->seekLight(data, aRayArray[i][j].dir));
            }
            */
            HitData *data = scene->getIntersect(aRayArray[i][j]);
            if (data->hit)
            {
               result->add(scene->seekLight(data, aRayArray[i][j].dir));
            }
            delete data;
         }
         result->multiply(1.0f / (float)AA_RAYS);
         if (SHOW_PROGRESS)
         {
            // Set the frequency of ticks to update every .01%, if possible.
            int tick = std::max(image.width*image.height/AA_RAYS / 10000, 1);
            printProgress(startTime, i * image.height + j,
                  image.width * image.height, tick);
         }
         image.setPixel(i, j, result);
         delete result;
      }
   }

   if (SHOW_PROGRESS)
   {
      std::cout << std::endl;
   }

   // Write image to file.
   image.write();

   return 0;
}

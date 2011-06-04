#include <stdio.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <sstream>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <math.h>
//#include <cutil.h>

#include "Pixel.h"
#include "Image.h"
#include "Scene.h"

// Include kernel structs and functions.
#include "cuda/Intersection_K.h"

#define POV_EXT ".pov"
#define DEFAULT_W 256
#define DEFAULT_H 256
#define AA_RAYS 4
#define RECURSION_DEPTH 6

// Set to 0 to turn off progress tracking.
#define SHOW_PROGRESS 1

// Determines the length of the progress bar. If your terminal is being overrun, try decreasing this.
#define BAR_LEN 20

using namespace std;

Image *image;
Scene *scene;
string inputFileName;
string filename;
int width = DEFAULT_W;
int height = DEFAULT_H;
int numAA = 1;

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

void setAA(char* strIn)
{
   if (strlen(strIn) == 0)
   {
      numAA = AA_RAYS;
   }
   else if (strIn[0] == '=')
   {
      numAA = atoi(strIn + 1);
   }
   else
   {
      numAA = atoi(strIn);
   }
   if (numAA < 1)
   {
      cerr << "Invalid antialiasing sample rate: " << numAA << endl;
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
   int dirIndex = (int)name.rfind('/');
   int extIndex = (int)name.rfind(POV_EXT);
   filename = "output/";
   filename.append(name.substr(dirIndex + 1, extIndex - dirIndex - 1));
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
      cout << "hit at " << t << endl;
   }
   else
   {
      cout << "no hit: " << t << endl;
   }
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
      cout << "hit at " << t << endl;
   }
   else
   {
      cout << "no hit: " << t << endl;
   }
}

float randFloat()
{
   float maxJitter = 0.5f;
   float minJitter = -0.5f;
   //float maxJitter = 1.0f;
   //float minJitter = -1.0f;
   return (float)rand()/(float)RAND_MAX*(maxJitter - minJitter);
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

   int maxBarLen = (pad * 2 + strPad * 2) + ((int)strlen("elapsed:") - 1)
      + ((int)strlen("eta:") - 1) + (timeLen * 2) + (percentLen + 1)
      + (BAR_LEN + 2) + 1;
   int midBarLen = (pad + (int)strlen("eta:") - 1 + strPad + timeLen
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
      seconds = (int)curTime.tv_sec - (int)startTime.tv_sec;
      useconds = (int)curTime.tv_usec - (int)startTime.tv_usec;
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
         string progress;
         // Fill progress bar.
         progress += "[";
         for (int j = 0; j < BAR_LEN; j++)
         {
            float j_percent = (float)j / (float)BAR_LEN;
            if (j_percent < percent)
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
               (int)strlen("eta:") - 1 + strPad, "eta:", min, sec, ms);
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

float r2d(float rads)
{
   return (float)(rads * 180 / M_PI);
}

void boxTest()
{
   vec3_t ray_o = vec3_t(0, 0, -10);
   vec3_t ray_d = vec3_t(0, 0, 1);
   //Ray *ray = new Ray(ray_o, ray_d);
   float t = -1;

   vector<Ray*> r_vec;
   r_vec.push_back(new Ray(ray_o, ray_d));
   //r_vec.push_back(new Ray(ray_o, vec3_t(0.1f, 0, -10)));
   //r_vec.push_back(new Ray(vec3_t(1.4f, 0, -10), ray_d));
   vector<Box*> b_vec;
   b_vec.push_back(new Box(vec3_t(-1, -1, -1), vec3_t(1, 1, 1)));
   b_vec.push_back(new Box(vec3_t(1, 0, 0), vec3_t(2, 1, 1)));
   b_vec.push_back(new Box(vec3_t(-1, 0, 0), vec3_t(1, 1, 1)));
   b_vec.push_back(new Box(vec3_t(100, 100, 100), vec3_t(1000, 1000, 1000)));
   for (unsigned i = 0; i < b_vec.size(); i++)
   {
      cout << "\n+===============+\n|" << "b_vec[" << i << "]:\t|\n+===============+" << endl;
      b_vec[i]->debug();
      if (b_vec[i]->hit(*r_vec[i % r_vec.size()], &t))
      {
         cout << "boxTest: hit at " << t << "." << endl;
      }
      else
      {
         cout << "boxTest: no hit." << endl;
      }
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

   /*
   // Box testing.
   boxTest();
   exit(0);
   */

   srand((int)time(NULL));

   int c;
   while ((c = getopt(argc, argv, "a::A::i:I:h:H:w:W:")) != -1)
   {
      switch (c)
      {
      case 'a': case 'A':
         if (optarg != NULL)
         {
            setAA(optarg);
         }
         else
         {
            setAA((char *)"");
         }
         break;
      case 'h': case 'H':
         setHeight(optarg);
         break;
      case 'i': case 'I':
         setFilename(optarg);
         break;
      case 'w': case 'W':
         setWidth(optarg);
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
         case 'a': case 'A':
            setAA(argv[i] + 2);
            cout << "o1" << endl;
            break;
         case 'h': case 'H':
            setHeight(argv[i] + 2);
            break;
         case 'i': case 'I':
            setFilename(argv[i] + 2);
            break;
         case 'w': case 'W':
            setWidth(argv[i] + 2);
            break;
         default:
            cerr << "Invalid command-line argument -" << c << endl;
            exit(EXIT_FAILURE);
            break;
         }
      }
   }

   ifstream inputFileStream;
   inputFileStream.open(inputFileName.c_str());

   if (inputFileStream.fail())
   {
      cerr << "File " << inputFileName << " does not exist." << endl;
      exit(EXIT_FAILURE);
   }
   scene = Scene::read(inputFileStream);

   image = new Image(width, height);
   image->filename = filename;
   //image->init();

   // Make numAA subpixels per pixel.
   Ray ***aRayArray = new Ray **[width];
   for (int i = 0; i < width; i++)
   {
      aRayArray[i] = new Ray *[height];
      for (int j = 0; j < height; j++)
      {
         aRayArray[i][j] = new Ray[numAA];
      }
   }

   float l = -scene->camera->right.length() / 2;
   float r = scene->camera->right.length() / 2;
   float b = -scene->camera->up.length() / 2;
   float t = scene->camera->up.length() / 2;

   /*
   //Figure out gridsize and launch kernal
   //dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
   int gridW = width / BLOCK_SIZE;
   int gridH = height / BLOCK_SIZE;

   if(width % BLOCK_SIZE != 0)
      gridW++;

   if(height % BLOCK_SIZE != 0)
      gridH++;

   //dim3 dimGrid(gridW, gridH);
   */

   cout << "Generating rays...";
   for (int i = 0; i < image->width; i++)
   {
      for (int j = 0; j < image->height; j++)
      {
         for (int k = 0; k < numAA; k++)
         {
            //float newI = (float)i;
            //float newJ = (float)j;
            float jitter = 0.5f;
            if (numAA > 1)
            {
               jitter = randFloat();
            }

            float uScale = (float)(l + (r - l) * ((float)i + jitter)
                  / (float)image->width);
            float vScale = (float)(b + (t - b) * ((float)j + jitter)
                  / (float)image->height);
            float wScale = -1;
            vec3_t sVector = scene->camera->location;
            vec3_t uVector = scene->camera->right;
            vec3_t vVector = scene->camera->up;
            vec3_t wVector = scene->camera->look_at
               - scene->camera->location;
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
            Ray *curRay = new Ray(curPoint, rayDir);
            aRayArray[i][j][k] = *curRay;
            delete curRay;
         }
      }
   }
   cout << "done" << endl;

   // Initialize variables for timekeeping.
   struct timeval startTime;
   gettimeofday(&startTime, NULL);

   if (numAA > 1)
   {
      cout << "Using " << numAA << "x AA." << endl;
   }
   else
   {
      cout << "Antialiasing is turned off." << endl;
   }

   cout << "Testing intersections." << endl;

   //scene->constructBVH();

   // Test intersections for each ray.
   for (int i = 0; i < image->width; i++)
   {
      for (int j = 0; j < image->height; j++)
      {
         Pixel *result = new Pixel();
         for (int k = 0; k < numAA; k++)
         {
            // Find intersections and get pixel data.
            Pixel tmp = Pixel();
            tmp = scene->getIntersect(aRayArray[i][j][k], RECURSION_DEPTH);
            tmp.multiply(1.0f / (float)numAA);
            result->add(tmp);
         }
         if (SHOW_PROGRESS)
         {
            // Set the frequency of ticks to update every .01%, if possible.
            int tick = max(image->width*image->height/numAA / 10000, 100);
            printProgress(startTime, i * image->height + j,
                  image->width * image->height, tick);
         }
         image->setPixel(i, j, result);
         delete result;
      }
   }

   if (SHOW_PROGRESS)
   {
      cout << endl;
   }

   // Write image to file.
   image->write();

   for (int i = 0; i < width; i++)
   {
      for (int j = 0; j < height; j++)
      {
         delete[] aRayArray[i][j];
      }
      delete[] aRayArray[i];
   }
   delete[] aRayArray;

   delete image;

   return 0;
}

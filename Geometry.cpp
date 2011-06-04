/**
 * Chris Brenton
 * Geometry base class
 * All objects in scenes will extend this.
 * 4/7/11
 */

#include "Geometry.h"
#include "Box.h"
#include <cstring>
#include <stdlib.h>

Geometry::Geometry()
{
   transform = Matrix4();
   //mat = Material(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
   mat = Material(0.0f, 0.0f, 0.0f);
}

Geometry::~Geometry()
{
   delete thisBBox;
}

bool Geometry::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   return -1.0;
}

bool Geometry::hitBVH(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   //return hit(ray, t, data, minT, maxT);
   float boxT;
   HitData boxData;
   if (thisBBox == NULL)
   {
      cerr << "null bounding box" << endl;
   }
   bool boxHit = thisBBox->hit(ray, &boxT, &boxData, minT, maxT);
   if (boxHit)
   {
      return hit(ray, t, data, minT, maxT);
   }
   return false;
}

bool Geometry::hitTransformed(Ray rayIn, float *t, HitData *data, float minT, float maxT)
{
   Ray *ray;
   ray = rayIn.transform(getInvM());
   return hit(*ray, t, data, minT, maxT);
}

vec3_t Geometry::getNormal(vec3_t point)
{
   return vec3_t(0.0, 0.0, 0.0);
}

Box* Geometry::bBox()
{
   Box *result = new Box(vec3_t(0, 0, 0), vec3_t(0, 0, 0));
   return result;
}

void Geometry::readOptions(std::istream& input)
{
   std::string line;
   bool isDone = false;
   getline(input, line);
   while (line != "}" && !isDone)
   {
      unsigned int count = 0;
      int curChar = 0;
      std::string option;
      while (count < line.length() && isspace(line[count]))
      {
         curChar = (int)line[count];
         count++;
      }
      while (count < line.length() && !isspace(line[count]))
      {
         curChar = line[count++];
         option += (char)curChar;
      }
      isDone = getOption(option, line.substr(count));
      getline(input, line);
   }
   transformInv = transform.inverse();
}

bool Geometry::getOption(std::string option, std::string line)
{
   bool result = false;
   int braceCount = 0;
   for (unsigned i = 0; i < line.size(); i++)
   {
      if (line[i] == '{')
      {
         braceCount++;
      }
      if (line[i] == '}')
      {
         braceCount--;
      }
   }
   if (braceCount < 0)
   {
      result = true;
   }
   //printf("%s line: %s\n", option.c_str(), line.c_str());
   if (option.compare("pigment") == 0)
   {
      //printf("pigment: %s\n", line.c_str());
      int scan = sscanf(line.c_str(), " { color rgb <%f, %f, %f> }",
            &mat.r, &mat.g, &mat.b);
      if (scan == 0)
      {
         scan = sscanf(line.c_str(), " { color rgbf <%f, %f, %f, %f> }",
               &mat.r, &mat.g, &mat.b, &mat.f);
      }
      //printf("\tPIGMENT %f. %f, %f\n", r, g, b);
   }
   else if (option.compare("finish") == 0)
   {
      int scan = -1;
      //char *tok;
      char *tok;
      char *lineptr = strdup(line.c_str());
      tok = strtok(lineptr, " {");
      while (tok != NULL)
      {
         if (strcmp(tok, "ambient") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.ambient);
         }
         else if (strcmp(tok, "specular") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.specular);
         }
         else if (strcmp(tok, "diffuse") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.diffuse);
         }
         else if (strcmp(tok, "roughness") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.roughness);
         }
         else if (strcmp(tok, "reflection") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.reflect);
         }
         else if (strcmp(tok, "refraction") == 0)
         {
            int tmp = -1;
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%d", &tmp);
            mat.refract = (tmp != 0);
         }
         else if (strcmp(tok, "ior") == 0)
         {
            tok = strtok(NULL, " {");
            scan = sscanf(tok, "%f", &mat.ior);
         }
         tok = strtok(NULL, " {");
      }
      free(lineptr);
      free(tok);
   }
   else if (option.compare("scale") == 0)
   {
      float x, y, z;
      sscanf(line.c_str(), " <%f, %f, %f>",
            &x, &y, &z);
      //printf("\tSCALE <%f, %f, %f>\n", x, y, z);
      Matrix4 newTrans = Matrix4(
            (float)x, 0, 0, 0,
            0, (float)y, 0, 0,
            0, 0, (float)z, 0,
            0, 0, 0, 1.0);
      transform = transform * newTrans;
      //transform.print();
   }
   else if (option.compare("translate") == 0)
   {
      float x, y, z;
      sscanf(line.c_str(), " <%f, %f, %f>",
            &x, &y, &z);
      //printf("\tTRANSLATE <%f, %f, %f>\n", x, y, z);
      Matrix4 newTrans = Matrix4(
            (float)x, 0, 0, 0,
            0, (float)y, 0, 0,
            0, 0, (float)z, 0,
            0, 0, 0, 1.0);
      transform = transform * newTrans;
      //transform.print();
   }
   else if (option.compare("rotate") == 0)
   {
      float rot[3];
      sscanf(line.c_str(), " <%f, %f, %f>",
            &rot[0], &rot[1], &rot[2]);
      //printf("\tROTATE <%f, %f, %f>\n", rot[0], rot[1], rot[2]);
      for (unsigned int i = 0; i < sizeof(rot) / sizeof(float); i++)
      {
         if (rot[i] != 0)
         {
            Matrix4 newTrans = Matrix4(
                  1, 0, 0, 0,
                  0, (float)cos(rot[i]), (float)-sin(rot[i]), 0,
                  0, (float)sin(rot[i]), (float)cos(rot[i]), 0,
                  0, 0, 0, 1);
            transform = transform * newTrans;
         }
      }
      //transform.print();
   }
   else
   {
      printf("\tinvalid option: %s\n", option.c_str());
      printf("\t\t%s\n", line.c_str());
   }
   return result;
}

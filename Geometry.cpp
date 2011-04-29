/**
 * Chris Brenton
 * Geometry base class
 * All objects in scenes will extend this.
 * 4/7/11
 */

#include "Geometry.h"

Geometry::Geometry()
{
   transform = Matrix4();
   mat = Material(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

Geometry::~Geometry()
{
}

bool Geometry::hit(Ray ray, float *t, float minT, float maxT)
{
   return -1.0;
}

vec3_t Geometry::getNormal(vec3_t point)
{
   return vec3_t(0.0, 0.0, 0.0);
}

void Geometry::readOptions(std::istream& input)
{
   std::string line;
   getline(input, line);
   while (line != "}")
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
      getOption(option, line.substr(count));
      getline(input, line);
   }
}

void Geometry::getOption(std::string option, std::string line)
{
   //printf("%s line: %s\n", option.c_str(), line.c_str());
   if (option.compare("pigment") == 0)
   {
      //printf("pigment: %s\n", line.c_str());
      sscanf(line.c_str(), " { color rgb <%f, %f, %f>}",
            &mat.r, &mat.g, &mat.b);
      //printf("\tPIGMENT %f. %f, %f\n", r, g, b);
   }
   else if (option.compare("finish") == 0)
   {
      sscanf(line.c_str(), " { ambient %f diffuse %f specular %f emissive %f}",
            //&ambient, &diffuse, &specular, &emissive);
            &mat.ambient, &mat.diffuse, &mat.specular, &mat.emissive);
      //printf("\tFINISH ambient %f diffuse %f\n", ambient, diffuse);
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
   }
}

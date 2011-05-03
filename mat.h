/**
 * Material and matrix structs.
 * Contains pigment, finish, and transform properties.
 * @author Chris Brenton
 * @date 04/23/11
 */

#ifndef __MAT_H__
#define __MAT_H__

#include <stdio.h>
#include "matrix.h"

typedef struct mat
{
   float r;
   float g;
   float b;
   float amb;
   float diff;
   float spec;
   float rough;
   mat()
   {
      r = g = b = 0;
      amb = diff = spec = rough = 0.5f;
   }
} mat_t;

inline void getOption(mat_t *cur, matrix4_t *transform, std::string option, std::string line)
{
   //printf("%s line: %s\n", option.c_str(), line.c_str());
   if (option.compare("pigment") == 0)
   {
      printf("pigment: %s\n", line.c_str());
      sscanf(line.c_str(), " { color rgb <%f, %f, %f>}",
            &cur->r, &cur->g, &cur->b);
      printf("\tPIGMENT %f. %f, %f\n", cur->r, cur->g, cur->b);
   }
   else if (option.compare("finish") == 0)
   {
      sscanf(line.c_str(), " { amb %f diff %f spec %f rough %f}",
            &cur->amb, &cur->diff, &cur->spec, &cur->rough);
      //printf("\tFINISH amb %f diff %f\n", amb, diff);
   }
   else if (option.compare("scale") == 0)
   {
      float x, y, z;
      sscanf(line.c_str(), " <%f, %f, %f>",
            &x, &y, &z);
      //printf("\tSCALE <%f, %f, %f>\n", x, y, z);
      matrix4_t newTrans;
      makeMatrix(newTrans,
            (float)x, 0, 0, 0,
            0, (float)y, 0, 0,
            0, 0, (float)z, 0,
            0, 0, 0, 1.0);
      //transform = transform * newTrans;
      matrixMult(transform, newTrans);
      //transform.print();
   }
   else if (option.compare("translate") == 0)
   {
      float x, y, z;
      sscanf(line.c_str(), " <%f, %f, %f>",
            &x, &y, &z);
      //printf("\tTRANSLATE <%f, %f, %f>\n", x, y, z);
      matrix4_t newTrans;
      makeMatrix(newTrans,
            (float)x, 0, 0, 0,
            0, (float)y, 0, 0,
            0, 0, (float)z, 0,
            0, 0, 0, 1.0);
      //transform = transform * newTrans;
      matrixMult(transform, newTrans);
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
            matrix4_t newTrans;
            makeMatrix(newTrans,
                  1, 0, 0, 0,
                  0, (float)cos(rot[i]), (float)-sin(rot[i]), 0,
                  0, (float)sin(rot[i]), (float)cos(rot[i]), 0,
                  0, 0, 0, 1);
            //transform = transform * newTrans;
            matrixMult(transform, newTrans);
         }
      }
      //transform.print();
   }
   else
   {
      printf("\tinvalid option: %s\n", option.c_str());
   }
}

inline void readOptions(mat_t *cur, matrix4_t *transform, std::istream& input)
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
      getOption(cur, transform, option, line.substr(count));
      getline(input, line);
   }
}

#endif

/**
 * Sterling Hirsh
 * Geometry Base class
 * All objects in scenes will extend this.
 */

#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include "vector.h"
//#include "Structs.h"
#include "Ray.h"
#include "Matrix4.h"
#include "Pixel.h"
#include "Material.h"

#define DIST_MAX 10000

class Geometry {
   public:
      Geometry();
      virtual ~Geometry();
      virtual bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      virtual vec3_t getNormal(vec3_t point);
      virtual void debug() {};
      Material mat;
      inline float getR()
      {
         return mat.r;
      }
      inline float getG()
      {
         return mat.g;
      }
      inline float getB()
      {
         return mat.b;
      }
      inline float getAmbient()
      {
         return mat.ambient;
      }
      inline float getDiffuse()
      {
         return mat.diffuse;
      }
      inline float getSpecular()
      {
         return mat.specular;
      }
      inline float getRoughness()
      {
         return mat.roughness;
      }
      inline float getReflect()
      {
         return mat.reflect;
      }
      /*
      float r;
      float g;
      float b;
      float ambient;
      float diffuse;
      float specular;
      float emissive; // Maybe?
      */
      vec3_t location;
   protected:
      void readOptions(std::istream& input);
      void getOption(std::string option, std::string line);
      //void getOption(std::string option, const char *line);
      // Also keep a matrix here for transforms.
      Matrix4 transform;
};

#endif

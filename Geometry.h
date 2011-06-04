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
#include "Ray.h"
#include "Matrix4.h"
#include "Pixel.h"
#include "Material.h"
#include "HitData.h"

#define DIST_MAX 10000

class Box;

class Geometry {
   public:
      Geometry();
      virtual ~Geometry();
      virtual bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      virtual bool hitBVH(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      virtual bool hitTransformed(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      virtual vec3_t getNormal(vec3_t point);
      virtual Box *bBox();
      virtual void debug() {};
      Box *thisBBox;
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
      inline float getF()
      {
         return mat.f;
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
      inline bool getRefract()
      {
         return mat.refract;
      }
      inline float getIOR()
      {
         return mat.ior;
      }
      inline Matrix4 getM()
      {
         return transform;
      }
      inline Matrix4 getInvM()
      {
         return transformInv;
      }
      vec3_t location;
   protected:
      void readOptions(std::istream& input);
      bool getOption(std::string option, std::string line);
      //void getOption(std::string option, const char *line);
      // Also keep a matrix here for transforms.
      Matrix4 transform;
      Matrix4 transformInv;
};

#endif

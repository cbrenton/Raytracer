/**
 * Sterling Hirsh
 * Cone object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __CONE_H__
#define __CONE_H__

#include "Geometry.h"

class Cone : public Geometry {
   public:
      Cone(std::istream& input);
      bool hit(Ray ray, float *t, float minT = 0.0, float maxT = DIST_MAX);
      void debug();
   private:
      // Center1 is Geometry::location
      Vector3D center2;
      float radius1;
      float radius2;
};

#endif



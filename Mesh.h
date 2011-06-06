/**
 * Sterling Hirsh
 * Mesh object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __MESH_H__
#define __MESH_H__

#include "Geometry.h"
#include "Box.h"
#include "Triangle.h"

class Mesh : public Geometry {
   public:
      Mesh() {nVertices = 0;};
      Mesh(std::istream& input);
      Box *bBox();
      bool isTriLine(std::string line, bool *isComment);
      void findTriLine(std::istream& input);
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      vec3_t getNormal(vec3_t point);
      void setMats(Material matIn);
      inline void debug()
      {
         printf("Mesh.\n\n");
         for (unsigned i = 0; i < faces.size(); i++)
            faces[i]->debug();
         printf("End of mesh.\n\n");
      }
   private:
      vector<vec3_t> points;
      vector<Triangle*> faces;
      int nVertices;
      //float extrema[3][2];
};

#endif



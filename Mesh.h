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
      ~Mesh();
      Box *bBox();
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      void setMats(Material matIn);
      inline void debug()
      {
         printf("Mesh.\n");
         printf("\nVertices:\n");
         for (unsigned i = 0; i < points.size(); i++)
            cout << *points[i] << endl;
         printf("\nFaces:\n");
         for (unsigned i = 0; i < faces.size(); i++)
            faces[i]->debug();
         printf("End of mesh.\n\n");
      }
   private:
      vector<vec3_t*> points;
      vector<Triangle*> faces;
      int nVertices, nFaces;
};

#endif



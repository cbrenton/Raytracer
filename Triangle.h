/**
 * Sterling Hirsh
 * Triangle object for raytracing.
 * 473
 * 4/5/11
 */

#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Geometry.h"
#include "cuda/Intersection_K.h"
#include "Box.h"

class Triangle : public Geometry
{
   public:
      Triangle();
      Triangle(std::istream& input);
      Triangle(vec3_t c1, vec3_t c2, vec3_t c3);
      ~Triangle();
      Box *bBox();
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      bool hit(vec3_t pt);
      vec3_t getNormal(vec3_t point = vec3_t(0, 0, 0));
      vec3_t getPoint(int pt);
      bool contains(vec3_t pt);
      bool contains(vec3_t pt1, vec3_t pt2);
      //int contains(vec3_t pt);
      bool isNeighbor(Triangle *other);
      bool isNeighbor(vec3_t c1, vec3_t c2);
      //vector<Triangle*> subdivide(Triangle *tri1, Triangle* tri2, Triangle* tri3);
      void findAdj(vector<Triangle*> tris);
      vector<Triangle*> subdivide();
      vec3_t getFacePoint();
      vec3_t getMidAvg(vec3_t pt);
      vec3_t getMidAvg();
      triangle_dev_t getStruct();
      void debug();
      vec3_t *points[3];
      vector<vec3_t*> edgePts;
   protected:
      // Corner1 is Geometry::location
      vec3_t corner2;
      vec3_t corner3;
      vec3_t facePt;
      vector<Triangle*> adj;
};

#endif

/**
 * Sterling Hirsh
 * Scene Class
 * This has everything a scene needs.
 * 4/3/11
 */

#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Geometry.h"
#include "Box.h"
#include "Cone.h"
#include "Plane.h"
#include "Sphere.h"
#include "Semi.h"
#include "Triangle.h"
#include "Pixel.h"
#include "bvh_node.h"

class Scene {
   public:
      Scene() {hasBVH = false;}
      virtual ~Scene();
      static Scene* read(std::istream& input);
      void constructBVH();
      bool hit(Ray ray, HitData *data);
      Pixel getIntersect(Ray ray, int depth = 1);
      Pixel seekLight(HitData *data, vec3_t view);
      vec3_t reflect(vec3_t d, vec3_t n);
      vec3_t refract(vec3_t d, vec3_t n, float n1, float n2, bool *success);
      Camera* camera;
      vector<Geometry*> geometry_vec;
   private:
      bool hasBVH;
      int lights_size, geometry_size, planes_size, spheres_size,
          triangles_size;
      bvh_node *sceneBVH;
      Light **lights;
      Geometry **geometry;
      Plane **planes;
      Sphere **spheres;
      Triangle **triangles;
};

#endif

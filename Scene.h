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

struct HitData
{
   bool hit;
   vec3_t point;
   float t;
   Geometry *object;
   HitData() :
      hit(false), t(-1.0)
   {
   }
};

class Scene {
   public:
      // Nothing yet.
      static Scene* read(std::istream& input);
      //Pixel* getIntersect(Ray ray);
      HitData* getIntersect(Ray ray);
      //Pixel* seekLight(vec3_t origin, Geometry* hitObject);
      Pixel* seekLight(HitData *data, vec3_t view);
      Camera* camera;
   private:
      int lights_size, geometry_size, planes_size, spheres_size,
          triangles_size;
      Light **lights;
      Geometry **geometry;
      Plane **planes;
      Sphere **spheres;
      Triangle **triangles;
};

#endif

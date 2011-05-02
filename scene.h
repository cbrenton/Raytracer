/**
 * Sterling Hirsh
 * Scene Class
 * This has everything a scene needs.
 * 4/3/11
 */

#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "camera.h"
#include "light.h"
//#include "geom.h"
//#include "box.h"
//#include "cone.h"
#include "plane.h"
#include "sphere.h"
//#include "semi.h"
//#include "triangle.h"
#include "pixel.h"

struct HitData
{
   bool hit;
   vec3_t point;
   float t;
   //Geometry *object;
   HitData() :
      hit(false), t(-1.0)
   {
   }
};

class Scene {
   public:
      // Nothing yet.
      static Scene* read(std::istream& input);
      HitData* getIntersect(Ray ray);
      pixel_t* seekLight(HitData *data, vec3_t view);
      Camera* camera;
   private:
      int lights_size, geometry_size, planes_size, spheres_size;
      Light **lights;
      plane_t **planes;
      sphere_t **spheres;
};

#endif

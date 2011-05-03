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
#include "Ray.h"
#include "Geometry.h"
//#include "mat.h"
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
   geom_t *object;
   HitData() : hit(false) {}
};

class Scene {
   public:
      // Nothing yet.
      static Scene* read(std::istream& input);
      //void read(std::istream& input);
      HitData* getIntersect(ray_t ray);
      pixel_t* seekLight(HitData *data, vec3_t view);
      camera_t camera;
   private:
      int lights_size, geometry_size, planes_size, spheres_size;
      light_t *lights;
      plane_t *planes;
      sphere_t *spheres;
};

#endif

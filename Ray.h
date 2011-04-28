#include "vector.h"

class Ray {
   public:
      Ray() {};
      Ray(vec3_t _pt, vec3_t _dir);
      ~Ray() {};
      vec3_t point, dir;
      //float cast(Scene *scene);
      void debug();
};

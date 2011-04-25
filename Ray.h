#include "Vector3D.h"

class Ray {
   public:
      Ray() {};
      Ray(Vector3D _pt, Vector3D _dir);
      ~Ray() {};
      Vector3D point, dir;
      //float cast(Scene *scene);
      void debug();
};

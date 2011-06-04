/**
 * Material class.
 * Contains pigment and finish properties.
 * @author Chris Brenton
 * @date 04/23/11
 */

#include <stdio.h>

class Material
{
   public:
      Material() { Material(0.0, 0.0, 0.0, 0.5,
            1.0, 0.0, 0.0, 1.0,
            0.0, false, 0.0); };
      Material(float _r, float _g, float _b, float _f = 0.5,
            float _a = 1.0, float _d = 0.0, float _s = 0.0, float _rough = 1.0,
            float _reflect = 0.0, bool _refract = false, float _ior = 0.0);
      ~Material();
      float r, g, b, f, ambient, diffuse, specular, roughness, reflect;
      bool refract;
      float ior;
      float emissive;
      inline void debug()
      {
         printf("%f, %f, %f, %f,\n%f, %f, %f, %f,\n%f, %d, %f\n",
               r, g, b, f,
               ambient, diffuse, specular, roughness,
               reflect, refract, ior);
      }
};

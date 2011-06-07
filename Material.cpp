/**
 * Material class.
 * Contains pigment and finish properties.
 * @author Chris Brenton
 * @date 04/23/11
 */

#include <stdlib.h>
#include "Material.h"

Material::Material()
{
   r = g = b = f = 0.0;
   ambient = diffuse = specular = roughness = 0.0;
   reflect = 0.0;
   refract = false;
   ior = 0.0;
};

Material::Material(float _r, float _g, float _b, float _f,
      float _a, float _d, float _s, float _rough,
      float _reflect, bool _refract, float _ior) :
   r(_r), g(_g), b(_b), f(_f),
   ambient(_a), diffuse(_d), specular(_s), roughness(_rough),
   reflect(_reflect), refract(_refract), ior(_ior)
{
}

void Material::random()
{
   r = 1.0f * (float)rand()/(float)RAND_MAX;
   g = 1.0f * (float)rand()/(float)RAND_MAX;
   b = 1.0f * (float)rand()/(float)RAND_MAX;
   ambient = 0.3f * (float)rand()/(float)RAND_MAX;
   diffuse = (1.0f - ambient) * (float)rand()/(float)RAND_MAX;
   specular = 0.3f * (float)rand()/(float)RAND_MAX;
}

Material::~Material()
{
}

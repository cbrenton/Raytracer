/**
 * Material class.
 * Contains pigment and finish properties.
 * @author Chris Brenton
 * @date 04/23/11
 */

#include "Material.h"

Material::Material(float _r, float _g, float _b,
      float _a, float _d, float _s, float _rough, float _reflect) :
   r(_r), g(_g), b(_b),
   ambient(_a), diffuse(_d), specular(_s),
   roughness(_rough), reflect(_reflect)
{
}

Material::~Material()
{
}

void Material::setPigment(float _r, float _g, float _b)
{
   r = _r;
   g = _g;
   b = _b;
}

void Material::setFinish(float _a, float _d, float _s)
{
   ambient = _a;
   diffuse = _d;
   specular = _s;
}

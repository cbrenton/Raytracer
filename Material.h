/**
 * Material class.
 * Contains pigment and finish properties.
 * @author Chris Brenton
 * @date 04/23/11
 */
class Material
{
   public:
      Material() {};
      Material(float _r, float _g, float _b, float _a, float _d, float _s);
      ~Material();
      void setPigment(float _r, float _g, float _b);
      void setFinish(float _a, float _d, float _s);
      float r, g, b, ambient, diffuse, specular;
      float emissive;
};

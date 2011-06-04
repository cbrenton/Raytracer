/**
 * HitData
 *
 * A struct to hold various data resulting from an intersection.
 * @author Chris Brenton
 * @date 06/03/2011
 */

class Geometry;

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

/**
 *
 * bvh_node
 *
 * A node in a bounding volume heirarchy tree.
 * Contains two children that can be either a geometry object or another node.
 *
 * @author Chris Brenton
 * @date 06/02/2011
 *
 */

#ifndef __BVH_NODE_H__
#define __BVH_NODE_H__

#include "Geometry.h"
#include "Box.h"

class bvh_node : public Geometry
{
   public:
      bvh_node() : Geometry() {thisBBox = new Box();};
      bvh_node(vector<Geometry*> A, int axis);
      //void construct(vector<Geometry*> A, int axis);
      virtual ~bvh_node();
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      Box *bBox();
      Geometry *left;
      Geometry *right;
      Box *thisBBox;
};

#endif

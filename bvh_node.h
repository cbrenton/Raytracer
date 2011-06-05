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
#include <algorithm>

struct xCmp
{
     bool operator ()(Geometry *lhs, Geometry *rhs)
     {
        return lhs->location.x() < rhs->location.x();
     }
};

struct yCmp
{
     bool operator ()(Geometry *lhs, Geometry *rhs)
     {
        return lhs->location.y() < rhs->location.y();
     }
};

struct zCmp
{
     bool operator ()(Geometry *lhs, Geometry *rhs)
     {
        return lhs->location.z() < rhs->location.z();
     }
};

class bvh_node : public Geometry
{
   public:
      bvh_node() : Geometry() {thisBBox = new Box();};
      bvh_node(vector<Geometry*> A, int axis);
      virtual ~bvh_node();
      bool hit(Ray ray, float *t, HitData *data = NULL, float minT = 0.0, float maxT = DIST_MAX);
      Box *bBox();
      Geometry *left;
      Geometry *right;
      Box *thisBBox;
      inline void debug()
      {
         if (left == NULL)
            cout << "empty bvh_node" << endl;
         else if (right == NULL)
         {
            cout << "bvh_node: 1" << endl;
            cout << "\tleft:" << endl;
            left->debug();
         }
         else
         {
            cout << "bvh_node: 2" << endl;
            cout << "\tleft:" << endl;
            left->debug();
            cout << "\tright:" << endl;
            right->debug();
         }
      }
};

#endif

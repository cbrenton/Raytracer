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

#include "bvh_node.h"

bvh_node::~bvh_node()
{
   delete left;
   delete right;
   delete thisBBox;
}

bvh_node::bvh_node(vector<Geometry*> A, int axis)
{
   thisBBox = new Box();
   // If there is only one object, put it on the left.
   if (A.size() == 1)
   {
      left = A[0];
      right = NULL;
      thisBBox = A[0]->bBox();
   }
   else if (A.size() == 2)
   {
      left = A[0];
      right = A[1];
      thisBBox->combine(A[0]->bBox(), A[1]->bBox());
   }
   else
   {
      // More than 2: sort and divide into 2 groups.
      switch (axis)
      {
      case 0:
         sort(A.begin(), A.end(), xCmp());
         break;
      case 1:
         sort(A.begin(), A.end(), yCmp());
         break;
      case 2:
         sort(A.begin(), A.end(), zCmp());
         break;
      }
      vector<Geometry*>::const_iterator first = A.begin();
      vector<Geometry*>::const_iterator mid = A.begin() + A.size() / 2 + 1;
      vector<Geometry*>::const_iterator last = A.end();
      vector<Geometry*> leftVec(first, mid);
      vector<Geometry*> rightVec(mid, last);
      left = new bvh_node(leftVec, (axis + 1) % 3);
      right = new bvh_node(rightVec, (axis + 1) % 3);
      thisBBox->combine(left->bBox(), right->bBox());
   }
}

bool bvh_node::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   if (left == NULL && right == NULL)
   {
      return false;
   }
   // If this node's bounding box is not intersected by the ray, return false.
   if (!bBox()->hit(ray, t, data))
   {
      return false;
   }
   else
   {
      float lT = -1;
      float rT = -1;
      HitData lData, rData;
      bool hitLeft = false;
      bool hitRight = false;

      // If there is a left node, calculate hit for the left side.
      if (left != NULL)
      {
         hitLeft = left->hit(ray, &lT, &lData);
      }
      // If there is a right node, calculate hit for the right side.
      if (right != NULL)
      {
         hitRight = right->hit(ray, &rT, &rData);
      }

      // If both boxes are hit, find both intersections and pick the one
      // closer to the ray origin.
      if (hitLeft && hitRight)
      {
         // If left intersection is closer than right intersection, return
         // the left intersection.
         if (lT < rT)
         {
            *t = lT;
            *data = lData;
            return true;
         }
         // If right intersection is closer than left intersection, return
         // the right intersection.
         else
         {
            *t = rT;
            *data = rData;
            return true;
         }
      }
      // If only the left box is hit, return that intersection.
      else if (hitLeft)
      {
         *t = lT;
         *data = lData;
         return true;
      }
      // If only the right box is hit, return that intersection.
      else if (hitRight)
      {
         *t = rT;
         *data = rData;
         return true;
      }
      // If no box is hit, return false.
      return false;
   }
}

Box* bvh_node::bBox()
{
   return thisBBox;
}

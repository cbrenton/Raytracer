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
   if (left != NULL)
   {
      delete left;
   }
   if (right != NULL)
   {
      delete right;
   }
   if (thisBBox != NULL)
   {
      delete thisBBox;
   }
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
      // TODO : Sort A by axis.
      vector<Geometry*>::const_iterator first = A.begin();
      vector<Geometry*>::const_iterator mid = A.begin() + A.size() / 2 + 1;
      vector<Geometry*>::const_iterator last = A.end();
      vector<Geometry*> leftVec(first, mid);
      vector<Geometry*> rightVec(mid, last);
      left = new bvh_node(leftVec, (axis + 1) % 3);
      right = new bvh_node(rightVec, (axis + 1) % 3);
      thisBBox->combine(A[0]->bBox(), A[1]->bBox());
   }
}

bool bvh_node::hit(Ray ray, float *t, HitData *data, float minT, float maxT)
{
   //cerr << "bvh_node::hit()" << endl;
   if (left == NULL && right == NULL)
   {
      //cerr << "both nodes null" << endl;
      return false;
   }
   if (!bBox()->hit(ray, t, data))
   {
      //cerr << "did not hit." << endl;
      return false;
   }
   else
   {
      //cerr << "testing both nodes." << endl;
      float lT = -1;
      float rT = -1;
      HitData lData, rData;
      bool hitLeft = left->hit(ray, &lT, &lData);
      bool hitRight = right->hit(ray, &rT, &rData);
      if (hitLeft && hitRight)
      {
         if (lT > rT)
         {
            *t = lT;
            *data = lData;
            return true;
         }
         else
         {
            *t = rT;
            *data = rData;
            return true;
         }
      }
      else if (hitLeft)
      {
         *t = lT;
         return true;
      }
      else if (hitRight)
      {
         *t = rT;
         return true;
      }
      return false;
   }
}

Box* bvh_node::bBox()
{
   return thisBBox;
}

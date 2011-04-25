/**
 * Sterling Hirsh
 * Vector3D
 */

#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <stdio.h>

class Vector3D {
   public:
      float x, y, z;
      //const static float drawScale = 0.01;

      Vector3D(float _x = 0, float _y = 0, float _z = 0);
      Vector3D(Vector3D*);
      Vector3D cross(Vector3D&);
      Vector3D add(Vector3D);
      void addUpdate(Vector3D);
      float getLength();
      void normalize();
      void updateMagnitude(float, float, float);
      void updateMagnitude(Vector3D*);
      void rotate(float deg, float _x, float _y, float _z);
      void rotate(float angle, const Vector3D& axis);
      void rotateByDegrees(float angle, const Vector3D& axis);
      Vector3D scalarMultiply(float);
      void scalarMultiplyUpdate(float);
      Vector3D getNormalVector();
      void subtractUpdate(const Vector3D rhs);
      void print() {
         printf("Vector: (%f, %f, %f)\n", x, y, z);
      }

      /**
       * Return a new vector with the difference of the magnitudes and the
       * pos of this vector.
       */
      inline Vector3D subtract(Vector3D& rhs) {
         Vector3D sum(x - rhs.x, y - rhs.y, z - rhs.z);
         return sum;
      }

      inline float dot(Vector3D& rhs) {
         return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
      }

      Vector3D &operator=(const Vector3D& rhs);
};

#endif

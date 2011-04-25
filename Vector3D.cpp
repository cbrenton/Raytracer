
#include <math.h>
#include <algorithm>
#include "Vector3D.h"

using namespace std;

float distance3D(float x1, float y1, float z1, float x2 = 0.0f,
 float y2 = 0.0f, float z2 = 0.0f);
float distance3D(float x1, float y1, float z1, float x2,
      float y2, float z2) {
   return (float)sqrt(
         (x2 - x1) * (x2 - x1) +
         (y2 - y1) * (y2 - y1) +
         (z2 - z1) * (z2 - z1));
}

// Default, magnitude-only, and magnitude-and-position constructors all in one.
Vector3D::Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

Vector3D::Vector3D(Vector3D* other) {
   updateMagnitude(other);
}

void Vector3D::updateMagnitude(float _x, float _y, float _z) {
   x = _x;
   y = _y;
   z = _z;
}

void Vector3D::updateMagnitude(Vector3D* inVector) {
   updateMagnitude(inVector->x, inVector->y, inVector->z);
}

void Vector3D::normalize() {
   float length = getLength();
   if (length == 0)
      return;

   x = x / length;
   y = y / length;
   z = z / length;
}

/**
 * Return a float of the distance from head to tail.
 */
float Vector3D::getLength() {
   return distance3D(x, y, z);
}

Vector3D Vector3D::cross(Vector3D& rhs) {
   Vector3D product(
    (y * rhs.z) - (z * rhs.y),
    (z * rhs.x) - (x * rhs.z),
    (x * rhs.y) - (y * rhs.x));
   return product;
}

/**
 * Return a new vector with the sum of the magnitudes and the pos of this vector.
 */
Vector3D Vector3D::add(Vector3D rhs) {
   Vector3D sum(x + rhs.x, y + rhs.y, z + rhs.z);
   return sum;
}

/**
 * Add rhs to this vector and save the result in this vector.
 */
void Vector3D::addUpdate(Vector3D rhs) {
   x += rhs.x;
   y += rhs.y;
   z += rhs.z;
}

void Vector3D::subtractUpdate(const Vector3D rhs) {
   x -= rhs.x;
   y -= rhs.y;
   z -= rhs.z;
}

/**
 * Angle is in radians.
 * Axis will be called u, v, w.
 * Found the body of this function at:
 * http://www.blitzbasic.com/Community/posts.php?topic=57616
 */
void Vector3D::rotate(float angle, const Vector3D& axis) {
   float u = axis.x;
   float v = axis.y;
   float w = axis.z;
   float ux = x * u;
   float uy = y * u;
   float uz = z * u;
   float vx = x * v;
   float vy = y * v;
   float vz = z * v;
   float wx = x * w;
   float wy = y * w;
   float wz = z * w;
   float sa = (float)sin(angle);
   float ca = (float)cos(angle);
   x = u * (ux+vy+wz) + (x * (v*v + w*w) - u * (vy+wz)) * ca + (-wy+vz) * sa;
   y = v * (ux+vy+wz) + (y * (u*u + w*w) - v * (ux+wz)) * ca + (wx-uz)  * sa;
   z = w * (ux+vy+wz) + (z * (u*u + v*v) - w * (ux+vy)) * ca + (-vx+uy) * sa;
   normalize();
}

void Vector3D::rotateByDegrees(float angle, const Vector3D& axis) {
   rotate(angle / (180.0f / 3.14159265f), axis);
}

Vector3D Vector3D::scalarMultiply(float scalar) {
   Vector3D product (x * scalar, y * scalar, z * scalar);
   return product;
}

void Vector3D::scalarMultiplyUpdate(float scalar) {
   x *= scalar;
   y *= scalar;
   z *= scalar;
}

Vector3D Vector3D::getNormalVector() {
   float closestToZero = (float)min(fabs(x), min(fabs(y), fabs(z)));
   Vector3D otherPoint(1, 1, 1);

   if (fabs(x) == closestToZero) {
      otherPoint.updateMagnitude(1, 0, 0);
   } else if (fabs(y) == closestToZero) {
      otherPoint.updateMagnitude(0, 1, 0);
   } else {
      otherPoint.updateMagnitude(0, 0, 1);
   }
   Vector3D normal(cross(otherPoint));
   return normal;
}

Vector3D &Vector3D::operator=(const Vector3D& rhs) {
   x = rhs.x;
   y = rhs.y;
   z = rhs.z;
   return *this;
}

/**
 * @file
 * Implementation of 4x4 Matrix class.
 *
 * @author Mike Smith
 */

#include "Matrix4.h"
#include "vector.h"
#include <math.h>

const Matrix4 Matrix4::Identity = Matrix4(1, 0, 0, 0,
                                          0, 1, 0, 0,
                                          0, 0, 1, 0,
                                          0, 0, 0, 1);

Matrix4::Matrix4() {
   toIdent();
}

/** Yes, this is a very ugly constructor. But it's convenient, so there is no
 * reason for it to be changed.
 */
Matrix4::Matrix4(float m11, float m12, float m13, float m14,
                 float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34,
                 float m41, float m42, float m43, float m44) {
   _11 = m11;
   _12 = m12;
   _13 = m13;
   _14 = m14;
   _21 = m21;
   _22 = m22;
   _23 = m23;
   _24 = m24;
   _31 = m31;
   _32 = m32;
   _33 = m33;
   _34 = m34;
   _41 = m41;
   _42 = m42;
   _43 = m43;
   _44 = m44;
}

Matrix4::Matrix4(float _m[4][4]) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] = _m[i][j];
}

Matrix4::Matrix4(const Matrix4& copy) {
   this->copy(copy);
}

Matrix4::~Matrix4() {}

void Matrix4::print() {
   printf("\t[\t%f\t%f\t%f\t%f\t]\n", _11, _12, _13, _14);
   printf("\t[\t%f\t%f\t%f\t%f\t]\n", _21, _22, _23, _24);
   printf("\t[\t%f\t%f\t%f\t%f\t]\n", _31, _32, _33, _34);
   printf("\t[\t%f\t%f\t%f\t%f\t]\n", _41, _42, _43, _44);
}

void Matrix4::copy(const Matrix4 &rhs) {
   for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++)
         m[i][j] = rhs.m[i][j];
   }
}

Matrix4 Matrix4::fromvec3_t(const vec3_t& point) {
   Matrix4 temp((float) point.v[0], 0, 0, 0,
                (float) point.v[1], 0, 0, 0,
                (float) point.v[2], 0, 0, 0,
                0 , 0, 0, 0);
   return temp;
}

vec3_t Matrix4::getRow(int row) const {
   vec3_t t;
   if (row < 0 || row >= 4)
      return t;

   t.x(m[0][row]);
   t.y(m[1][row]);
   t.z(m[2][row]);

   return t;
}

vec3_t Matrix4::getCol(int col) const {
   vec3_t t;
   if (col < 0 || col >=4)
    return t;

   t.x(m[col][0]);
   t.y(m[col][1]);
   t.z(m[col][2]);

   return t;
}

Matrix4 &Matrix4::operator=(const Matrix4& rhs) {
   if (&rhs == this)
      return *this;

   copy(rhs);

   return *this;
}

Matrix4 &Matrix4::operator+=(const Matrix4 &rhs) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] += rhs.m[i][j];

   return *this;
}

const Matrix4 Matrix4::operator+(const Matrix4 &rhs) const {
   return Matrix4(*this) += rhs;
}

Matrix4 &Matrix4::operator-=(const Matrix4 &rhs) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] -= rhs.m[i][j];

   return *this;
}

const Matrix4 Matrix4::operator-(const Matrix4 &rhs) const {
   return Matrix4(*this) -= rhs;
}

Matrix4 &Matrix4::operator*=(float scale) {
   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
         m[i][j] *= scale;

   return *this;
}

const Matrix4 Matrix4::operator*(float scale) const {
   return Matrix4(*this) *= scale;
}

const Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
   Matrix4 t;

   for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++) {
         vec3_t col = rhs.getCol(j);
         t.m[i][j] = (float) (getRow(i).dot(col) + m[3][i] * rhs.m[j][3]);
      }

   return t;
}

const vec3_t Matrix4::operator*(const vec3_t& rhs) const {
   vec3_t newPoint;
   newPoint.x(_11 * rhs.v[0] + _21 * rhs.v[1] + _31 * rhs.v[2] + _41);
   newPoint.y(_12 * rhs.v[0] + _22 * rhs.v[1] + _32 * rhs.v[2] + _42);
   newPoint.z(_13 * rhs.v[0] + _23 * rhs.v[1] + _33 * rhs.v[2] + _43);

   return newPoint;
}

float Matrix4::det() const {
   float det = _11 * _22 * _33 * _44 + _11 * _23 * _34 * _42 +
                _11 * _24 * _32 * _43 + _12 * _21 * _34 * _43 +
                _12 * _23 * _31 * _44 + _12 * _24 * _33 * _41 +
                _13 * _21 * _32 * _44 + _13 * _22 * _34 * _41 +
                _13 * _24 * _31 * _42 + _14 * _21 * _33 * _42 +
                _14 * _22 * _31 * _43 + _14 * _23 * _32 * _41 -
                _11 * _22 * _34 * _43 - _11 * _23 * _32 * _44 -
                _11 * _24 * _33 * _42 - _12 * _21 * _33 * _44 -
                _12 * _23 * _34 * _41 - _12 * _24 * _31 * _43 -
                _13 * _21 * _34 * _42 - _13 * _22 * _31 * _44 -
                _13 * _24 * _32 * _41 - _14 * _21 * _32 * _43 -
                _14 * _22 * _33 * _41 - _14 * _23 * _31 * _42;
   return det;
}

float Matrix4::det3() const {
   float det = _11 * _22 * _33 + _12 * _23 * _31 + _13 * _21 * _32 -
               _13 * _22 * _31 - _11 * _23 * _32 - _12 * _21 * _33;
   return det;
}

Matrix4& Matrix4::toIdent() {
   *this = Identity;
   return *this;
}

Matrix4 Matrix4::inverse() const {
   Matrix4 temp;
   float d = det();
   if (d == 0)
      return temp;

   temp._11 = _22 * _33 * _44 + _23 * _34 * _42 + _24 * _32 * _43 -
              _22 * _34 * _43 - _23 * _32 * _44 - _24 * _33 * _42;

   temp._12 = _12 * _34 * _43 + _13 * _32 * _44 + _14 * _33 * _42 -
              _12 * _33 * _44 - _13 * _34 * _42 - _14 * _32 * _43;

   temp._13 = _12 * _23 * _44 + _13 * _24 * _42 + _14 * _22 * _43 -
              _12 * _24 * _43 - _13 * _22 * _44 - _14 * _23 * _42;

   temp._14 = _12 * _24 * _33 + _13 * _22 * _34 + _14 * _23 * _32 -
              _12 * _23 * _34 - _13 * _24 * _32 - _14 * _22 * _33;

   temp._21 = _21 * _34 * _43 + _23 * _31 * _44 + _24 * _33 * _41 -
              _21 * _33 * _44 - _23 * _34 * _41 - _24 * _31 * _43;

   temp._22 = _11 * _33 * _44 + _13 * _34 * _41 + _14 * _31 * _43 -
              _11 * _34 * _43 - _13 * _31 * _44 - _14 * _33 * _41;

   temp._23 = _11 * _24 * _43 + _13 * _21 * _44 + _14 * _23 * _41 -
              _11 * _23 * _44 - _13 * _24 * _41 - _14 * _21 * _43;

   temp._24 = _11 * _23 * _34 + _13 * _24 * _31 + _14 * _21 * _33 -
              _11 * _24 * _33 - _13 * _21 * _34 - _14 * _23 * _31;

   temp._31 = _21 * _32 * _44 + _22 * _34 * _41 + _24 * _31 * _42 -
              _21 * _34 * _42 - _22 * _31 * _44 - _24 * _32 * _41;

   temp._32 = _11 * _34 * _42 + _12 * _31 * _44 + _14 * _32 * _41 -
              _11 * _32 * _44 - _12 * _34 * _41 - _14 * _31 * _42;

   temp._33 = _11 * _22 * _44 + _12 * _24 * _41 + _14 * _21 * _42 -
              _11 * _24 * _42 - _12 * _21 * _44 - _14 * _22 * _41;

   temp._34 = _11 * _24 * _32 + _12 * _21 * _34 + _14 * _22 * _31 -
              _11 * _22 * _34 - _12 * _24 * _31 - _14 * _21 * _32;

   temp._41 = _21 * _33 * _42 + _22 * _31 * _43 + _23 * _32 * _41 -
              _21 * _32 * _43 - _22 * _33 * _41 - _23 * _31 * _42;

   temp._42 = _11 * _32 * _43 + _12 * _33 * _41 + _13 * _31 * _42 -
              _11 * _33 * _42 - _12 * _31 * _43 - _13 * _32 * _41;

   temp._43 = _11 * _23 * _42 + _12 * _21 * _43 + _13 * _22 * _41 -
              _11 * _22 * _43 - _12 * _23 * _41 - _13 * _21 * _42;

   temp._44 = _11 * _22 * _33 + _12 * _23 * _31 + _13 * _21 * _32 -
              _11 * _23 * _32 - _12 * _21 * _33 - _13 * _22 * _31;

   temp *= 1 / d;

   return temp;
}

Matrix4& Matrix4::toInverse() {
   *this = inverse();

   return *this;
}

Matrix4& Matrix4::toTranspose() {
   Matrix4 tmp(*this);

   for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
         m[i][j] = tmp.m[j][i];
      }
   }

   return *this;
}

Matrix4 Matrix4::transpose() const {
   Matrix4 tmp(*this);
   return tmp.toTranspose();
}

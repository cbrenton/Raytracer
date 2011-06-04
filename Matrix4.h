/**
 * @file
 * A simple 4x4 Matrix class.
 *
 * <pre>
 * A basic Matrix class to provide operations for 4x4 matrices. Included are
 * functions to invert, transpose, multiply, add, subtract, scale, and
 * find determinants. I'm also cutting support for the vec3_t class. vec3_t
 * is the correct class to use, despite the incorrect name.
 * </pre>
 *
 * @author Mike Smith
 */

#pragma once
#ifndef MATRIX4_H
#define MATRIX4_H

#include <stdio.h>

struct vec3_t;
struct vec4_t;

struct Matrix4 {
   union {
      struct {
         float _11, _21, _31, _41,
                _12, _22, _32, _42,
                _13, _23, _33, _43,
                _14, _24, _34, _44;
      };
      float m[4][4];
   };
   
   Matrix4(float m11, float m12, float m13, float m14,
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34,
           float m41, float m42, float m43, float m44);

   Matrix4(float _m[4][4]);
   Matrix4(const Matrix4& copy);
   Matrix4();

   ~Matrix4();

   static Matrix4 fromvec3_t(const vec3_t& point);
   
   void print();

   vec3_t getRow(int row) const;
   vec3_t getCol(int col) const;
   
   Matrix4 &operator=(const Matrix4& rhs);
   Matrix4 &operator+=(const Matrix4& rhs);
   Matrix4 &operator-=(const Matrix4& rhs);
   Matrix4 &operator*=(float scale);
   const vec3_t operator*(const vec3_t& rhs) const;
   const vec4_t operator*(const vec4_t& rhs) const;
   const Matrix4 operator*(const Matrix4& rhs) const;
   const Matrix4 operator-(const Matrix4& rhs) const;
   const Matrix4 operator*(float scale) const;
   const Matrix4 operator+(const Matrix4& rhs) const;

   float det() const;
   float det3() const;
   Matrix4& toIdent();
   Matrix4 inverse() const;
   Matrix4& toInverse();
   Matrix4 transpose() const;
   Matrix4& toTranspose();

   static const Matrix4 Identity;
   
   private:

   void copy(const Matrix4 &rhs);
};

#endif

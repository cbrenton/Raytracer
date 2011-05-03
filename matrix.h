#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef struct matrix4
{
   union {
      struct {
         float _11, _21, _31, _41,
                _12, _22, _32, _42,
                _13, _23, _33, _43,
                _14, _24, _34, _44;
      };
      float m[4][4];
   };
} matrix4_t;

inline void makeMatrix(matrix4_t cur,
      float m11, float m12, float m13, float m14,
      float m21, float m22, float m23, float m24,
      float m31, float m32, float m33, float m34,
      float m41, float m42, float m43, float m44)
{
   cur._11 = m11;
   cur._12 = m12;
   cur._13 = m13;
   cur._14 = m14;
   cur._21 = m21;
   cur._22 = m22;
   cur._23 = m23;
   cur._24 = m24;
   cur._31 = m31;
   cur._32 = m32;
   cur._33 = m33;
   cur._34 = m34;
   cur._41 = m41;
   cur._42 = m42;
   cur._43 = m43;
   cur._44 = m44;
}

inline void matrixMult(matrix4_t *first, matrix4_t second)
{
}

#endif

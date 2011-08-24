/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Matrix.cpp,v $
Revision 1.1.10.1  2006/04/13 17:45:53  jay
Add Translation to matrix

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Matrix.h"
#include "Common.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
// Inversion code borrowed from http://www.flipcode.com/documents/matrfaq.html

    typedef float MATRIX3[9];
    typedef float MATRIX4[16];

    typedef float VFLOAT;

    void m4_submat( const MATRIX4 mr, MATRIX3 mb, int i, int j )
      {
      int ti, tj, idst = 0, jdst = 0;

      for ( ti = 0; ti < 4; ti++ )
        {
        if ( ti < i )
          idst = ti;
        else
          if ( ti > i )
            idst = ti-1;

        for ( tj = 0; tj < 4; tj++ )
          {
          if ( tj < j )
            jdst = tj;
          else
            if ( tj > j )
              jdst = tj-1;

          if ( ti != i && tj != j )
            mb[idst*3 + jdst] = mr[ti*4 + tj ];
          }
        }
      }

      VFLOAT m3_det( MATRIX3 mat )
      {
      VFLOAT det;

      det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
          - mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
          + mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );

      return( det );
      }

    VFLOAT m4_det( const MATRIX4 mr )
      {
      VFLOAT  det, result = 0, i = 1;
      MATRIX3 msub3;
      int     n;

      for ( n = 0; n < 4; n++, i *= -1 )
        {
        m4_submat( mr, msub3, 0, n );

        det     = m3_det( msub3 );
        result += mr[n] * det * i;
        }

      return( result );
      }

    int m4_inverse( MATRIX4 mr, const MATRIX4 ma )
      {
      VFLOAT  mdet = m4_det( ma );
      MATRIX3 mtemp;
      int     i, j, sign;

      if ( fabs( mdet ) < 0.0005 )
        return( 0 );

      for ( i = 0; i < 4; i++ )
        for ( j = 0; j < 4; j++ )
          {
          sign = 1 - ( (i +j) % 2 ) * 2;

          m4_submat( ma, mtemp, i, j );

          mr[i+j*4] = ( m3_det( mtemp ) * sign ) / mdet;
          }

      return( 1 );
      }


bool Matrix::inverse(Matrix* pResult) const
{
    Assert(pResult);
    Matrix res;

    int success = m4_inverse(res.m_e, m_e);

    if (success)
    {
        *pResult = res;

#if defined(_DEBUG)
        // Let's check that this worked! Multiply this matrix by its inverse 
        // - we should get the identity.
        Matrix check = (*this) * res;
        Matrix id;
        id.identity();
        Assert(check == id);
        // Check it works the other way round too.
        check = res * (*this);
        Assert(check == id);
#endif
        return true;
    }

    return false;
}

Matrix::Matrix()
{
    m_e[0] = m_e[1] = m_e[2] = m_e[3] = 0;
    m_e[4] = m_e[5] = m_e[6] = m_e[7] = 0;
    m_e[8] = m_e[9] = m_e[10] = m_e[11] = 0;
    m_e[12] = m_e[13] = m_e[14] = m_e[15] = 0;
}

void Matrix::identity()
{
    m_e[0] = 1; m_e[1] = 0; m_e[2] = 0; m_e[3] = 0;
    m_e[4] = 0; m_e[5] = 1; m_e[6] = 0; m_e[7] = 0;
    m_e[8] = 0; m_e[9] = 0; m_e[10] = 1; m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0; m_e[14] = 0; m_e[15] = 1;
}


void Matrix::rotatex(float rads)
{
    float s = sin(rads), c = cos(rads);

    m_e[0] = 1;  m_e[1] = 0;     m_e[2] = 0;    m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = c;     m_e[6] = s;    m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = -s;    m_e[10] = c;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::rotatey(float rads)
{
    float s = sin(rads), c = cos(rads);

    m_e[0] = c;  m_e[1] = 0;     m_e[2] = -s;    m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = 1;     m_e[6] = 0;    m_e[7] = 0;
    m_e[8] = s;  m_e[9] = 0;     m_e[10] = c;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::rotatez(float rads)
{
    float s = sin(rads), c = cos(rads);

    m_e[0] = c;  m_e[1] = s;     m_e[2] = 0;    m_e[3] = 0;
    m_e[4] = -s; m_e[5] = c;     m_e[6] = 0;    m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = 0;     m_e[10] = 1;   m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0;    m_e[14] = 0;   m_e[15] = 1;
}

void Matrix::translate(float x, float y, float z)
{
    m_e[0] = 1;  m_e[1] = 0;     m_e[2] = 0;    m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = 1;     m_e[6] = 0;    m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = 0;     m_e[10] = 1;   m_e[11] = 0;
    m_e[12] = x; m_e[13] = y;    m_e[14] = z;   m_e[15] = 1;
}

void Matrix::Scale(float x, float y, float z)
{
    m_e[0] = x;  m_e[1] = 0;  m_e[2] = 0;  m_e[3] = 0;
    m_e[4] = 0;  m_e[5] = y;  m_e[6] = 0;  m_e[7] = 0;
    m_e[8] = 0;  m_e[9] = 0;  m_e[10] = z; m_e[11] = 0;
    m_e[12] = 0; m_e[13] = 0; m_e[14] = 0; m_e[15] = 1;
}

void Matrix::ModelView()
{
	AmjuGL::GetMatrix(AmjuGL::AMJU_MODELVIEW_MATRIX, m_e);
}

void Matrix::Projection()
{
	AmjuGL::GetMatrix(AmjuGL::AMJU_PROJECTION_MATRIX, m_e);
}

void Matrix::LoadMatrix()
{
//  glLoadMatrixf(m_e);
  AmjuGL::SetIdentity();
  AmjuGL::MultMatrix(m_e);
}

Matrix operator*(const Matrix& m, const Matrix& n)
{
  Matrix res;

  int nrows = 4;
  int ncolumns = 4;
  int nsummands = 4;
  for (int i = 0; i < nrows; i++) {
    for (int j = 0; j < ncolumns; j++) {
      for (int k = 0; k < nsummands; k++) {
        //c[i][j] = c[i][j] + a[i][k] * b[k][j];
        res.set(i, j, res.at(i, j) + m.at(i, k) * n.at(k, j) );
      }
    }
  }

  return res;
}

bool Matrix::operator==(const Matrix& rhs)
{
    for (int i = 0; i < 16; i++)
    {
        // Don't use straight comparison - floats won't exactly match!
        if (fabs(m_e[i] - rhs.m_e[i]) > SMALLEST)
        {
            return false;
        }
    }
    return true;
}

ostream& operator<<(ostream& os, const Matrix& m)
{
    os << m.m_e[0]  << "\t" << m.m_e[1]  << "\t" << m.m_e[2]  << "\t" << m.m_e[3]  << "\n"
       << m.m_e[4]  << "\t" << m.m_e[5]  << "\t" << m.m_e[6]  << "\t" << m.m_e[7]  << "\n"
       << m.m_e[8]  << "\t" << m.m_e[9]  << "\t" << m.m_e[10] << "\t" << m.m_e[11] << "\n"
       << m.m_e[12] << "\t" << m.m_e[13] << "\t" << m.m_e[14] << "\t" << m.m_e[15] << "\n";
    return os;
}

VertexBase operator*(VertexBase p, const Matrix& m)
{
    return VertexBase(m.m_e[0] * p.x + m.m_e[4] * p.y + m.m_e[8] * p.z + m.m_e[12],
		     m.m_e[1] * p.x + m.m_e[5] * p.y + m.m_e[9] * p.z + m.m_e[13],
		     m.m_e[2] * p.x + m.m_e[6] * p.y + m.m_e[10] * p.z + m.m_e[14]);
}

void Matrix::AddTranslation(const VertexBase& v)
{
  m_e[12] += v.x;
  m_e[13] += v.y;
  m_e[14] += v.z;
}
}

/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Matrix.h,v $
Revision 1.1.10.1  2006/04/13 17:45:53  jay
Add Translation to matrix

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(MATRIX_H_INCLUDED)
#define MATRIX_H_INCLUDED

#include "Vertex.h"
#include <iostream>

namespace Amju
{
class VertexBase;

class Matrix
{
public:
    Matrix();

    // Sets the matrix to the identity matrix.
    void identity();

    // Load the current OpenGL modelview matrix into this matrix.
    void ModelView();

    // Load the current OpenGL projection matrix into this matrix.
    void Projection();

    // Load this matrix into the current OpenGL matrix.
    void LoadMatrix();

    // These functions set the matrix values in order to perform the 
    // transformation.  
    // NB These functinos do not add to the existing transformation,
    // they overwrite it.
    void rotatex(float xradians);
    void rotatey(float yradians);
    void rotatez(float zradians);
    void translate(float x, float y, float z);
    void Scale(float x, float y, float z);

    // Add a translation to the matrix. 
    void AddTranslation(const VertexBase& v);

    // Matrix inversion.
    // Sets the matrix pointed to by pResult to be the inverse of the
    // current matrix, if this is possible. If successful, returns true.
    // (Inverse A' of matrix A is defined as
    //    A x A' = I (the identity matrix) and also A' x A = I.)
    bool inverse(Matrix* pResult) const;

    friend Matrix operator*(const Matrix& m, const Matrix& n);
    friend VertexBase operator*(VertexBase p, const Matrix& m);

    float at(int row, int col) const { return m_e[row * 4 + col]; }
 
    float at(int index) const { return m_e[index]; }

    void set(int row, int col, float t) { m_e[row * 4 + col] = t; } 

    bool operator==(const Matrix& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);

private:
    float m_e[16];
};
}
#endif

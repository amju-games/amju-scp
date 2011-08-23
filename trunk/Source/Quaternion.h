/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: Quaternion.h,v $
Revision 1.1.2.1  2005/03/28 11:10:13  jay
Added Quaternion class - needed for pool ball rotation.

*/

#ifndef AMJU_QUATERNION_H_INCLUDED
#define AMJU_QUATERNION_H_INCLUDED

namespace Amju
{
class Quaternion  
{
public:
  Quaternion();
  ~Quaternion();
  Quaternion operator *(Quaternion q);
  void CreateMatrix(float *pMatrix);
  void CreateFromAxisAngle(float x, float y, float z, float degrees);

private:
  float m_w;
  float m_z;
  float m_y;
  float m_x;
};
}

#endif 

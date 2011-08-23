/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextCurver.cpp,v $
Revision 1.1  2004/09/08 15:42:57  jay
Added to repository
  
*/

#include "TextCurver.h"
#include "AngleCompare.h"
#include <math.h>

namespace Amju
{
TextCurver::TextCurver()
{
  m_initialRot = 0;
  m_curvature = 0;
}

void TextCurver::SetInitialRot(float r)
{
  m_initialRot = r;
}

void TextCurver::SetCurvature(float deg)
{
  m_curvature = deg;
}

void TextCurver::Curve(float x, float* pX, float* pZ, float* pRot)
{
  const float yr1 = m_initialRot + m_curvature * x;
  const float yr = yr1 + 90.0f;
  *pX = x * (float)sin(DegToRad(yr));
  *pZ = x * (float)cos(DegToRad(yr));

  *pRot = yr1 * 2.0f; // I don't understand why this must be doubled .. oh well!
}
}


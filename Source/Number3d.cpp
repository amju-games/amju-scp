/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number3d.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Number3d.h"
#include "Common.h"
#include "Engine.h"

namespace Amju
{
PSolidComponent Number3d::s_digits[10] = { 0, 0, 0,   0, 0, 0,   0, 0, 0,   0 };
float Number3d::s_digitWidth;

Number3d::Number3d()
{
}

bool Number3d::Init()
{
  // Load the digits from file.
  for (int i = 0; i < 10; i++)
  {
    // Get the name
    char buf[10];
    sprintf(buf, "num_%d", i);
    // Look up name in config file to get Component filename.
    std::string filename = Engine::Instance()->GetConfigValue(buf);
    PSolidComponent pComp = SolidComponent::LoadSolid(filename);
    if (!pComp.GetPtr())
    {
      return false;
    }
    s_digits[i] = pComp;
  }

  s_digitWidth = atof(Engine::Instance()->GetConfigValue("num_width").c_str());
  return true;
}

void Number3d::Draw()
{
  AmjuGL::PushMatrix();
  
  float s;
  switch (m_justify)
  {
  case CENTRE:
    // Translate so middle of number is centre 
    s = (float)(m_digits.size() - 1) / 2.0f;
    AmjuGL::Translate(-s, 0, 0);
    break;
  case LEFT:
    break;
  case RIGHT:
    s = (float)(m_digits.size() -1);
    AmjuGL::Translate(-s, 0, 0);
  }

  // slight y adjust.
  AmjuGL::Translate(0, -0.5, 0);

  // Rotate numbers from flat to upright.
  AmjuGL::RotateX(90); //, 1, 0, 0);
  
  for (DigitList::iterator it = m_digits.begin(); it != m_digits.end(); ++it)
  {
    PSolidComponent pComp = *it;
    pComp->Draw();

    // Translate by width of a digit.
    AmjuGL::Translate(s_digitWidth, 0, 0);
  }
  AmjuGL::PopMatrix();
}

void Number3d::SetNumber(int n)
{
  if (n == m_number)
  {
    return;
  }

  m_digits.clear();
  m_number = n;
  // Decompose the number into a list of digits, by repeatedly dividing n by 10.
  do
  {
    int d = n % 10;
    Assert(d >= 0);
    Assert(d < 10);
    PSolidComponent pComp = s_digits[d];
    Assert(pComp.GetPtr());
    // d is the least significant digit of n, but more significant than any digits
    // already in the list. So this digit goes at the front of the list.
    m_digits.push_front(pComp);
    n /= 10;
  } while (n > 0);
}

}


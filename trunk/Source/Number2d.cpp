/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Number2d.cpp,v $
Revision 1.1  2004/09/08 15:42:53  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Number2d.h"
#include "Engine.h"

namespace Amju
{
TexturedQuad* Number2d::s_digits[10] = { 0, 0, 0,  0, 0, 0,  0, 0, 0,  0 };

Number2d::Number2d() : m_top(0), m_left(0), m_width(1), m_height(1)
{
}

void Number2d::SetPosition(float top, float left)
{
  m_top = top;
  m_left = left;
}

void Number2d::SetDigitSize(float width, float height)
{
  m_width = width;
  m_height = height;
}

bool Number2d::Init()
{
  for (int i = 0; i < 10; i++)
  {
    // Get the name
    char buf[20];

    // Look up name in config file to get .BMP filename.
    sprintf(buf, "num2d_%d", i);
    std::string filename = Engine::Instance()->GetConfigValue(buf);
    
    // Get alpha .BMP filename
    sprintf(buf, "num2d_%d_alpha", i);
    std::string alphaname = Engine::Instance()->GetConfigValue(buf);

    TexturedQuad* pQuad = new TexturedQuad;
    if (!pQuad->Load(filename, alphaname))
    {
      return false;
    }
    s_digits[i] = pQuad;
  }

  return true;
}

void Number2d::Draw()
{
  float top = m_top;
  float left = m_left;
 
  for (DigitList::iterator it = m_digits.begin(); it != m_digits.end(); ++it)
  {
    TexturedQuad* pQuad = *it;
    pQuad->Draw(top, left, top + m_height, left + m_width);

    left += m_width;
  }
}

void Number2d::SetNumber(int n)
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
    TexturedQuad* pQuad = s_digits[d];
    Assert(pQuad);
    // d is the least significant digit of n, but more significant than any digits
    // already in the list. So this digit goes at the front of the list.
    m_digits.push_front(pQuad);
    n /= 10;
  } while (n > 0);
}
}


/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: DecalShadow.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#include "DecalShadow.h"

namespace Amju
{
void DecalShadow::SetTexture(Texture* p)
{
  m_pTexture = p;
}
  
void DecalShadow::BindTexture()
{
  Assert(m_pTexture);
  m_pTexture->Bind();
}
}

  



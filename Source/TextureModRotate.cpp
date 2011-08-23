/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureModRotate.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextureModRotate.h"
#include "Engine.h"
#include "Common.h"

namespace Amju
{
TextureModRotate::TextureModRotate(Texture* pTex) : TextureModifier(pTex)
{
  m_time = 0;
}

void TextureModRotate::Update()
{
  float dt = Engine::Instance()->GetDeltaTime();
  m_time +=dt;
  if (m_time > 0.1f)
  {
    m_time = 0;

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
    AmjuGL::RotateZ(5.0f); //, 0, 0, 1.0f);
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

  }
}

}

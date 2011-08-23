/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureAnimation.cpp,v $
Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextureAnimation.h"
#include "Engine.h"

namespace Amju
{
TextureAnimation::TextureAnimation()
{
  m_element = 0;
  m_endElement = 0;
}

void TextureAnimation::Draw()
{
  if (m_element == m_endElement)
  {
    return;
  }

  m_pTextureSequence->Bind();

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);

  m_pTextureSequence->Draw(m_element);

  AmjuGL::PopAttrib();


  m_time += Engine::Instance()->GetDeltaTime();
  if (m_time > m_frameTime)
  {
    ++m_element;
    m_time = 0;
    if (m_loop && m_element == m_endElement)
    {
      m_element = m_startElement;
    }
  }
}

void TextureAnimation::Reset()
{
  m_element = m_endElement;
}

void TextureAnimation::Start()
{
  m_time = 0;
  m_element = m_startElement;
}

void TextureAnimation::Set(
  int startElement, int endElement, bool loop, float frameTime)
{
  m_frameTime = frameTime;
  m_loop = loop;
  m_startElement = startElement;
  m_endElement = endElement;
  m_element = m_startElement;
}

void TextureAnimation::SetSequence(TextureSequence* t)
{
  m_pTextureSequence = t;
}
}



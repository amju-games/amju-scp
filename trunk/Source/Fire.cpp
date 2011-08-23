/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Fire.cpp,v $
Revision 1.1  2004/09/08 15:42:49  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Fire.h"
#include "TextureServer.h"
#include "CompositeFactory.h"
#include <string>
#include "Engine.h"

namespace Amju
{
extern void ReportError(const std::string& error);

Fire::Fire() : m_fireTextureModifier(0)
{
  m_vertical = true; // Fire is always vertical.
}

void Fire::Update()
{
  m_fireTextureModifier.SetTexture(m_pTexture);
  m_fireTextureModifier.Update();
}

void Fire::Draw()
{
  //m_fireTextureModifier.SetTexture(m_pTexture);
  //m_fireTextureModifier.Update();
/*
  Engine::Instance()->PushColour(1.0f, 0.2f, 0.2f, 1.0f);
  AmjuGL::PushMatrix();
  BillBoard::Draw();
  AmjuGL::PopMatrix();
  Engine::Instance()->PopColour();
  return;
*/
  //m_fireTextureModifier.SetTexture(m_pTexture);
  //m_fireTextureModifier.Update();

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);

  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  //Engine::Instance()->PushColour(1.0f, 0.2f, 0.2f, 1.0f);

  AmjuGL::PushMatrix();
  GetOrientation()->Draw(); 
  BillBoard::Draw();
  AmjuGL::PopMatrix();

  //Engine::Instance()->PopColour();

  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::PopAttrib();
  AmjuGL::PopAttrib();
}

bool Fire::Load(const std::string& filename)
{
  return BillBoard::Load(filename);


#if defined(SCENE_EDITOR)
  m_name = filename;
#endif

  // We just need to know the size. 
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }  

  if (!f.GetFloat(&m_size))
  {
    f.ReportError("Expected fire radius.");
    return false;
  }

  m_pTexture = TextureServer::Instance()->Get("fire32.bmp", "fire32_a.bmp");
  if (!m_pTexture)
  {
    ReportError("Bad fire texture.");
    return false;
  }
  m_pTexture->CreateBinding(Texture::REGULAR);
  
  m_fireTextureModifier.SetTexture(m_pTexture);

  return true;
}
}


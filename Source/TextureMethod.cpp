/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: TextureMethod.cpp,v $
Revision 1.1.10.2  2005/03/19 22:05:11  Administrator
Scene Editor fixes, mainly to new NullWithCoords method

Revision 1.1.10.1  2005/03/18 22:02:38  jay
Added NullWithCoords method

Revision 1.1  2004/09/08 15:42:58  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "TextureMethod.h"
#include "Texture.h"
#include "File.h"
#include "TextureServer.h"
#include "LeafData.h"

namespace Amju
{
void TextureMethod::Bind()
{
  if (m_pTexture)
  {
    m_pTexture->Bind();
  }
}

PoolTexture* TextureMethod::GetTexture() 
{
  return m_pTexture; 
}

void TextureMethod::SetTexture(PoolTexture* p)
{ 
  m_pTexture = p; 

#if defined(SCENE_EDITOR)
  // Only required for Editor, where textures on meshes may be changed.
  // For release game, this is not used.

  // Set binding. Yuck, this should really be a virtual function.
  if (dynamic_cast<TexMethodRegular* const>(this) ||
      dynamic_cast<TexMethodNullWithCoords* const>(this))
  {
    m_pTexture->CreateBinding(Texture::REGULAR);
  }
  else if (dynamic_cast<TexMethodRegularClamp* const>(this))
  {
    // NB false parameter below means CLAMP.
    m_pTexture->CreateBinding(Texture::REGULAR, false);
  }
  else if (dynamic_cast<TexMethodAuto* const>(this))
  {
    m_pTexture->CreateBinding(Texture::AUTO_GENERATE);
  }
  else if (dynamic_cast<TexMethodEnv* const>(this))
  {
    m_pTexture->CreateBinding(Texture::ENV_MAP);
  }
#endif
}

// ----------------------------------------------------------------------------
#ifdef SCENE_EDITOR
bool TexMethodNull::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(4))
  {
    return false;
  }
  return true;
}

bool TexMethodNullWithCoords::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(6))
  {
    return false;
  }
  return true;
}
#endif

// ----------------------------------------------------------------------------

bool TexMethodRegular::Load(File* pf)
{
  m_pTexture = TextureServer::Instance()->Get(pf);
  if (!m_pTexture)
  {
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::REGULAR);
  return true;
}
 
#ifdef SCENE_EDITOR
bool TexMethodRegular::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(1))
  {
    return false;
  }

  if (!TextureServer::Instance()->Save(pf, m_pTexture))
  {
    return false;
  }
  return true;
}
#endif

void TexMethodRegular::SetTexGenMode() const
{
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
}

// ----------------------------------------------------------------------------

TexMethodAuto::TexMethodAuto()
{
  // Texture coord generation. These are default values for the coeffs.
  // See Red Book p.365.
  m_coeffSA = 1.0f;
  m_coeffSB = 0;
  m_coeffSC = 0;
  m_coeffSD = 0;
  m_coeffTA = 0;
  m_coeffTB = 1.0f;
  m_coeffTC = 0;
  m_coeffTD = 0;
}

bool TexMethodAuto::LoadNoBinding(File* pf)
{
  m_pTexture = TextureServer::Instance()->Get(pf);
  if (!m_pTexture)
  {
    return false;
  }
  // Get the coefficients for the texture coord generation.
  if (pf->GetVer() >= 6)
  {
    if (!pf->GetFloat(&m_coeffSA) ||
        !pf->GetFloat(&m_coeffSB) ||
        !pf->GetFloat(&m_coeffSC) ||
        !pf->GetFloat(&m_coeffSD) ||
        !pf->GetFloat(&m_coeffTA) ||
        !pf->GetFloat(&m_coeffTB) ||
        !pf->GetFloat(&m_coeffTC) ||
        !pf->GetFloat(&m_coeffTD))
    {
      pf->ReportError("Bad coefficients for texture coord generation.");
      return false;
    }
  }
  else
  {
    pf->ReportError("Old format auto texture.");
    return false;
  }
  return true;
}

bool TexMethodAuto::Load(File* pf)
{
  if (!LoadNoBinding(pf))
  {
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::AUTO_GENERATE);
  return true;
}

#ifdef SCENE_EDITOR
bool TexMethodAuto::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(2))
  {
    return false;
  }

  if (!TextureServer::Instance()->Save(pf, m_pTexture))
  {
    return false;
  }

  // Write the coeffs. for texture coord generation.
  pf->WriteComment("// Coeffs. for texture coord generation follow.");
  pf->WriteFloat(m_coeffSA);
  pf->WriteFloat(m_coeffSB);
  pf->WriteFloat(m_coeffSC);
  pf->WriteFloat(m_coeffSD);
  pf->WriteFloat(m_coeffTA);
  pf->WriteFloat(m_coeffTB);
  pf->WriteFloat(m_coeffTC);
  pf->WriteFloat(m_coeffTD);

  return true;
}
#endif

void TexMethodAuto::BeginDisplayList(LeafData* pLd)
{
/*
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);

  // Texture coord generation coefficients. See Red Book p.365.
  float scoeffs[4] = { m_coeffSA, m_coeffSB, m_coeffSC, m_coeffSD };
  float tcoeffs[4] = { m_coeffTA, m_coeffTB, m_coeffTC, m_coeffTD };
  glTexGenfv(GL_S, GL_OBJECT_PLANE, scoeffs);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, tcoeffs);
*/
}

void TexMethodAuto::EndDisplayList(LeafData* pLd)
{
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
  //AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
}

void TexMethodAuto::SetTexGenMode() const
{
  //glTexGeni(GL_S, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_OBJECT_LINEAR); 
  //glTexGeni(GL_T, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_OBJECT_LINEAR);
}

// ----------------------------------------------------------------------------

bool TexMethodEnv::Load(File* pf)
{
  m_pTexture = TextureServer::Instance()->Get(pf);
  if (!m_pTexture)
  {
    return false;
  }
  m_pTexture->CreateBinding(PoolTexture::ENV_MAP);
  return true;
}

#ifdef SCENE_EDITOR
bool TexMethodEnv::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(3))
  {
    return false;
  }

  if (!TextureServer::Instance()->Save(pf, m_pTexture))
  {
    return false;
  }
  return true;
}
#endif

void TexMethodEnv::BeginDisplayList(LeafData* pLd)
{
//  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
//  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
}

void TexMethodEnv::EndDisplayList(LeafData* pLd)
{
//  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_S);
//  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_MATRIX_GEN_T);
}

void TexMethodEnv::SetTexGenMode() const
{
//  glTexGeni(GL_S, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_SPHERE_MAP); 
//  glTexGeni(GL_T, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_SPHERE_MAP);
}

// ----------------------------------------------------------------------------

bool TexMethodRegularClamp::Load(File* pf)
{
  m_pTexture = TextureServer::Instance()->Get(pf);
  if (!m_pTexture)
  {
    return false;
  }
  // NB false parameter below means CLAMP.
  m_pTexture->CreateBinding(PoolTexture::REGULAR, false);
  return true;
}

#ifdef SCENE_EDITOR
bool TexMethodRegularClamp::Save(File* pf)
{
  if (!pf->WriteComment("// TextureMethod") ||
      !pf->WriteInteger(5))
  {
    return false;
  }
  return true;

}
#endif

}

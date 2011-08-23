/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Attrib.cpp,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Attrib.h"
#include "File.h"
#include "Common.h"
#include "Engine.h"
#include "TextureMethod.h"
#include "TextureMethodFactory.h"
#include "FunctionFactory.h"
#include "FunctionSimple.h" // TODO hard coded function type for Texture Scale

namespace Amju
{
const char* DepthAttrib::TypeName = "depth";

const char* DepthAttrib::GetTypeName() const
{
  return DepthAttrib::TypeName;
}

DepthAttrib::DepthAttrib() : m_bDepthBufferEnabled(true)
{
}

PAttrib DepthAttrib::Clone()
{
  return new DepthAttrib(*this);
}

void DepthAttrib::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_READ);
  if (m_bDepthBufferEnabled)
  {
    AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  }
  else 
  {
    AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  }
}

void DepthAttrib::Restore()
{
  AmjuGL::PopAttrib();
}

bool DepthAttrib::Load(File* pf)
{
  // 1 means turn it on; 2 means turn it off.
  int flag = 0;
  if (!pf->GetInteger(&flag))
  {
    pf->ReportError("Expected depth buffer flag.");
    return false;
  }
  if (flag == 1)
  {
    m_bDepthBufferEnabled = true;
  }
  else if (flag == 2)
  {
    m_bDepthBufferEnabled = false;
  }
  else
  {
    pf->ReportError("Bad depth test flag.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool DepthAttrib::Save(File* pf)
{
  pf->WriteInteger(m_bDepthBufferEnabled ? 1 : 2);
  return true;
}
#endif

//-----------------------------------------------------------------------------

const char* LightAttrib::TypeName = "light";

const char* LightAttrib::GetTypeName() const
{
  return LightAttrib::TypeName;
}

LightAttrib::LightAttrib() : m_bLightingEnabled(true)
{
}

PAttrib LightAttrib::Clone()
{
  return new LightAttrib(*this);
}

void LightAttrib::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  if (m_bLightingEnabled)
  {
    AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  }
  else 
  {
    AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  }
}

void LightAttrib::Restore()
{
  AmjuGL::PopAttrib();
}

bool LightAttrib::Load(File* pf)
{
  // 1 means enable lighting; 2 means disable lighting.
  int flag = 0;
  if (!pf->GetInteger(&flag))
  {
    pf->ReportError("Expected lighting flag.");
    return false;
  }
  if (flag == 1)
  {
    m_bLightingEnabled = true;
  }
  else if (flag == 2)
  {
    m_bLightingEnabled = false;
  }
  else
  {
    pf->ReportError("Bad lighting flag.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool LightAttrib::Save(File* pf)
{
  pf->WriteInteger(m_bLightingEnabled ? 1 : 2);
  return true;
}
#endif

//-----------------------------------------------------------------------------

const char* ColourAttrib::TypeName = "colour";

const char* ColourAttrib::GetTypeName() const
{
  return ColourAttrib::TypeName;
}

ColourAttrib::ColourAttrib()
{
  m_r = m_g = m_b = m_a = 1.0f;
}

PAttrib ColourAttrib::Clone()
{
  return new ColourAttrib(*this);
}

void ColourAttrib::Draw()
{
  // Draw new colour.
  Engine::Instance()->PushColour(m_r, m_g, m_b, m_a);
}

void ColourAttrib::Restore()
{
  // Restore saved colour.
  Engine::Instance()->PopColour();
}

bool ColourAttrib::Load(File* pf)
{
  if (!pf->GetFloat(&m_r))
  {
    pf->ReportError("Expected red colour attrib decorator.");
    return false;
  }
  if (!pf->GetFloat(&m_g))
  {
    pf->ReportError("Expected green colour attrib decorator.");
    return false;
  }
  if (!pf->GetFloat(&m_b))
  {
    pf->ReportError("Expected blue colour attrib decorator.");
    return false;
  }
  if (!pf->GetFloat(&m_a))
  {
    pf->ReportError("Expected alpha colour attrib decorator.");
    return false;
  }
  return true;
}

#if defined(SCENE_EDITOR)
bool ColourAttrib::Save(File* pf)
{
  pf->WriteFloat(m_r);
  pf->WriteFloat(m_g);
  pf->WriteFloat(m_b);
  pf->WriteFloat(m_a);
  return true;
}
#endif

void ColourAttrib::Set(float r, float g, float b, float a)
{
  m_r = r;
  m_g = g;
  m_b = b;
  m_a = a;
}

//-----------------------------------------------------------------------------

const char* BlendAttrib::TypeName = "blend";

const char* BlendAttrib::GetTypeName() const
{
  return BlendAttrib::TypeName;
}

PAttrib BlendAttrib::Clone()
{
  return new BlendAttrib(*this);
}

void BlendAttrib::Draw()
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);
  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
}

void BlendAttrib::Restore()
{
  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::PopAttrib();
  AmjuGL::PopAttrib();
}

bool BlendAttrib::Load(File* pf)
{
  // TODO load blend function flag.
  return true;
}

#if defined(SCENE_EDITOR)
bool BlendAttrib::Save(File* pf)
{
  return true;
}
#endif

//-----------------------------------------------------------------------------

const char* TextureRotateAttrib::TypeName = "texrotate";

const char* TextureRotateAttrib::GetTypeName() const
{
  return TextureRotateAttrib::TypeName;
}

PAttrib TextureRotateAttrib::Clone()
{
  return new TextureRotateAttrib(*this);
}

void TextureRotateAttrib::Draw()
{
  float dt = Engine::Instance()->GetDeltaTime();

  m_angle += dt * m_rotVel;
  if (m_angle >= 360.0f)
  {
    m_angle -= 360.0f;
  }
  if (m_angle <= -360.0f)
  {
    m_angle += 360.0f;
  }

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  // Rotate texture about Z axis.
  // TODO Load all these params!
  AmjuGL::RotateZ(m_angle);//, 0, 0, 1.0f);
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

void TextureRotateAttrib::Restore()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

bool TextureRotateAttrib::Load(File* pf)
{
  if (!pf->GetFloat(&m_rotVel))
  {
    pf->ReportError("Expected texture rotate vel.");
    return false;
  }

  return true;
}

#if defined(SCENE_EDITOR)
bool TextureRotateAttrib::Save(File* pf)
{
  pf->WriteComment("// Rotate vel");
  pf->WriteFloat(m_rotVel);

  return true;
}
#endif

//-----------------------------------------------------------------------------

const char* TextureScrollAttrib::TypeName = "texscroll";

const char* TextureScrollAttrib::GetTypeName() const
{
  return TextureScrollAttrib::TypeName;
}

TextureScrollAttrib::TextureScrollAttrib()
{
  m_time = 0;
  m_xvel = m_yvel = m_zvel = 0;
}

TextureScrollAttrib::TextureScrollAttrib(float x, float y, float z)
{
  m_time = 0;
  m_xvel = x;
  m_yvel = y;
  m_zvel = z;
}

PAttrib TextureScrollAttrib::Clone()
{
  return new TextureScrollAttrib(*this);
}

void TextureScrollAttrib::Draw()
{
  float dt = Engine::Instance()->GetDeltaTime();
  m_time +=dt;

  float x = m_time * m_xvel; 
  float y = m_time * m_yvel;
  float z = m_time * m_zvel;

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  AmjuGL::Translate(x, y, z);
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

void TextureScrollAttrib::Restore()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

bool TextureScrollAttrib::Load(File* pf)
{
  // Get x, y and z vels. Not sure if we need z, maybe for auto-generated coords.
  if (!pf->GetFloat(&m_xvel))
  {
    pf->ReportError("Expected x vel. for scrolling texture.");
    return false;
  }
  if (!pf->GetFloat(&m_yvel))
  {
    pf->ReportError("Expected y vel. for scrolling texture.");
    return false;
  }
  if (!pf->GetFloat(&m_zvel))
  {
    pf->ReportError("Expected z vel. for scrolling texture.");
    return false;
  }

  return true;
}

#if defined(SCENE_EDITOR)
bool TextureScrollAttrib::Save(File* pf)
{
  pf->WriteFloat(m_xvel);
  pf->WriteFloat(m_yvel);
  pf->WriteFloat(m_zvel);

  return true;
}
#endif

//-----------------------------------------------------------------------------

const char* TextureScaleAttrib::TypeName = "texscale";

TextureScaleAttrib::TextureScaleAttrib()
{
}

void TextureScaleAttrib::Draw()
{
  Orientation o(1.0f, 1.0f, 1.0f, 0, 0, 0);
  m_pFunc->Recalc(&o);
  float x = o.GetX();
  float y = o.GetY();
  float z = o.GetZ();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  AmjuGL::Scale(x, y, z);
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

void TextureScaleAttrib::Restore()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_TEXTURE_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
}

bool TextureScaleAttrib::Load(File* pf)
{
  // Create and load Function.
  m_pFunc = FunctionFactory::Instance()->Create(FunctionSimple::Name);
  return m_pFunc->Load(pf);
}

#if defined(SCENE_EDITOR)
bool TextureScaleAttrib::Save(File* pf)
{
  return m_pFunc->Save(pf);
}
#endif

PAttrib TextureScaleAttrib::Clone()
{
  return new TextureScaleAttrib(*this);
}

const char* TextureScaleAttrib::GetTypeName() const
{
  return TextureScaleAttrib::TypeName;
}

//-----------------------------------------------------------------------------

const char* TextureMethodAttrib::TypeName = "texmethod";

const char* TextureMethodAttrib::GetTypeName() const
{
  return TextureMethodAttrib::TypeName;
}

TextureMethodAttrib::TextureMethodAttrib() 
{
}

PAttrib TextureMethodAttrib::Clone()
{
  return new TextureMethodAttrib(*this);
}

void TextureMethodAttrib::Draw()
{
#if defined(SCENE_EDITOR)
  if (!m_pTextureMethod.GetPtr())
  {
    return;
  }
#endif

  Assert(m_pTextureMethod.GetPtr());

  m_pTextureMethod->Bind();
  m_pTextureMethod->BeginDisplayList(0); // sets texture-coord generation params
  // TODO Why does BeginDisplayList() need a param ?
  m_pTextureMethod->SetTexGenMode(); // sets mode (sphere/linear)
}

void TextureMethodAttrib::Restore()
{
#if defined(SCENE_EDITOR)
  if (!m_pTextureMethod.GetPtr())
  {
    return;
  }
#endif

  Assert(m_pTextureMethod.GetPtr());
  m_pTextureMethod->EndDisplayList(0); 
}

bool TextureMethodAttrib::Load(File* pf)
{
  m_pTextureMethod = TextureMethodFactory::Create(pf);

  if (!m_pTextureMethod->Load(pf))
  {
    return false;
  }

  return true;
}

#if defined(SCENE_EDITOR)
bool TextureMethodAttrib::Save(File* pf)
{
  Assert(m_pTextureMethod.GetPtr());

  if (!m_pTextureMethod.GetPtr())
  {
    return false;
  }
  if (!m_pTextureMethod->Save(pf))
  {
    return false;
  }
  return true;
}
#endif

}

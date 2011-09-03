/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: BlastWaveEffect.cpp,v $
Revision 1.1.10.1  2005/09/11 18:59:40  jay
Changed alpha effect, didn't look good before.

Revision 1.1  2004/09/08 15:42:33  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "BlastWaveEffect.h"
#include "Texture.h"
#include "Common.h"
#include "Engine.h"
#include "Polygon.h"
#include "Vertex.h"
#include "File.h"
#include "TextureServer.h"

namespace Amju
{
BlastWaveEffect::BlastWaveEffect()
{
  m_pTexture = 0;
  m_maxTime = 0;
  m_speed = 0;
  m_dead = true;
  m_size = 0;
  m_time = 0;
  m_xtilt = 0;
  m_ztilt = 0;
}

bool BlastWaveEffect::IsDead() const
{
  return m_dead;
}

bool BlastWaveEffect::Load(const std::string& fileName)
{
  File f;
  if (!f.OpenRead(fileName))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }
  m_pTexture = TextureServer::Instance()->Get(&f);
  if (!m_pTexture)
  {
    f.ReportError("Failed to load texture.");
    return false;
  }
  if (!f.GetFloat(&m_maxTime))
  {
    f.ReportError("Expected max time.");
    return false;
  }
  if (!f.GetFloat(&m_speed))
  {
    f.ReportError("Expected speed.");
    return false;
  }
  if (!f.GetFloat(&m_xtilt))
  {
    f.ReportError("Expected x tilt.");
    return false;
  }
  if (!f.GetFloat(&m_ztilt))
  {
    f.ReportError("Expected z tilt.");
    return false;
  }
  return true;
}

void BlastWaveEffect::Init(PoolTexture* pTex, float speed, float maxTime, float xtilt, float ztilt)
{
  m_pTexture = pTex;
  m_maxTime = maxTime;
  m_speed = speed;
  m_xtilt = xtilt;
  m_ztilt = ztilt;
}

void BlastWaveEffect::Draw()
{
  if (m_dead)
  {
    return;
  }

  // TODO tilt

  float deltaTime = Engine::Instance()->GetDeltaTime();
  m_time += deltaTime;
  if (m_time > m_maxTime)
  {
    m_dead = true;
  }

  m_size += m_speed * deltaTime;

  const float x1 = m_centre.x + m_size;
  const float x2 = m_centre.x - m_size;
  const float z1 = m_centre.z + m_size;
  const float z2 = m_centre.z - m_size;
  const float y = m_centre.y;

  Polygon poly;

  poly.AddVertex(SceneVertex(x1, y, z1));
  poly.AddTexVertex(TexVertex(0, 0));

  poly.AddVertex(SceneVertex(x1, y, z2));
  poly.AddTexVertex(TexVertex(0, 1));

  poly.AddVertex(SceneVertex(x2, y, z2));
  poly.AddTexVertex(TexVertex(1, 1));

  poly.AddVertex(SceneVertex(x2, y, z1));
  poly.AddTexVertex(TexVertex(1, 0));

  //poly.SetPerpendicularNormals(); 

  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING); 
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  m_pTexture->Bind();

  float a = 1.0f;
  //if (m_time / m_maxTime > 0.5f)
  //{
  //  a = 1.0f - m_time / m_maxTime;
  //}
  a = 1.0f - m_time / m_maxTime;
  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, a);
  poly.Draw();
  Engine::Instance()->PopColour();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::PopAttrib();
}

void BlastWaveEffect::Reset(const VertexBase& centre)
{
  if (!m_dead) 
  {
    return; // ?
  }

  m_centre = centre;
  m_size = 0;
  m_time = 0;
  m_dead = false;
}

}

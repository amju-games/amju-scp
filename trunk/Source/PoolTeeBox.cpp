/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PoolTeeBox.cpp,v $
Revision 1.1.2.1  2006/08/14 17:50:25  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:35  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.2  2005/07/30 12:21:33  jay
Always set orientation on load. Not doing this causes undo stack to not
pick up the initial orientation correctly.

Revision 1.1.8.1  2005/05/24 20:42:40  jay
Invisible for pool

Revision 1.1  2004/09/08 15:43:06  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "PoolTeeBox.h"
#include "File.h"
#include "SolidComponent.h"
#include "DecalShadow.h"
#include "Engine.h"
#include "TextureServer.h"

namespace Amju
{
static float TEE_BOX_DECAL_SIZE = 1.5f;

PoolTeeBox::PoolTeeBox()
{
  PSolidComponent pSolid = SolidComponent::LoadSolid("null.comp");
  SetSolid(pSolid);

  m_isTranslucent = true;
}

const char* PoolTeeBox::GetTypeName() const
{
  return "golf_tee";
}

void PoolTeeBox::DrawShadow() const
{
  // POOL: do nothing
}

void PoolTeeBox::Draw()
{
  // POOL: do nothing
}

void PoolTeeBox::CreateShadow()
{
/* // POOL - not reqd

  std::string bitmapname = Engine::Instance()->GetConfigValue("golf_tee_bm");
  std::string alphaname = Engine::Instance()->GetConfigValue("golf_tee_alpha");

  Texture* pTex = TextureServer::Instance()->Get(bitmapname, alphaname);
  if (pTex)
  {
    pTex->CreateBinding(Texture::REGULAR);
  }

  DecalShadow* pShadow = new DecalShadow;
  pShadow->SetTexture(pTex);
  m_pShadow = pShadow;
*/
}

void PoolTeeBox::SetOrientation(const Orientation& o)
{
  m_teeVertex = o.GetVertex();
  SolidGameObject::SetOrientation(o);
}

const BoundingSphere* PoolTeeBox::GetBoundingSphere() const
{
  return &m_bs;
}

void PoolTeeBox::CreateBoundingSphere(const Matrix& cm)
{
  m_bs.SetCentre(m_teeVertex);
  m_bs.SetRadius(3.0f); 

}

bool PoolTeeBox::Load(File* pf)
{
  // Get vertex.
  if (!pf->GetFloat(&m_teeVertex.x)  ||
      !pf->GetFloat(&m_teeVertex.y)  ||
      !pf->GetFloat(&m_teeVertex.z))
  {
    pf->ReportError("Failed to load tee box vertex.");
    return false;
  }

  m_bs.SetCentre(m_teeVertex);
  m_bs.SetRadius(TEE_BOX_DECAL_SIZE); 

  SetShadowSize(TEE_BOX_DECAL_SIZE); // TODO TEMP TEST
  CreateShadow();

  Orientation o;
  o.SetVertex(m_teeVertex);
  SetOrientation(o);

  TEE_BOX_DECAL_SIZE = Engine::Instance()->GetConfigFloat("golf_tee_size");

  return true;
}

#if defined(SCENE_EDITOR)
bool PoolTeeBox::Save(File* pf)
{
  pf->WriteComment("// Tee box position");
  pf->WriteFloat(m_teeVertex.x);
  pf->WriteFloat(m_teeVertex.y);
  pf->WriteFloat(m_teeVertex.z);

  return true;
}

GameObject::PropertyMap PoolTeeBox::GetProperties() const
{
  // TODO
  PropertyMap pm;
  return pm;
}

void PoolTeeBox::SetProperties(const GameObject::PropertyMap& pm)
{
  // TODO
}
#endif
}


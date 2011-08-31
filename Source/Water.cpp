/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Water.cpp,v $
Revision 1.1  2004/09/08 15:43:00  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Water.h"
#include "Common.h"
#include "CharacterGameObject.h"
#include "File.h"
#include "Level.h"
#include "Engine.h"
#include "TextureServer.h"
#include "Message.h"
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;
namespace Amju
{
float Water::Ripple::s_expandVel = 1.0f; 
float Water::Ripple::s_maxsize  = 5.0f;
Texture* Water::Ripple::s_pTexture;

const float OFFSET = 0.01f;

Water::Water()
{
  //m_height = 0;

#if defined(SCENE_EDITOR)
  // TODO default constructed Game Object must be saveable for Editor.
/*
  string waterName = Engine::Instance()->GetConfigValue("default_water_comp");
  if (waterName.empty())
  {
    waterName = "water1.leaf";
  }
  m_pWaterLeaf = SolidComponent::LoadSolid(waterName);
*/
  std::string waterBmp = Engine::Instance()->GetConfigValue("water_bmp");
  std::string waterBmpA = Engine::Instance()->GetConfigValue("water_bmp_a");
  m_pWaterTexture = TextureServer::Instance()->Get("wh8.bmp", "");
  m_pWaterTexture->CreateBinding(Texture::REGULAR);
  m_box.Set(1.0f, 0, 1.0f, 0, 1.0f, 0);
#endif
}

void Water::Draw()
{
/*
  //Assert(m_pWaterLeaf.GetPtr());

  //AmjuGL::PushMatrix();
  // Translate to the height of the water surface.
  //AmjuGL::Translate(0, m_height, 0);
  //m_pWaterLeaf->Draw();
  //AmjuGL::PopMatrix();

  if (m_box.m_ymax <= m_box.m_ymin)
  {
    return;
  }

  Engine::Instance()->PushColour(0, 0, 1.0f, 0.2f);
  m_pWaterTexture->Bind();
  // Draw a quad facing up and one facing down for the top of the water.
  SceneVertex a(m_box.m_xmax, m_box.m_ymax, m_box.m_zmax);
  a.SetNormal(0, 1.0f, 0);

  SceneVertex b(m_box.m_xmax, m_box.m_ymax, m_box.m_zmin);
  b.SetNormal(0, 1.0f, 0);

  SceneVertex c(m_box.m_xmin, m_box.m_ymax, m_box.m_zmin);
  c.SetNormal(0, 1.0f, 0);

  SceneVertex d(m_box.m_xmin, m_box.m_ymax, m_box.m_zmax);
  d.SetNormal(0, 1.0f, 0);

  SceneVertex e(m_box.m_xmax, m_box.m_ymin, m_box.m_zmax);
  e.SetNormal(0, 1.0f, 0);

  SceneVertex f(m_box.m_xmax, m_box.m_ymin, m_box.m_zmin);
  f.SetNormal(0, 1.0f, 0);

  SceneVertex g(m_box.m_xmin, m_box.m_ymin, m_box.m_zmin);
  g.SetNormal(0, 1.0f, 0);

  SceneVertex h(m_box.m_xmin, m_box.m_ymin, m_box.m_zmax);
  h.SetNormal(0, 1.0f, 0);

  glBegin(GL_QUADS);
    TexVertex(0, 0).DrawVertex();
    a.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    b.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    c.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    d.DrawVertex();

    TexVertex(0, 0).DrawVertex();
    d.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    c.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    b.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    a.DrawVertex();

    // Draw bottom of water.. TODO may not be necessary
    TexVertex(0, 0).DrawVertex();
    e.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    f.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    g.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    h.DrawVertex();

    TexVertex(0, 0).DrawVertex();
    h.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    g.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    f.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    e.DrawVertex();

  glEnd();

  Engine::Instance()->PopColour();

  // Draw any ripples.
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  //Ripple::SetHeight(m_box.m_ymax);
  float dt = Engine::Instance()->GetDeltaTime();
  for (RippleVec::iterator it = m_ripples.begin(); it != m_ripples.end(); ++it)
  {
    it->Draw(dt);
  }

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);

  // TODO Draw any splashes.
*/
}

void Water::Ripple::Draw(float deltaTime)
{
/*
  if (!IsActive())
  {
    return;
  }

  // Expand the ripple
  m_size += deltaTime * s_expandVel;
  if (m_size > s_maxsize)
  {
    Reset();
  }

  Assert(s_pTexture);
  s_pTexture->Bind();

  // Ripple becomes more translucent as it expands, until it's transparent.
  float alpha = 1.0f - (m_size / s_maxsize);
  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, alpha);

  // Make a Quad on the fly, centred around m_x, m_z. The normal points upwards.
  float y = m_y + OFFSET;

  SceneVertex a(m_x + m_size, y, m_z + m_size);
  a.SetNormal(0, 1.0f, 0);

  SceneVertex b(m_x + m_size, y, m_z - m_size);
  b.SetNormal(0, 1.0f, 0);

  SceneVertex c(m_x - m_size, y, m_z - m_size);
  c.SetNormal(0, 1.0f, 0);

  SceneVertex d(m_x - m_size, y, m_z + m_size);
  d.SetNormal(0, 1.0f, 0);

  // Draw the ripple twice, so we can see if from under the water too.
  glBegin(GL_QUADS);
    TexVertex(0, 0).DrawVertex();
    a.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    b.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    c.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    d.DrawVertex();

    TexVertex(0, 0).DrawVertex();
    d.DrawVertex();
    TexVertex(1, 0).DrawVertex();
    c.DrawVertex();
    TexVertex(1, 1).DrawVertex();
    b.DrawVertex();
    TexVertex(0, 1).DrawVertex();
    a.DrawVertex();
  glEnd();

  // Restore translucency.
  Engine::Instance()->PopColour();
*/
}

const BoundingSphere* Water::GetBoundingSphere() const
{
  //Assert(m_pWaterLeaf.GetPtr());
  return &m_sphere;
}

void Water::CreateBoundingSphere(const Matrix& cm)
{
  //Assert(m_pWaterLeaf.GetPtr());
  //m_pWaterLeaf->CreateBoundingSphere(cm);
  m_sphere = m_box.GetBoundingSphere();
}

#ifdef SCENE_EDITOR
bool Water::Save(File* pf)
{
  //pf->WriteComment("// Water: solid name");
  //pf->Write(StripPath(m_pWaterLeaf->GetName()));
  //pf->WriteComment("// Water height");
  //pf->WriteFloat(m_height);
  // Save bounding box
  pf->WriteComment("// Bounding box");
  pf->WriteFloat(m_box.m_xmin);
  pf->WriteFloat(m_box.m_xmax);
  pf->WriteFloat(m_box.m_ymin);
  pf->WriteFloat(m_box.m_ymax);
  pf->WriteFloat(m_box.m_zmin);
  pf->WriteFloat(m_box.m_zmax);

  return true; 
}
#endif

#if defined(SCENE_EDITOR)
static const std::string WATER_SOLID = "water solid";
static const std::string WATER_HEIGHT = "height";
static const std::string XMIN = "xmin";
static const std::string XMAX = "xmax";
static const std::string YMIN = "ymin";
static const std::string YMAX = "ymax";
static const std::string ZMIN = "zmin";
static const std::string ZMAX = "zmax";

GameObject::PropertyMap Water::GetProperties() const
{
  PropertyMap pm;
//  pm[WATER_SOLID] = StripPath(m_pWaterLeaf->GetName());
  //pm[WATER_HEIGHT] = m_height;
  pm[XMIN] = m_box.m_xmin;
  pm[XMAX] = m_box.m_xmax;
  pm[YMIN] = m_box.m_ymin;
  pm[YMAX] = m_box.m_ymax;
  pm[ZMIN] = m_box.m_zmin;
  pm[ZMAX] = m_box.m_zmax;
  return pm;
}

void Water::SetProperties(const GameObject::PropertyMap& pm)
{
  //m_height = const_cast<GameObject::PropertyMap&>(pm)[WATER_HEIGHT].GetFloat();
 /* 
  string solidName = const_cast<GameObject::PropertyMap&>(pm)[WATER_SOLID].GetString();
  PSolidComponent pSolid = SolidComponent::LoadSolid(solidName);
  if (pSolid.GetPtr())
  {
    m_pWaterLeaf = pSolid;
  }
*/
  m_box.m_xmin = const_cast<GameObject::PropertyMap&>(pm)[XMIN].GetFloat();
  m_box.m_xmax = const_cast<GameObject::PropertyMap&>(pm)[XMAX].GetFloat();
  m_box.m_ymin = const_cast<GameObject::PropertyMap&>(pm)[YMIN].GetFloat();
  m_box.m_ymax = const_cast<GameObject::PropertyMap&>(pm)[YMAX].GetFloat();
  m_box.m_zmin = const_cast<GameObject::PropertyMap&>(pm)[ZMIN].GetFloat();
  m_box.m_zmax = const_cast<GameObject::PropertyMap&>(pm)[ZMAX].GetFloat();
}
#endif

const char* Water::GetTypeName() const
{
  return "water";
}

bool Water::Load(File* pf)
{
  // Get solid leaf name. This is expected to be a Leaf, decorated with a
  // moving texture.
  // The leaf should be a double-sided horizontal plane poly, at height y=0.
  /*
  string solidname;
  if (!pf->GetDataLine(&solidname))
  {
    pf->ReportError("Expected water leaf name.");
    return false;
  }
  m_pWaterLeaf = SolidComponent::LoadSolid(solidname);
  if (!m_pWaterLeaf.GetPtr())
  {
    pf->ReportError("Bad water leaf name.");
    return false;
  }
  */
  /*
  if (!pf->GetFloat(&m_height))
  {
    pf->ReportError("Expected water height.");
    return false;
  }
  */
  // Bounding box
  if (!pf->GetFloat(&m_box.m_xmin) ||
      !pf->GetFloat(&m_box.m_xmax) ||
      !pf->GetFloat(&m_box.m_ymin) ||
      !pf->GetFloat(&m_box.m_ymax) ||
      !pf->GetFloat(&m_box.m_zmin) ||
      !pf->GetFloat(&m_box.m_zmax) )
  {
    pf->ReportError("Failed reading water bounding box.");
    return false;
  }
  m_height = m_box.m_ymax;

  std::string waterBmp = Engine::Instance()->GetConfigValue("water_bmp");
  std::string waterBmpA = Engine::Instance()->GetConfigValue("water_bmp_a");
  m_pWaterTexture = TextureServer::Instance()->Get("wh8.bmp", "");
  if (!m_pWaterTexture)
  {
    pf->ReportError("Failed to load water texture.");
    return false;
  }

  m_pWaterTexture->CreateBinding(Texture::REGULAR);

  // Load static stuff from config file
  // Get the max number of ripples
  int numRipples = atoi(Engine::Instance()->GetConfigValue("water_num_ripples").c_str());

  // Create numRipples elements
  m_ripples = RippleVec(numRipples);

  // Get the max ripple size
  float maxripplesize = atof(Engine::Instance()->GetConfigValue("water_max_rip_size").c_str());
  // Get expand velocity
  float rippleexpandvel = atof(Engine::Instance()->GetConfigValue("water_exp_vel").c_str());
  // Get the ripple texture and alpha
  string rippletex = Engine::Instance()->GetConfigValue("water_rip_tex");
  string ripplealpha = Engine::Instance()->GetConfigValue("water_rip_alpha");
  Texture* pRippleTexture = TextureServer::Instance()->Get(rippletex, ripplealpha);
  if (!pRippleTexture)
  {
    pf->ReportError("Failed to get ripple texture.");
    return false;
  }
  pRippleTexture->CreateBinding(Texture::REGULAR);

  Ripple::SetTexture(pRippleTexture);
  Ripple::SetMaxSize(maxripplesize);
  Ripple::SetExpandVel(rippleexpandvel);

  // TODO Splashes

  return true;
}

void Water::Update()
{
}

void Water::OnRoomEntry()
{
/*
  if (m_pLevel)
  {
    m_pLevel->SetCurrentRoomWaterHeight(GetHeight());
  }
*/
  // Reset all ripple stuff.
  m_rippleSet.clear();
  for (RippleVec::iterator it = m_ripples.begin(); it != m_ripples.end(); ++it)
  {
    it->Reset();
  }  
}

void Water::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  // If we are underwater, decrease player health.
  // TODO work out how do do this with timed messages.
  // Maybe start up a Power Timer ?
  // Player could float or walk along the floor, this depends on player state.

  // Do ripples - same as for other objects.
  HandleObjectCollision(pPlayer);
}

bool Water::IsRippling(int objectId)
{
  bool b = m_rippleSet.find(objectId) != m_rippleSet.end();
  return b;
}

bool Water::CreateRipple(float x, float y, float z)
{
  // Find an inactive ripple, which we then activate.
  for (RippleVec::iterator it = m_ripples.begin(); it != m_ripples.end(); ++it)
  {
    if (!it->IsActive())
    {
      // Found an inactive one
      it->SetHeight(y);
      it->Reset(x, z);
      it->Activate();
      return true;
    }
  }
  return false;
}

void Water::HandleObjectCollision(GameObject* pObj)
{
  FreeMovingGameObject* pF = dynamic_cast<FreeMovingGameObject*>(pObj);
  if (!pObj)
  {
    return;
  }

  // Object must be in box, not just the bounding sphere.
  if (!m_box.Intersects(*pObj->GetBoundingSphere()))
  {
    if (pF && pF->GetWater() == this)
    {
        pF->SetWater(0);
    }
    return;
  }

  if (pF && pF->GetWater() == this)
  {
    return;
  }

  if (pF)
  {
    pF->SetWater(this);
  } 

  // Play water splash wav - should be per-object type.
  // TODO
  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("wav_splash")); 

  // If the object wasn't in the water before, we should draw a splash.
  // The size of the splash depends on the velocity of the object.

  // Ripples
  // -------
  // We hold a container of objects which we just draw a ripple around.
  // We send a timed message to clear the object later.
  if (IsRippling(pObj->GetId()))
  {
    // We recently created a new ripple around the object.
    return;
  }

  // If object's bounding sphere intersects water plane, draw ripples.
  const float y = pObj->GetBoundingSphere()->GetCentre().y;
  const float r = pObj->GetBoundingSphere()->GetRadius();
  float maxy = y + r;
  //float miny = y - r;

  // NB Only test for object lower than water line.
  // Otherwise a fast object may not cause a ripple.
  if (m_height < maxy) //&& m_height > miny)
  {
    // Draw a ripple - set Rippled flag
    if (CreateRipple(pObj->GetBoundingSphere()->GetCentre().x, m_height, 
        pObj->GetBoundingSphere()->GetCentre().z))
    {
      m_rippleSet.insert(pObj->GetId());
      // Send a timed message to clear the rippled flag later.
      // TODO Just a test. Real message should contain the ID to remove from the set.
      PMessage m = new Message(
        "ripple wait", 2.0f, GetId(), GetId(), 0); //RIPPLE_WAIT);
      Engine::Instance()->SendGameMessage(m);
    }
  }
}

void Water::ReceiveMessage(const Message& m)
{
  m_rippleSet.clear(); // TODO Total hack
}

void Water::SetOrientation(const Orientation& o)
{
  SetHeight(o.GetY());
  m_box.Translate(o.GetX(), o.GetY(), o.GetZ());
  m_sphere = m_box.GetBoundingSphere();
}


void Water::SetHeight(float y)
{
  //m_height = y;
/*
  if (m_pLevel)
  {
    m_pLevel->SetCurrentRoomWaterHeight(GetHeight());
  }
*/
  m_box.m_ymax = y;
  for (RippleVec::iterator it = m_ripples.begin(); it != m_ripples.end(); ++it)
  {
    it->SetHeight(y);
  }
}

float Water::GetHeight() const
{
  return m_box.m_ymax;
}

void Water::RefreshHeightServer()
{
}

bool Water::AddToHeightServer(const HeightServer& )
{
  return true;
}

void Water::DrawShadow() const
{
}

}


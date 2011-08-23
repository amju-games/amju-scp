/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Egg.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Egg.h"
#include "File.h"
#include "Engine.h"
#include "SolidComposite.h"
#include "Orientation.h"
#include "GameState.h"
#include "SolidComponent.h"
#include "EngineRunning.h"
#include "CharacterManager.h"
#include "Level.h"
#include "GameObjectFactory.h"
#include "Frustum.h"
#include "SchAssert.h"
#include "Controller.h"

using namespace std;

namespace Amju
{
float Egg::s_maxRotate;
float Egg::s_rotateDecel;
/*
// Register creation function with Game Object Factory
GameObject* CreateEgg() { return new Egg; }
static const bool isEggRegistered = GameObjectFactory::Instance()->
  RegisterFactoryFunction("egg", CreateEgg);
*/

Egg::Egg()
{
#if defined(SCENE_EDITOR)
  // If created in editor, m_pSolid won't be set. But we can get it from the config file.
  string eggFilename = Engine::Instance()->GetConfigValue("solid_egg");
  m_pSolid = SolidComponent::LoadSolid(eggFilename);
  // Initial default chick ID. There has to be one, or client won't know 
  // what type to put in the list.
  m_chickIds.push_back(999);
#endif
}

void Egg::Hatch()
{
  GameState* pGs = Engine::Instance()->GetGameState();

  // Create Chick(s) which drops out of the egg centre.
  // Create a new chick at the given coords, in the current room.
  Assert(m_pSolid.GetPtr());
  Assert(m_pSolid->GetOrientation());

  const Orientation& o = *(m_pSolid->GetOrientation());
  string chickName = Engine::Instance()->GetConfigValue("chick");

  // More than one chick can hatch out of the egg. 
  for (int i = 0; i < m_chickIds.size(); i++)
  {
    Chick* chick = new Chick;
    int chickId = m_chickIds[i];
    
    // Give this Chick game object an ID. 
    chick->SetId(chickId);
    
    // Tell the chick which egg it hatched from 
    chick->SetEgg(GetId());
    // Set room and Level for the chick
    chick->SetLevel(GetLevel());
    chick->SetRoomId(GetRoomId());
    
    // Chick starts life at Egg orientation.
    chick->SetOrientation(o);

    // Create a new Chick character
    Character* pChickCharacter = CharacterManager::Instance()->GetCharacter(chickName);
    chick->AddMesh(pChickCharacter);

    // Add the new Chick to the list of Game Objects maintained by the Engine.
    Engine::Instance()->HoldGameObject(GetLevel()->GetId(), GetRoomId(), chick);

    chick->SetState(JUST_HATCHED); 

/*
    // Face a random direction, and move, rather than dropping straight down.
    float yDegs = (float)(rand() % 360);
    float hatchFwdVel = atof(Engine::Instance()->
      GetConfigValue("chick_hatch_fwd_vel").c_str());
    chick->GetOrientation()->SetYRot(yDegs);
    chick->SetForwardVel(hatchFwdVel);
    // Throw chick upwards 
    chick->SetDownwardVel(-2.0f); // TODO CONFIG
*/

    // Set chick state to UNKNOWN after a short interval, so it can be snatched.
    Engine::Instance()->SendGameMessage(
      new Message("chick takeable", 1.5f, // TODO CONFIG 
      GetId(), chickId, UNKNOWN));
 
  } // for each chick

/*
  // Get points for hatching egg    
  Engine::Instance()->GetGameState()->AddScore(100); // TODO CONFIG
*/

  // Set state to "Show number", and a timed message to set the state to
  // Out of Play after showing the number.
  //pGs->SetObjectState(m_id, State(EGG_SHOW_NUM));
  SetState(EGG_SHOW_NUM);
  float numTime = atof(Engine::Instance()->GetConfigValue("egg_num_time").c_str());
  if (numTime == 0)
  {
    numTime = 3.0f;
  }

  Engine::Instance()->SendGameMessage(new Message("egg numbers", numTime, 
    GetId(), GetId(), OUT_OF_PLAY));

  // Increment egg counter.
//  int eggs = pGs->GetNumEggs();
//  eggs++;
//  pGs->SetNumEggs(eggs);

  // particle effect
  m_explosion.Reset(GetBoundingSphere()->GetCentre());

  Engine::Instance()->PlayWav(Engine::Instance()->GetConfigValue("egg_wav"));
 
  // Spinning number
  m_rotate = 0;
  m_speed = s_maxRotate;
  // Number of eggs hatched. TODO use this to show any bonus score etc ?
  //m_number.SetNumber(eggs);
}

#if defined(SCENE_EDITOR)
bool Egg::Save(File* pf)
{
  Assert(m_pSolid.GetPtr());

  pf->WriteComment("// Egg solid name from config file.");
  pf->WriteComment("// Egg orientation");
  m_pSolid->GetOrientation()->Save(pf);

  pf->WriteComment("// Number of chicks");
  pf->WriteInteger(m_chickIds.size());
  for (int i = 0; i < m_chickIds.size(); i++)
  {
    pf->WriteComment("// Egg chick ID");
    pf->WriteInteger(m_chickIds[i]);
  }

  pf->WriteComment("// Egg regenerate flag,  (1 or 2, 1 -> regenerate)");
  pf->WriteInteger(m_regenerate ? 1 : 2);

  return true; 
}

static const std::string CHICK_IDS = "Chick IDs";
static const std::string REGENERATE = "Regenerate";

GameObject::PropertyMap Egg::GetProperties() const
{
  // TODO
  PropertyMap pm;

  // Create list of chick IDs
  VariableVec chickIds;
  for (int i = 0; i < m_chickIds.size(); i++)
  {
    chickIds.push_back(m_chickIds[i]);
  }
  pm[CHICK_IDS] = chickIds;
 
  pm[REGENERATE] = m_regenerate;  

  return pm;
}

void Egg::SetProperties(const GameObject::PropertyMap& pm)
{
  m_chickIds.clear();
  const Property p = const_cast<GameObject::PropertyMap&>(pm)[CHICK_IDS];
  VariableVec chickIds = p.GetVector();
  for (int i = 0; i < chickIds.size(); i++)
  {
    m_chickIds.push_back(chickIds[i].GetInt());
  }

  m_regenerate = (const_cast<GameObject::PropertyMap&>(pm)[REGENERATE]).GetBool();  
}
#endif

const char* Egg::GetTypeName() const
{
  return "egg";
}

bool Egg::Load(File* pf)
{
  string eggFilename = Engine::Instance()->GetConfigValue("solid_egg");

  m_pSolid = SolidComponent::LoadSolid(eggFilename);

  if (!m_pSolid.GetPtr())
  {
    return false;
  }

  Orientation o;
  if (!o.Load(pf))
  {
    pf->ReportError("Failed to load egg orientation.");
    return false;
  }
  SetOrientation(o);

  m_chickIds.clear();

  if (pf->GetVer() < 9)
  {
    int chickId = 0;
    // Get ID for chick which hatches out of this egg.
    if (!pf->GetInteger(&chickId))
    {
      pf->ReportError("Expected chick ID for egg.");
      return false;
    }
    m_chickIds.push_back(chickId);
  }
  else
  {
    // More than one chick may hatch out of the egg.
    int numChicks = 0;
    if (!pf->GetInteger(&numChicks))
    {
      pf->ReportError("Expected number of chicks for egg.");
      return false;
    }
    for (int i = 0; i < numChicks; i++)
    {
      int chickId = 0;
      if (!pf->GetInteger(&chickId))
      {
        pf->ReportError("Expected chick ID.");
        return false;
      }
      m_chickIds.push_back(chickId);
    }
  }

  SetState(UNKNOWN);

  // Regenerate flag: if chick is OUT_OF_PLAY, should this egg regenerate 
  // (i.e. be takeable again) ?
  int regenerate;
  if (!pf->GetInteger(&regenerate))
  {
    pf->ReportError("Expected regenerate flag.");
    return false;
  }
  // Only allow values of 1 or 2, helps find missing flags.
  if (regenerate != 1 && regenerate != 2) 
  {
    pf->ReportError("Expected regenerate flag (1 or 2, 1 -> regenerate)");
    return false;
  }
  m_regenerate = (regenerate == 1);

  // Other data, loaded from config
  // ------------------------------

  string solidName = Engine::Instance()->GetConfigValue("egg_particle");

  // Load the explosion effect
  PSolidComponent pBb = SolidComponent::LoadSolid(solidName);
  if (!pBb.GetPtr())
  {
    pf->ReportError("Bad particle name for egg.");
    return false;
  }

  // Load an Attrib Decorator to define the texture for the egg number.
  // TODO we could in theory have a different texture for each egg, but we 
  // stick to a global egg number texture for now.
  string numTextureFile = Engine::Instance()->GetConfigValue("egg_num_texture");
  if (!m_numTexture.Load(numTextureFile))
  {
    pf->ReportError("Failed to load number texture.");
    return false;
  }

  // TODO CONFIG num particles, max vel/time 
  float numParticles = atof(Engine::Instance()->GetConfigValue("egg_num_particles").c_str());
  if (numParticles == 0)
  {
    return false;
  }
  m_explosion.Init(numParticles, pBb, 10, 10);

  s_maxRotate = atof(Engine::Instance()->GetConfigValue("egg_num_max_speed").c_str());
  s_rotateDecel = atof(Engine::Instance()->GetConfigValue("egg_num_decel").c_str());

  return true;
}

void Egg::HandlePlayerCollision(CharacterGameObject* pPlayer)
{
  if (!pPlayer)
  {
    return;
  }
  if (GetState() != UNKNOWN)
  {
    return;
  }
  Hatch();
}

void Egg::Update()
{
  Assert(m_pSolid.GetPtr());
  m_pSolid->Update();  
}

void Egg::CullDraw(Frustum* pFrustum)
{
  if (pFrustum->Contains(*(GetBoundingSphere())))
  {
    Draw();
  }
  // Draw the explosion here so we still see the particles even if the egg
  // is not in the view frustum. 
  m_explosion.Draw();
}

void Egg::Draw()
{
  if (GetState() == EGG_SHOW_NUM)
  {
/*
    AmjuGL::PushMatrix();
    // Position the number where the egg used to be.
    m_pSolid->GetOrientation()->Draw();
    // Numbers don't have a texture, so we use a TextureMethod in an
    // Attribute Decorator to give the number a texture.
    m_numTexture.Draw();

    // TODO TEMP TEST
    // Can't seem to get the texture to env-map using attrib decorator :-(
    glTexGeni(GL_S, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_SPHERE_MAP); 
    glTexGeni(GL_T, AmjuGL::AMJU_TEXTURE_MATRIX_GEN_MODE, GL_SPHERE_MAP);

    // Spin number abount y-axis. 
    const float delta = Engine::Instance()->GetDeltaTime();
    m_rotate += delta * m_speed;
    m_speed -= delta * s_rotateDecel; 
    AmjuGL::Rotate(m_rotate, 0, 1, 0);

    m_number.Draw();
    AmjuGL::PopMatrix();
*/
  }
  else if (GetState() != OUT_OF_PLAY)
  {
    AmjuGL::PushMatrix();
    m_pSolid->GetOrientation()->Draw();
    m_pSolid->Draw();
    AmjuGL::PopMatrix();
  }
}

void Egg::DrawShadow() const
{
  if (GetState() == UNKNOWN)
  {
    SolidGameObject::DrawShadow();
  }
}

void Egg::SetState(State newState)
{
  if (newState == HATCH)
  {
    Hatch();
    return; // state is set in Hatch().
  }
  else if (newState == UNKNOWN)
  {
    // Get rid of any pending messages for this egg. 
    Engine::Instance()->ClearMessages(GetId());

    // Get rid of any existing Chick for this egg.
    // Now multiple chicks per egg are allowed, so don't do this.

    //Engine::Instance()->SendGameMessage(
    //  new Message("hide chick", 0, GetId(), m_chickId, OUT_OF_PLAY));
  }
  GameObject::SetState(newState);
}
}


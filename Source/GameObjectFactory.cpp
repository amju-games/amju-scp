/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameObjectFactory.cpp,v $
Revision 1.1.8.12  2006/08/14 17:37:30  jay
Rename "Pool"

Revision 1.1.8.11  2006/07/26 21:25:24  jay
Use Pool/, not Pool/

Revision 1.1.8.10  2005/09/23 20:02:17  jay
Add new pool items

Revision 1.1.8.9  2005/09/16 20:06:05  jay
Add Trick Shot Rules

Revision 1.1.8.8  2005/09/08 20:01:02  jay
Added PoolBlock

Revision 1.1.8.7  2005/08/31 17:29:24  jay
Add more types to POOL

Revision 1.1.8.6  2005/06/22 21:46:31  jay
Added RulesUs8Ball

Revision 1.1.8.5  2005/06/05 00:23:59  Administrator
Add SortScene to factory, so we can have trees in Pool.

Revision 1.1.8.4  2005/04/17 22:04:53  jay
Added NonHeadArea (POOL)

Revision 1.1.8.3  2005/03/31 22:24:19  jay
Moved pool ball/cue ball construction into PoolBall.o - so changes to the class
do not force us to recompile the factory.

Revision 1.1.8.2  2005/03/19 17:22:11  jay
Add rules for uk and n-ball pool

Revision 1.1.8.1  2005/03/17 20:50:22  jay
Add pool classes - remove everything else

Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "GameObjectFactory.h"
#include "GameObject.h"
#include "Engine.h"
// Hack to get around the strange fact that Factory<GameObject> 
// doesn't get all the objects registered.
// We wouldn't have to do this if GameObjects registered themselves,
// which they are supposed to do, but don't :-(
//#include "PoolBall.h"

//#include "Bonus.h"
//#include "Npc.h"
//#include "Platform.h"
#include "Shed.h"
//#include "Water.h"
#include "SortScene.h"
//#include "Target.h"
#include "Stupid.h"
#include "PoolHole.h"
#include "PoolTeeBox.h"
//#include "ChangeButton.h"
//#include "BonusBall.h"
#include "FreezeBonus.h"
//#include "BeanTrajectory.h"
//#include "BeanControl.h"
//#include "Waypoint.h"
//#include "Takeable.h"
//#include "Locator.h"
//#include "Portal.h"
//#include "Egg.h"
//#include "Bean.h"
//#include "BadThing.h"
//#include "BadPlatform.h"
//#include "Croc.h"
//#include "CrocHome.h"
//#include "Button.h"
//#include "GameObjectWatcher.h"
//#include "GameObjectSoundSource.h"
//#include "Bomb.h"
//#include "TakeableCrate.h"
//#include "LaunchPad.h"
//#include "PortalDestination.h"
//#include "GameObjectFire.h"
#include "PoolBonus.h"
#include "PoolExplBall.h"
#include "PoolPuck.h"

using namespace std;

namespace Amju
{
extern GameObject* CreateRulesUk();
extern GameObject* CreateRulesUs8Ball();
extern GameObject* CreateRulesNBall();
extern GameObject* CreatePoolBall();
extern GameObject* CreateCueBall();
extern GameObject* CreateNonHeadArea();
extern GameObject* CreatePoolBlock();
extern GameObject* CreateRulesTrickShot();
extern GameObject* CreatePoolExplBall();
extern GameObject* CreatePoolPuck();

GameObjectFactory* GameObjectFactory::Instance()
{
  static GameObjectFactory f;
  return &f;
}

GameObjectFactory::GameObjectFactory()
{
#if defined(SCENE_EDITOR)
  // Yuck, we should be calling a static Name function on each type.
  // And anyway, each type should register itself so we would have this
  // list without having to populate it ourselves. This sucks :-(
//  m_typenames.push_back("bonus");
//  m_typenames.push_back("locator");
//  m_typenames.push_back("portal");
//  m_typenames.push_back("egg");
//  m_typenames.push_back("bean");
//  m_typenames.push_back("platform");
//  m_typenames.push_back("badthing");
//  m_typenames.push_back("badplatform");
//  m_typenames.push_back("croc");
  m_typenames.push_back("shed");
//  m_typenames.push_back("crochome");
//  m_typenames.push_back("button");
//  m_typenames.push_back("watcher");
//  m_typenames.push_back("sound");
//  m_typenames.push_back("water");
//  m_typenames.push_back("bomb");
  m_typenames.push_back("sortscene");
//  m_typenames.push_back("crate");
//  m_typenames.push_back("target");
//  m_typenames.push_back("launchpad");
//  m_typenames.push_back("portaldest");
//  m_typenames.push_back("ramp");
//  m_typenames.push_back("ai");
  m_typenames.push_back("stupid");
  m_typenames.push_back("golf_hole");
  m_typenames.push_back("golf_tee");
//  m_typenames.push_back("change_button");
//  m_typenames.push_back("fire");
//  m_typenames.push_back("bonusball");
//  m_typenames.push_back("bean_trajectory");
//  m_typenames.push_back("bean_control");
  m_typenames.push_back("freezebonus");
//  m_typenames.push_back("waypoint");
  m_typenames.push_back("pool_ball");
  m_typenames.push_back("cue_ball");
  m_typenames.push_back("rules-uk");
  m_typenames.push_back("rules-n-ball");
  m_typenames.push_back("rules-us-8-ball");
  m_typenames.push_back("nonheadarea");
  m_typenames.push_back("pool-bonus");
  m_typenames.push_back("pool-block");
  m_typenames.push_back("rules-trick-shot");
  m_typenames.push_back("pool-expl-ball");
  m_typenames.push_back("pool-puck");
#endif
}

#if defined(SCENE_EDITOR)
const std::vector<std::string>& GameObjectFactory::GetTypeNames() const
{
  return m_typenames;
}
#endif

GameObject* GameObjectFactory::Create(const string& gameObjectName)
{
  if (gameObjectName == "freezebonus")
  {
    return new FreezeBonus;
  }
  if (gameObjectName == "stupid")
  {
    return new Stupid;
  }
  if (gameObjectName == "shed")
  {
    return new Shed;
  }
  if (gameObjectName == "pool_ball")
  {
    return CreatePoolBall();
  }
  if (gameObjectName == "cue_ball")
  {
    return CreateCueBall();
  }
  if (gameObjectName == "golf_hole")
  {
    return new PoolHole;
  }
  if (gameObjectName == "golf_tee")
  {
    return new PoolTeeBox;
  }
  if (gameObjectName == "rules-uk")
  {
    return CreateRulesUk(); 
  }
  if (gameObjectName == "rules-n-ball")
  {
    return CreateRulesNBall(); 
  }
  if (gameObjectName == "rules-us-8-ball")
  {
    return CreateRulesUs8Ball(); 
  }
  if (gameObjectName == "nonheadarea")
  {
    return CreateNonHeadArea(); 
  }
  if (gameObjectName == "sortscene")
  {
    return new SortScene; 
  }
  if (gameObjectName == "pool-bonus")
  {
    return new PoolBonus; 
  }
  if (gameObjectName == "pool-block")
  {
    return CreatePoolBlock(); 
  }
  if (gameObjectName == "rules-trick-shot")
  {
    return CreateRulesTrickShot(); 
  }
  if (gameObjectName == "pool-expl-ball")
  {
    return CreatePoolExplBall(); 
  }
  if (gameObjectName == "pool-puck")
  {
    return CreatePoolPuck(); 
  }
  return 0;
}
}


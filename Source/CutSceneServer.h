/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CutSceneServer.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(CUT_SCENE_SERVER_H_INCLUDED)
#define CUT_SCENE_SERVER_H_INCLUDED

#include "SpecialRecipient.h"
#include "ClipSequence.h"
#include <map>

namespace Amju
{
class Message;

// A message recipient that shows a Cut Scene when it gets a message
// to do so.
class CutSceneServer : public SpecialRecipient
{
public:
  CutSceneServer();

  // Override MessageRecipient fn.
  virtual void ReceiveMessage(const Message& m);

  // Add a scene to the player. This means that when a message is received
  // with the Scene ID as the Message Code, the associated Clip will be
  // played.
  //void AddCutScene(int sceneId, ClipSequence* pClip);

  PClipSequence GetScene(const std::string& sequenceFileName);

protected:
  typedef std::map<int, ClipSequence*> SceneMap;
  SceneMap m_scenes;
};
}
#endif



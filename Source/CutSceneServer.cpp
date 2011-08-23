/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CutSceneServer.cpp,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "CutSceneServer.h"
#include "ClipSequence.h"
#include "Message.h"
#include "File.h"
#include "Engine.h"
#include "EngineCutSceneState.h"
#include <iostream> // cout

using namespace std;
namespace Amju
{
CutSceneServer::CutSceneServer()
{
}

void CutSceneServer::ReceiveMessage(const Message& m)
{
  PClipSequence pSeq = GetScene(m.GetName());

  if (!pSeq.GetPtr())
  {
#if defined(_DEBUG)
    cout << "Failed to get cut scene, name " << m.GetName().c_str() << endl;
#endif
    return;
  }

  Engine::Instance()->SetLetterbox(true);

  EngineCutSceneState* pStateCutScene = 
    (EngineCutSceneState*)Engine::Instance()->GetEngineState(EngineCutSceneState::Name).GetPtr();
  pStateCutScene->SetSequence(pSeq); // TODO
  pSeq->Start();
  Engine::Instance()->ChangeState(EngineCutSceneState::Name, Engine::IMMEDIATE);
}

/*
void CutSceneServer::AddCutScene(int sceneId, ClipSequence* pClip)
{
}
*/

PClipSequence CutSceneServer::GetScene(const std::string& sceneFileName)
{
  // TODO!!!
  PClipSequence pSeq = new ClipSequence;
  File f;
  f.OpenRead(sceneFileName);
  if (!pSeq->Load(&f))
  {
    return 0;
  }
  return pSeq;
}
}

/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LevelFactorySimple.cpp,v $
Revision 1.1.10.2  2005/09/17 22:39:20  Administrator
Fixed Object Delete in Scene Editor

Revision 1.1.10.1  2005/07/30 12:08:36  jay
Code tidy-up

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <algorithm>
#include "LevelFactorySimple.h"
#include "File.h"
#include "LevelWorkerFactory.h"
#include "CubeMap.h"
#include "Engine.h"
#include "Player.h"
#include "CharacterManager.h"
#include "StringUtils.h"

namespace Amju
{
LevelFactorySimple::LevelFactorySimple()
{
  m_loadCounter = 0;
  m_numWorkers = 0;
}

LevelFactorySimple::~LevelFactorySimple()
{
}

void LevelFactorySimple::ClearWorkerForObject(int id)
{
  // Get the Worker from the id - erase the worker, then erase the id.
  SharedPtr<LevelWorker> pWorker = m_idToWorker[id];
  Assert(pWorker.GetPtr());
  DeleteWorker(pWorker);
  m_idToWorker.erase(id);
}

PLevel LevelFactorySimple::Create(int levelId)
{
  int playAreaSize = (int)Engine::Instance()->GetConfigFloat("play_area_size");

  // Keep looping until we create the whole level successfully.
  while (true)
  {
    PLevel pLevel = new Level;

    pLevel->SetId(levelId);

    // All Game Objects with IDs lower than this should be deleted.
    int uniqueId = LevelWorker::GetUniqueId();

    pLevel->SetPlayAreaSize(playAreaSize); 

    Orientation start(0, 1.0f, 0, 0, 0, 0);
    pLevel->SetStartOrientation(start);

    CubeMap cm;

    // Add a bounding box around the player start pos so there's a bit of space
    // around the player at the start.
    float s = 3.0f; // box size
    BoundingBox bb(
      start.GetX() - s, start.GetX() + s, 
      start.GetY() - s, start.GetY() + s,
      start.GetZ() - s, start.GetZ() + s);
    cm.Set(bb);

    LevelWorker::RndSeed(levelId);

    bool ok = true;
    for (Workers::iterator it = m_workers.begin(); it != m_workers.end(); ++it)
    {
      if (!(*it)->AddFeature(pLevel.GetPtr(), &cm))
      {
        ok = false;
      }
      if (!ok)
      {
        break; // don't bother with the other features
      }
    }

    if (ok)
    {
      return pLevel;
    }
    else
    {
      //Assert(0);
      // Delete any game objects we created.
      Engine::Instance()->ClearGameObjects();
    }
  }

  return 0;
}

LoadResult LevelFactorySimple::Load(File* pf)
{
  if (m_loadCounter == 0)
  {
    // Get number of workers
    if (!pf->GetInteger(&m_numWorkers))
    {
      pf->ReportError("Expected number of workers.");
      return false;
    }
  }
  
  //for (int i = 0; i < numWorkers; i++)
  int i = m_loadCounter;
  if (m_loadCounter == m_numWorkers)
  {
    return true; // finished loading.
  }
  ++m_loadCounter;
 
    // TODO Load each worker. Add to m_workers.
    std::string workerName;
    if (!pf->GetDataLine(&workerName))
    {
      pf->ReportError("Expected worker name.");
      return false;
    }

    SharedPtr<LevelWorker> pWorker = 
      LevelWorkerFactory::Instance()->Create(workerName);
    if (!pWorker.GetPtr())
    {
      std::string err = "Bad worker name: ";
      err += workerName; 
      pf->ReportError(err);
      return false;
    }
    if (!pWorker->Load(pf))
    {
      pf->ReportError("Failed loading worker.");
      return false;
    }
    pWorker->SetFactory(this);
    m_workers.push_back(pWorker);

  return LoadResult::MORE;
}

#ifdef SCENE_EDITOR
bool LevelFactorySimple::Save(File* pf)
{
  // Save the number of workers, then save each worker.
  int numWorkers = m_workers.size();
  if (!pf->WriteInteger(numWorkers))
  {
    return false;
  }
  for (int i = 0; i < numWorkers; i++)
  {
    std::string s = "Saving factory worker ";
    s += ToString(i);
    s += ": ";
    s += m_workers[i]->GetTypeName();
    pf->ReportError(s);

    // Save worker name so it can be created by factory in Load() above.
    pf->Write(m_workers[i]->GetTypeName());

    if (!m_workers[i]->Save(pf))
    {
      pf->ReportError("Couldn't save level factory worker.");
      return false;
    }
  }
  return true;
}
#endif

void LevelFactorySimple::AddWorker(SharedPtr<LevelWorker> pWorker)
{
  m_workers.push_back(pWorker);
  pWorker->SetFactory(this);
}

void LevelFactorySimple::DeleteWorker(SharedPtr<LevelWorker> pWorker)
{
  // Can't directly use remove_if because using SharedPtr, 
  // so easiest to just loop until we find the worker.
  for (Workers::iterator it = m_workers.begin(); it != m_workers.end(); ++it)
  {
    if (it->GetPtr() == pWorker.GetPtr())
    {
      m_workers.erase(it);
      return;
    }
  }
  Assert(0);
}

SharedPtr<LevelWorker> LevelFactorySimple::GetWorker(const char* name)
{
  // TODO use multimap for fast lookup
  for (Workers::iterator it = m_workers.begin(); it != m_workers.end(); ++it)
  {
    if ((*it)->GetTypeName() == name)
    {
      return *it;
    }  
  }
  Assert(0);
  return 0;
}

}


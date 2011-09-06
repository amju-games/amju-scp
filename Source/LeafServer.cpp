/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafServer.cpp,v $
Revision 1.1.10.1  2005/06/22 21:47:11  jay
Allow debug builds to use .leafb files (and so be glue files)

Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "LeafServer.h"

using namespace std;

namespace Amju
{
LeafServer* LeafServer::Instance()
{
  static LeafServer ls;
  return &ls;
}

LeafServer::LeafServer()
{
  m_storeDuplicates = false;
}

LeafServer::~LeafServer()
{
  Clear();
}

void LeafServer::Clear()
{
  for (LeafMap::iterator it = m_leafmap.begin();
       it != m_leafmap.end(); 
       ++it)
  {
    LeafData* pld = (*it).second;
    delete pld;
  }

  // Empty map
  m_leafmap.clear();
}

LeafData* LeafServer::Get(const std::string& leafname)
{
  LeafMap::iterator it = m_leafmap.find(leafname);

  if (it != m_leafmap.end())
    return (*it).second;

  // Load the leaf data, and add it to the map.
  // Open the file and get the leaf type. 

  File* pFile = 0;
  // Try to open a binary file first. If this fails, fall back to text file.
  File textFile;

// The idea here is that for dev builds you want to be able to edit leaf
// files without rebuilding the .leafb file.

#ifdef USE_LEAFB
  File binFile;
  std::string binfilename = leafname + std::string("b");
  if (binFile.OpenRead(binfilename, true))
  {
    pFile = &binFile;
  }
  else
#endif

  {
    if (!textFile.OpenRead(leafname))
    {
      textFile.ReportError("Couldn't open file.");
      return 0;
    }
    pFile = &textFile;
  }

  LeafData* pld = new LeafData(leafname);

  if (!pld->Load(pFile))
  {
    return 0;
  }

std::cout << "Loaded file ok: " << leafname << "\n";

  // Add the leaf data to the map of created leaf data objects.
  if (!StoreDuplicates())
  {
    m_leafmap.insert(LeafMap::value_type(leafname, pld));
  }

  return pld;
}

}


/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafServer.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEAF_SERVER_H_INCLUDED)
#define LEAF_SERVER_H_INCLUDED

#include "SolidComponent.h"
#include "LeafData.h"
#include "Polygon.h"
#include "Vertex.h"
#include <string>
#include <vector>
#include <map>

namespace Amju
{
typedef std::multimap<std::string, LeafData*> LeafMap;

// Serves up SolidLeaves. All requests should go through the same
// Leaf Server, which lets many Leaves point to the same data.
class LeafServer
{
public:
  enum LeafType { MultiTextured, SingleTextured, File3ds };
  
  static LeafServer* Instance();
  ~LeafServer();

  LeafData* Get(const std::string& leafname);

  void Clear();
 
  // Normally we store only one instance of each distinct leaf. We may store
  // duplicates for Scene Editing.
  void SetStoreDuplicates(bool b) { m_storeDuplicates = b; }
  bool StoreDuplicates() { return m_storeDuplicates; }

private:
  LeafServer();
  LeafMap m_leafmap;
  bool m_storeDuplicates;
};
}
#endif


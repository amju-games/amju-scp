/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Function.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(PLATFORM_FUNCTION_H_INCLUDED)
#define PLATFORM_FUNCTION_H_INCLUDED

#include <string>
#include "RCPtr.h"

namespace Amju
{
class Orientation;
class File;

// Abstract base class. Functions change an Orientation w.r.t. time.
class Function : public RefCounted
{
public:
  Function();
  virtual ~Function();

  virtual bool Load(File* pf) = 0;

  // Opens file and calls the above function
  bool Load(const std::string& filename);

  // Recalculate the Orientation. Subclasses should use the Engine
  // to get the deta time, and add this to m_time. 
  // TODO in fact this can be enforced.
  // This allows us to reset the Function to its start state, etc.
  virtual void Recalc(Orientation* p) = 0;

  virtual void SetTime(float secs) { m_time = secs; }

protected:
  // Elapsed time for the function.
  // Updated in Recalc(), and Set by SetTime.
  float m_time;


#if defined(SCENE_EDITOR)
public:
  virtual const char* GetTypeName() const = 0;

  virtual bool Save(File* pf) = 0;
  bool Save();

  void SetName(const std::string& name) { m_name = name; }
  std::string GetName() const { return m_name; }

protected:
  std::string m_name;
#endif
};

typedef RCPtr<Function> PFunction;
}
#endif


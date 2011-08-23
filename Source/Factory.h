/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Factory.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(SCHMICKEN_FACTORY_H_INCLUDED)
#define SCHMICKEN_FACTORY_H_INCLUDED

#include <map>
#include <string>

namespace Amju
{
// Creates objects of type T or subclass-of-T.
// Each class must be registered with the Factory, by calling
// RegisterFactoryFunction. The function that is registered
// should return a new object.
// This Factory design comes from "Modern C++ Design", but MSVC
// can't compile the Loki library, so this is a simplified version.
template <class T>
class Factory 
{
public:

  // Non-member function which creates a new T.
  typedef T* (*FactoryFunction)();

  // Called once for each subtype of T. Registers a function
  // which creates an object of that type.
  bool RegisterFactoryFunction(const std::string& name, FactoryFunction f)
  {
    m_map[name] = f;
    return true;
  }

  // Create an object of type T. The subtype of
  // the object depends on the string. This should match the ID
  // of a subtype which is registered.
  T* Create(const std::string& name)
  {
    FactoryFunction f = m_map[name];
    if (f)
    {
      return f();
    }
    return 0;
  }

protected:
  typedef std::map<std::string, FactoryFunction> FuncMap;
  FuncMap m_map;
};
}
#endif

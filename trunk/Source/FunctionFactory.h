/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FunctionFactory.h,v $
Revision 1.1  2004/09/08 15:42:50  jay
Added to repository
  
*/

#if !defined(PLATFORM_FUNCTION_LORD_H_INCLUDED)
#define PLATFORM_FUNCTION_LORD_H_INCLUDED

#include <string>
#include <vector>
#include "Function.h"
#include "Factory.h"

namespace Amju
{
/*
// For some reason I can't register creation functions in MSVC.
typedef Factory<Function> FunctionFactory;
*/

class FunctionFactory
{
public:
  static FunctionFactory* Instance();
  // Create a new platform function, whose exact type depends on the name
  // given. If the name is unrecognised, returns 0.
  Function* Create(const std::string& fnName);

private:
  FunctionFactory();
  FunctionFactory(const FunctionFactory&);
  FunctionFactory& operator=(const FunctionFactory&);


#if defined(SCENE_EDITOR)
public:
  // Get all strings which are valid for Create().
  // Used by editing tool.
  const std::vector<std::string>& GetTypeNames() const;

private:
  std::vector<std::string> m_typenames;
#endif
};

}
#endif


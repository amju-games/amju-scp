/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ConfigKeys.h,v $
Revision 1.1  2004/09/08 15:42:47  jay
Added to repository
  
*/

#if !defined(CONFIG_KEYS_H_INCLUDED)
#define CONFIG_KEYS_H_INCLUDED

#include <string>

namespace Amju
{
namespace Cfg
{
typedef std::string Key;

extern const Key SMALLEST_REVERSE_DIST;
extern const Key SMALLEST_DOT_PRODUCT;
}
}

#endif

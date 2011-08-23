/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LogFile.h,v $
Revision 1.1.10.1  2007/03/10 23:12:20  jay
Log file: make time optional, for diffing files

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(LOG_FILE_H_INCLUDED)
#define LOG_FILE_H_INCLUDED

#include <string>

namespace Amju
{
class LogFile
{
public:
  void SetName(const std::string& name);
  std::string GetName() const;
  void Log(const std::string& message, bool prependTime = true);

protected:
  std::string m_filename;
};
}

#endif


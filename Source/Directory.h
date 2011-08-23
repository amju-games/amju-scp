/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Directory.h,v $
Revision 1.1  2004/09/08 15:43:18  jay
Added to repository
  
*/

#if !defined(AMJU_DIRECTORY_H_INCLUDED)
#define AMJU_DIRECTORY_H_INCLUDED

#include <string>
#include <vector>

namespace Amju
{
// Lists all files in the given directory.
// If useGlue is true, and a Glue File is ready, the list of files in
// the glue file is returned. In this case the directory is not used.
// Returns true if successful.
bool Dir(
  const std::string& directory, 
  std::vector<std::string>* pResult, 
  bool useGlue = true);

// Convenience function which filters entries from the given vector of
// strings. 
// If include is true, only strings which contain the given 
// substring are retained.
// If include is false, strings which contain the given substring
// are filtered out.
void Filter(
  std::vector<std::string>* pResult, 
  const std::string& substr,
  bool include = true);

// Get directory for saving preferences, etc.
// This is in the file system, not the glue file.
// For Mac OS X, we use ~/Library/Application Support/<application name>.
// For Windows, we try to get something similar. If this fails (Win 98/NT)
// we use the File::Root directory. The reason is that users do not expect
// a new high-level  directory to be created on their disk, and don't like it.
std::string GetSaveDir();

// Get user desktop directory.
// This is for saving fles that we want the user to be able to
// find easily.
std::string GetDesktopDir();

// Make a directory in OS filesystem - glue file not used.
bool MkDir(const std::string& dir);

}

#endif


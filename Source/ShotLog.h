/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#ifndef SHOT_LOG_H
#define SHOT_LOG_H

#include "StringUtils.h"

namespace Amju
{
#if defined(POOL_ONLINE) && defined(WRITE_SHOT_LOG)
// Log shot info to a file, for debugging client physics inconsistencies, etc.
// The filename is baswed on the active player name.
void ShotLog(const std::string& s);

#else
#define ShotLog(s) // s
#endif
}

#endif

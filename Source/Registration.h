/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Registration.h,v $
Revision 1.1.4.2.2.11  2009/07/22 12:44:26  jay
Make ASCP freeware, so disable Registration code.

Revision 1.1.4.2.2.10  2007/12/14 11:11:25  jay
Update reg version for v.2.1

Revision 1.1.4.2.2.9  2007/08/04 10:38:02  Administrator
Update registration version

Revision 1.1.4.2.2.8  2007/03/23 09:30:11  jay
Update reg version

Revision 1.1.4.2.2.7  2006/04/11 17:09:36  jay
New REG_VERSION; remove BOONTY ifdef but keep 1 hour trial period for now

Revision 1.1.4.2.2.6  2006/03/24 22:57:14  jay
Trial Period is now internal to Registration.

Revision 1.1.4.2.2.5  2006/03/23 08:38:09  jay
Just added some version number comments

Revision 1.1.4.2.2.4  2005/10/28 12:54:36  jay
Set new version

Revision 1.1.4.2.2.3  2005/08/26 21:13:52  jay
Updated version no to Pool 1.2. GetStatus() is now available to check
 status of reg file.

Revision 1.1.4.2.2.2  2005/08/06 09:10:26  jay
Updated version number to v.1.1

Revision 1.1.4.2.2.1  2005/07/09 06:43:16  jay
Hide REG_VERSION so client code need not recompile on new version.

Revision 1.1.4.2  2004/11/10 10:13:28  jay
Updated version no. for this release

Revision 1.1.4.1  2004/09/30 09:45:11  jay
Update version no to ASG v.2.0

Revision 1.1  2004/09/08 15:43:19  jay
Added to repository
  
*/

#if !defined(AMJU_REGISTRATION_H_INCLUDED)
#define AMJU_REGISTRATION_H_INCLUDED

#include <string>
#include "Common.h"

namespace Amju
{
namespace Registration
{
// Current version of software, starting at 1.
// 0 is reserved, and means "registered".
// 1  - ASCP v.1.0
// 2  - ASCP v.1.1
// 3  - ASCP v.1.2
// 4  - ASCP v.1.3
// 5  - ASCP v.1.4
// 6  - ASCP v.1.5
// 7  - ASCP v.2.0
// 8  - ASCP v.2.1
// 9  - ASCP v.2.2 - NB now freeware!

static const int REG_VERSION = 9; 

// Returns the status code of the reg file.
// -1 means the file is no good
// 0 means registered
int GetStatus();

// Call to initialise. Creates registration file on first ever call.
// On subsequent calls, if the version number is greater than that
// in the file, the file is updated.
// This updates the file timestamp and so enables the eval period for
// the user.
bool Init();

// Returns true if SetRegistered(true) was ever called.
bool IsRegistered();

// Get the number of seconds left in evaluation period.
// NB the trial period is hard coded in the cpp file for now.
int GetRemainingEvalPeriod();

// Returns true if given registration key is valid.
bool IsRegKeyValid(const std::string& key);

// Call with true to register, false to unregister.
void SetRegistered(bool registered);

std::string MakeRegKey(); 

}
}

#endif


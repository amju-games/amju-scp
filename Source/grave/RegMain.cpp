/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: RegMain.cpp,v $
Revision 1.1  2004/09/08 15:43:09  jay
Added to repository
  
*/

// Reg - registration key test.
#include <iostream>
#include "../Utils/Registration.h"

using namespace std;
using namespace Amju;

namespace Amju
{
const char * APPLICATION_NAME = "Amju Super Golf";
void ReportError(const std::string& s)
{
//  std::cout << s.c_str() << "\n";
}
}

void Usage()
{
  std::cout << "Usage:\n"
               "reg -r <current version>   Set registered\n"
               "reg -u <current version>   Set unregistered\n"
               "reg -q <current version>   Query state\n";
}

int main(int argc, char** argv)
{
  if (argc < 3)
  {
    Usage();
    exit(0);
  }

  int v = atoi(argv[2]);
  Registration::Init(v);
  std::cout << "**Reg file has been initialised.\n";

  if (std::string(argv[1]) == "-r")
  {
    Registration::SetRegistered(true, v);
  }
  else if (std::string(argv[1]) == "-u")
  {
    Registration::SetRegistered(false, v);
  }
  else if (std::string(argv[1]) == "-q")
  {
    const int EVAL_PERIOD = 60 * 60 * 24 * 30; // 30 days 

    if (Registration::IsRegistered())
    {
      cout << "REGISTERED.\n";
    }
    else
    {
      cout << "Not registered.\n";
    }

    int rem = Registration::GetRemainingEvalPeriod(EVAL_PERIOD);
    cout << "Eval period ends in " << rem << " secs (" << rem/(60*60*24) << " days).\n";
  }

/*
  std::string s = Registration::MakeRegKey();
  cout << "Key: " << s.c_str() << endl;

  if (Registration::IsRegKeyValid(s))
  {
    cout << "Registration is valid.\n";
    Registration::SetRegistered(true);
  }

  if (Registration::IsRegistered())
  {
    cout << "REGISTERED.\n";
  }
  else
  {
    cout << "Not registered.\n";
  }
*/

  return 0;
}


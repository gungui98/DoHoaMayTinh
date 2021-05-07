#include <string>
#include <cstdlib>

using std::string;

#include <Windows.h>

namespace CGL {

string resolve_path( const char* filename ) {
  char resolved[512];
  GetFullPathName(filename, 512, resolved, NULL);
  return string(resolved);
}

}

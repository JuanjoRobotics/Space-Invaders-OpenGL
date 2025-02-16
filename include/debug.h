#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef ENABLE_DEBUG
#define DEBUG(msg) std::cout << "file: " << __FILE__ << " line: " << __LINE__ << " \"" << msg << "\"" << std::endl
#else
#define DEBUG(msg)
#endif

#endif // DEBUG_H
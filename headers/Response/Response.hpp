#ifndef __RESPONSE__
#define __RESPONSE__

#include <iostream>

class Response {
public:
  void response() const {
    std::cout << "Responding..." << std::endl;
  }
};

#endif

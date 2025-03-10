#ifndef __REQUEST__
#define __REQUEST__

#include <iostream>

class Request {
public:
  void request() const {
    std::cout << "Requesting..." << std::endl;
  }
};

#endif

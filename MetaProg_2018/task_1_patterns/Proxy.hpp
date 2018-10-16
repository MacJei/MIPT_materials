//
//  Proxy.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef Proxy_h
#define Proxy_h
#include "BClass.hpp"
#include <iostream>

class Proxy : public BInterface {
  private:
    B* b_instance_;
  public:
    Proxy(int id);
    void receive();
    ~Proxy();
};
#endif /* Proxy_h */

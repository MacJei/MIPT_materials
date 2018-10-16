//
//  Mediator.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef Mediator_h
#define Mediator_h
#include <vector>
#include <cstdlib>
#include "BClass.hpp"

class Mediator : public BInterface {
  private:
    std::vector<B*> bees_;
  public:
    Mediator();
    void receive();
    ~Mediator();
};

#endif /* Mediator_h */

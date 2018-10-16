//
//  Mediator.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "Mediator.hpp"

Mediator::Mediator() {
    //create few b instaces
    for(int i = 0; i < 10; ++i){
        B* b = new B(i);
        bees_.push_back(b);
    }
}
void Mediator::receive() {
    std::cout<< "Send method went throug mediator"<< std::endl;
    bees_[rand() % 10]->receive();
}

Mediator::~Mediator() {
    for(auto it = bees_.begin(); it != bees_.end(); ++it) {
        delete *it;
    }
    }

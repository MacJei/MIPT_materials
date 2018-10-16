//
//  Proxy.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "Proxy.hpp"

Proxy::Proxy(int id){
    b_instance_ = new B(id);
}
void Proxy::receive() {
    std::cout<< "Send method went through proxy"<< std::endl;
    b_instance_->receive();
}

Proxy::~Proxy() {
    delete b_instance_;
}

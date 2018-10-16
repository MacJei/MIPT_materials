//
//  CClass.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "CClass.hpp"
#include "BClass.hpp"
#include "Proxy.hpp"
#include "Mediator.hpp"
#include "Observer.hpp"
#include "AClass.hpp"

C<Proxy>::C(int id) {
    instance_ = new Proxy(id);
}

void C<Proxy>::receive() {
    instance_->receive();
}

C<Proxy>::~C() {
    delete instance_;
}

C<Mediator>::C() {
    instance_ = new Mediator();
}

void C<Mediator>::receive() {
    instance_->receive();
}

C<Mediator>::~C() {
    delete instance_;
}

C<Observer>::C() {
    instance_ = new Observer();
}

void C<Observer>::receive_notification(B* b) {
    instance_->receive_notification(b);
}

void C<Observer>::receive_subscription(A* a, B* b) {
    instance_->receive_subscription(a, b);
}




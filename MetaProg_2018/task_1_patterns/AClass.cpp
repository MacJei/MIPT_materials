//
//  AClass.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "AClass.hpp"

A::A() {
    data_ = 10;
}
void A::send(BInterface* b_instance) {
    b_instance->receive();
}

void A::send(C<Proxy>* c_instance) {
    c_instance->receive();
}

void A::send(C<Mediator>* c_instance) {
    c_instance->receive();
}

void A::subscribe(B* b_instance, Observer* observer) {
    observer->receive_subscription(this, b_instance);
}

void A::subscribe(B* b_instance, C<Observer>* c_instance) {
    c_instance->receive_subscription(this, b_instance);
}

void A::change_data() {
    data_ = 20;
}

int A::get_data() {
    return data_;
}


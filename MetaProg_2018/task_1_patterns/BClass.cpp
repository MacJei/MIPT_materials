//
//  BClass.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "BClass.hpp"
#include "CClass.hpp"
#include "Observer.hpp"

B::B(int id) {
    data_ = "I'm B class # " + std::to_string(id) + "\n";
}

void B::receive() {
    std::cout<< this->data_;
}

void B::notify(Observer* observer) {
    observer->receive_notification(this);
}

void B::notify(C<Observer>* c_instance) {
    c_instance->receive_notification(this);
}


void B::change_data_and_notify(Observer* observer) {
    data_ += "I was changed\n";
    this->notify(observer);
}

void B::change_data_and_notify(C<Observer>* c_instance) {
    data_ += "I was changed\n";
    this->notify(c_instance);
}

void B::change_data() {
    data_ += "I was changed\n";
}

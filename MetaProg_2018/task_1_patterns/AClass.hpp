//
//  AClass.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef AClass_h
#define AClass_h
#include "BClass.hpp"
#include "Observer.hpp"
#include "CClass.hpp"

class A {
  private:
    int data_;
  public:
    A();
    void send(BInterface* b_instance);
    void send(C<Proxy>* c_instance);
    void send(C<Mediator>* c_instance);
    void subscribe(B* b_instance, Observer* observer);
    void subscribe(B* b_instance, C<Observer>* c_instace);
    void change_data();
    int get_data();
};



#endif /* AClass_h */

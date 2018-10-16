//
//  BClass.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef BClass_h
#define BClass_h
#include <iostream>
#include <string>
#include <vector>
#include "CClass.hpp"
class Observer;

class BInterface {
  public:
    virtual void receive() = 0;
    virtual ~BInterface() {}
};

class B : public BInterface{
  private:
    std::string data_;

  public:
    B(int id);
    
    void receive();
    
    void notify(Observer* observer);
    
    void notify(C<Observer>* c_instance);
    
    void change_data_and_notify(Observer* observer);
    
    void change_data_and_notify(C<Observer>* c_instance);
    
    void change_data();
    
};


#endif /* BClass_h */

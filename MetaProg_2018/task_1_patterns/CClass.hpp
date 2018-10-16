//
//  CClass.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef CClass_hpp
#define CClass_hpp

#include <iostream>
class BInterface;
class Proxy;
class Mediator;
class Observer;
class B;
class A;

template <class T> class C {
public:
    C() {}
};

template <>
class C<Proxy> {
  private:
    Proxy* instance_;
  public:
    C(int id);
    void receive();
    ~C();
};

template <>
class C<Mediator> {
  private:
    Mediator* instance_;
  public:
    C();
    void receive();
    ~C();
};

template <>
class C<Observer> {
  private:
    Observer* instance_;
  public:
    C();
    void receive_subscription(A* a, B* b);
    void receive_notification(B* b);
};

#endif /* CClass_hpp */

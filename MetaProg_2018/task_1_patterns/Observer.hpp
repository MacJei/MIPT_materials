//
//  Observer.hpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#ifndef Observer_h
#define Observer_h
#include <iostream>
#include <map>
#include <vector>

class BInterface;
class A;
class B;

class Observer  {
  private:
    std::map<B*, std::vector<A*>> subscriptions_;
  public:
    Observer();
    void receive_notification(B* b_instance);
    void receive_subscription(A* a_instace, B* b_instace);
};

#endif /* Observer_h */

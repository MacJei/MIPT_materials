//
//  Observer.cpp
//  test2
//
//  Created by Valery Statinov on 16/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include "Observer.hpp"
#include "BClass.hpp"
#include "AClass.hpp"

Observer::Observer() {}

void Observer::receive_notification(B* b_instance) {
    auto it = subscriptions_.find(b_instance);
    if (it == subscriptions_.end()) {
        //not found
        return;
    }
    else {
        for (auto vector_it = it->second.begin(); vector_it != it->second.end(); ++vector_it) {
            (*vector_it)->change_data();
        }
    }
}

void Observer::receive_subscription(A* a_instace, B* b_instace) {
    auto it = subscriptions_.find(b_instace);
    if (it == subscriptions_.end()) {
        //not found
        std::vector<A*> v;
        v.push_back(a_instace);
        subscriptions_.insert(std::pair<B*, std::vector<A*>>(b_instace, v));
    }
    else {
        //already exists
        it->second.push_back(a_instace);
    }
    return;
}

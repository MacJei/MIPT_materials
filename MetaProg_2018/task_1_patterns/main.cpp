//
//  main.cpp
//  test2
//
//  Created by Valery Statinov on 15/10/2018.
//  Copyright © 2018 Valery Statinov. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <typeinfo>
#include <unistd.h>
#include "AClass.hpp"
#include "BClass.hpp"
#include "Proxy.hpp"
#include "Mediator.hpp"
#include "Observer.hpp"
#include "CClass.hpp"




int main() {
    A a = A();
    A a2 = A();
    B* b = new B(11);
    B* b2 = new B(22);
    
    //test proxy
    std::cout<< "TESTING PROXY\n";
    BInterface* pr = new Proxy(10);
    a.send(pr);
    std::cout<< std::endl;
    
    //test mediator
    std::cout<< "TESTING MEDIATOR\n";
    BInterface* med = new Mediator();
    a.send(med);
    std::cout<< std::endl;
    
    //test observer
    std::cout<< "TESTING OBSERVER\n";
    Observer* obs = new Observer();
    a.subscribe(b, obs);
    std::cout<< "Data in a before changes in b = " << a.get_data()<< std::endl;
    b->change_data_and_notify(obs);
    std::cout<< "Data in a after changes in b = " << a.get_data()<< std::endl;
    std::cout<< std::endl;
    
    //test C class
    std::cout<< "TESTING C AS PROXY\n";
    C<Proxy>* c = new C<Proxy>(123);
    a2.send(c);
    std::cout<< std::endl;
    
    std::cout<< "TESTING C AS MEDIATOR\n";
    C<Mediator>* c2 = new C<Mediator>();
    a2.send(c2);
    std::cout<< std::endl;
    
    std::cout<< "TESTING C AS OBSERVER\n";
    C<Observer>* c3 = new C<Observer>();
    a2.subscribe(b2, c3);
    std::cout<< "Data in a2 before changes in b2 = " << a2.get_data()<< std::endl;
    b2->change_data_and_notify(c3);
    std::cout<< "Data in a2 after changes in b2 = " << a2.get_data()<< std::endl;
    
    delete b;
    delete b2;
    delete pr;
    delete med;
    delete obs;
    delete c;
    delete c2;
    delete c3;
    
    return 0;
}





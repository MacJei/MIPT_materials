cmake_minimum_required(VERSION 2.8)	 # Проверка версии CMake.
set (CMAKE_CXX_STANDARD 11)
project(task1_patterns)
add_executable(main AClass.cpp BClass.cpp CClass.cpp Mediator.cpp 
Observer.cpp Proxy.cpp main.cpp)
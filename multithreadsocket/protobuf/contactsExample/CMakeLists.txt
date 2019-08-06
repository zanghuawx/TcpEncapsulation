cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CPP_COMPILER g++)
add_compile_options(-std=c++11)
project (Test)
add_executable(testProtoc contacts.pb.cc PersonStruct.cpp Contacts.cpp Test.cpp)
target_link_libraries(testProtoc pthread protobuf)



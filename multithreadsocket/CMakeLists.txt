cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CPP_COMPILER g++)
add_compile_options(-std=c++11)
INCLUDE_DIRECTORIES(./protobuf/contactsExample)
project (test)
add_executable(testClient Any.cpp Address.cpp Channel.cpp EventLoop.cpp EventLoopThread.cpp  EventLoopThreadPool.cpp Poller.cpp Timer.cpp Socket.cpp TcpClient.cpp TcpConnection.cpp FileOperator.cpp TcpServer.cpp ThreadPool.cpp TestClient.cpp)
target_link_libraries(testClient pthread protobuf)

add_executable(testServer Any.cpp Address.cpp Channel.cpp EventLoop.cpp EventLoopThread.cpp  EventLoopThreadPool.cpp Poller.cpp Socket.cpp TcpClient.cpp TcpConnection.cpp FileOperator.cpp TcpServer.cpp ThreadPool.cpp TestAll.cpp)
target_link_libraries(testServer pthread protobuf)

add_executable(TestFile Any.cpp Address.cpp Channel.cpp EventLoop.cpp EventLoopThread.cpp  EventLoopThreadPool.cpp Poller.cpp Socket.cpp TcpClient.cpp TcpConnection.cpp FileOperator.cpp TcpServer.cpp ThreadPool.cpp TestFile.cpp)
target_link_libraries(TestFile pthread protobuf)


add_executable(TestContactSerializeTransServer Any.cpp Address.cpp Channel.cpp EventLoop.cpp EventLoopThread.cpp  EventLoopThreadPool.cpp Poller.cpp Socket.cpp TcpClient.cpp TcpConnection.cpp FileOperator.cpp TcpServer.cpp ThreadPool.cpp ./protobuf/contactsExample/PersonStruct.cpp
 ./protobuf/contactsExample/contacts.pb.cc ./protobuf/contactsExample/Contacts.cpp  ./protobuf/contactsExample/ContactSerializeTransfer.cpp ./protobuf/contactsExample/TestContactSerializeTransServer.cpp )
target_link_libraries(TestContactSerializeTransServer pthread protobuf)


add_executable(TestContactSerializeTransClient Any.cpp Address.cpp Channel.cpp EventLoop.cpp EventLoopThread.cpp  EventLoopThreadPool.cpp Poller.cpp Socket.cpp TcpClient.cpp TcpConnection.cpp FileOperator.cpp TcpServer.cpp ThreadPool.cpp ./protobuf/contactsExample/PersonStruct.cpp
 ./protobuf/contactsExample/contacts.pb.cc ./protobuf/contactsExample/Contacts.cpp  ./protobuf/contactsExample/ContactSerializeTransfer.cpp ./protobuf/contactsExample/TestContactSerializeTransClient.cpp )
target_link_libraries(TestContactSerializeTransClient pthread protobuf)


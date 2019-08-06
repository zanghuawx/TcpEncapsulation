cmake_minimum_required (VERSION 2.8)
SET(CMAKE_CPP_COMPILER g++)
add_compile_options(-std=c++11)
project (Test)
add_executable(testProtocMysqlClient pet.pb.cc PetMysqlOperator.cpp PetProtocMessCodec.cpp  
				ProtocolMessageCodec.cpp RandomProductPet.cpp TestPetSerializeTransClient.cpp 
				../Any.cpp ../Address.cpp ../Channel.cpp ../EventLoop.cpp ../EventLoopThread.cpp  ../EventLoopThreadPool.cpp ../Poller.cpp 
				../Socket.cpp ../TcpClient.cpp ../TcpConnection.cpp ../FileOperator.cpp ../TcpServer.cpp ../ThreadPool.cpp ../Timer)

target_link_libraries(testProtocMysqlClient  pthread protobuf mysqlclient)


add_executable(testProtocMysqlServer pet.pb.cc PetMysqlOperator.cpp PetProtocMessCodec.cpp  
				ProtocolMessageCodec.cpp RandomProductPet.cpp TestPetSerializeTransServer.cpp 
				../Any.cpp ../Address.cpp ../Channel.cpp ../EventLoop.cpp ../EventLoopThread.cpp  ../EventLoopThreadPool.cpp ../Poller.cpp 
				../Socket.cpp ../TcpClient.cpp ../TcpConnection.cpp ../FileOperator.cpp ../TcpServer.cpp ../ThreadPool.cpp ../Timer)

target_link_libraries(testProtocMysqlServer  pthread protobuf mysqlclient)





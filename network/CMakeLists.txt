
INCLUDE_DIRECTORIES(../include)
ADD_LIBRARY(network MasterdTransport.cc  netSocket.cc  network.cc  Packet.cc  ServerAddress.cc ulError.cxx)

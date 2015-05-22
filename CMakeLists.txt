cmake_minimum_required(VERSION 2.3)

# if(CMAKE_COMPILER_IS_GNUCXX)
#  set( CMAKE_CXX_FLAGS "/GLOBAL_FLAGS_GO_HERE")
#  set( CMAKE_CXX_FLAGS_DEBUG "-Wall -g -O2")
#  set( CMAKE_CXX_FLAGS_RELEASE  "-Wall -O2")
# endif()
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -g -m32")

PROJECT(pbms)
OPTION(SERVERSTORE_RAM "Build using ServerStoreRAM" ON)

SUBDIRS(network masterd)


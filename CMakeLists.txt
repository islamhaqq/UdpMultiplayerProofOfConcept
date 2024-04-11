set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_TOOLCHAIN_FILE} CACHE STRING "")

cmake_minimum_required(VERSION 3.28)
project(SimpleUdpApp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost package
find_package(Boost REQUIRED COMPONENTS system thread)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Add executables for server and client
add_executable(server server.cpp)
add_executable(client client.cpp)

# Link Boost libraries
target_link_libraries(server ${Boost_LIBRARIES} Ws2_32)
target_link_libraries(client ${Boost_LIBRARIES} Ws2_32)

add_definitions(-D_WIN32_WINNT=0x0601)

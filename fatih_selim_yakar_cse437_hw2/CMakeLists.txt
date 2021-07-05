cmake_minimum_required(VERSION 3.10)
project(161044054_Fatih_Selim_Yakar VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

set(SOURCE_FILES main.cpp Timer.cpp TimerTask.cpp)
add_executable(run ${SOURCE_FILES})

find_package(Threads)
target_link_libraries(run Threads::Threads)

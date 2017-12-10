cmake_minimum_required(VERSION 2.8)

project(AutoBalsa)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++11 -fopenmp -ffast-math")

add_executable(
	BalsaTransform
	src/main.cpp
)

find_package(Boost REQUIRED COMPONENTS system)

target_link_libraries(
	BalsaTransform
	${Boost_SYSTEM_LIBRARY} 
)

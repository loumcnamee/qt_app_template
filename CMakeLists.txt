cmake_minimum_required(VERSION 3.14)

project(qt_template VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 COMPONENTS Charts REQUIRED)
add_subdirectory(aLib)
add_subdirectory(aTest)
add_subdirectory(theApp)


#add_executable(cpptest cpptest.cpp)


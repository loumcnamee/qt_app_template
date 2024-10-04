cmake_minimum_required(VERSION 3.14)

project(cmake_template VERSION 0.1.0 LANGUAGES C CXX)


add_subdirectory(aLib)
add_subdirectory(aTest)
add_subdirectory(theApp)


#add_executable(cpptest cpptest.cpp)


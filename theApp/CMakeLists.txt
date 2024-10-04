cmake_minimum_required(VERSION 3.14)

add_executable(theApp theApp.cpp)



# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# the application
target_link_libraries (theApp LINK_PUBLIC aLib)

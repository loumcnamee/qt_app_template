cmake_minimum_required(VERSION 3.8)

# Find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CMAKE_INCLUDE_PATH "/usr/include/x86_64-linux-gnu/qt6")
set (CMAKE_INCLUDE_PATH "/usr/lib/qt6")
#
# Turn on automatic invocation of the MOC, UIC & RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


# Add a compiler flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Make this a GUI application on Windows
if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

# Find the QtWidgets library
find_package(Qt6 COMPONENTS Core Gui Widgets Charts REQUIRED)

# Tell CMake to create the helloworld executable
add_executable(theApp theApp.cpp MainWindow.cpp)
target_link_libraries(theApp PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Charts)



# cpp_cmake_template
A template for C++ projects using CMAKE

This repository contains a template for a C++ application using CMake for the build system.
It includes support to build a static library containing classes, a google test project that test the library and an application that uses the library classes.

## Organization

### Folder structure
```
├ .vscode           # contains files to support integration with VS Code
├ aLib              # contains C++ classes built into the library aLib
├ aTest             # contains unit tests of classes in aLib
├ theApp            # main entry point using the classes in aLib
├ CMakeLists.txt    # main Cmake project file that references CMake projects in aLib, aTest, and theApp folders
```
## Important Files
* CMakeLists.txt    # this is the main CMake project file. Calling it with CMake will configure, build the entire project

## Toolchain

You will need the following tools to build this project
* an operating system to run CMake, the builder, compiler and linker
** options are
* CMake
* a build tool like Make, Ninja, NMake, etc
* a C++ compiler and linker
* (optional) an integrated development environment e.g. VS Code, CLion, CodeBlocks, MS Visual Studio


## Helpful References
 https://cmake.org/cmake/help/latest/
 https://code.visualstudio.com/docs/cpp/cmake-quickstart
 https://google.github.io/googletest/
 

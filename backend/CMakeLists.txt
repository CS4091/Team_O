cmake_minimum_required(VERSION 3.10)

project(CS4091backend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)

set(PYBIND11_FINDPYTHON ON)
find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(
    backend_binding
    src/map.cpp
    src/binding.cpp
    src/aircraft.cpp
    src/router1.cpp
)

target_include_directories(backend_binding PRIVATE src)
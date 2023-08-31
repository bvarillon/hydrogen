cmake_minimum_required(VERSION 3.1.0)
project(RayTracing VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
add_executable(hydro src/main.cpp)

install(TARGETS hydro
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)


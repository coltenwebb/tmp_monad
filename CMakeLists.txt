cmake_minimum_required(VERSION 3.15)
project(tmp_monad CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXXFLAGS} -march=skylake -O3")

find_package(benchmark)

add_executable(${PROJECT_NAME} src/main.cpp)

# Use the global target
target_link_libraries(${PROJECT_NAME} benchmark::benchmark)


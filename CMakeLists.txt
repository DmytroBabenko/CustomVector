cmake_minimum_required(VERSION 3.10)
project(CustomVector)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)

include_directories(
        include
        ${GTEST_INCLUDE_DIRS}
)

find_package( Threads )

add_executable(CustomVector tests/my_vector.test.cpp)
target_link_libraries(
        CustomVector ${GTEST_LIBRARIES} Threads::Threads)

enable_testing()
add_test(CustomVector "./my_vector.test")
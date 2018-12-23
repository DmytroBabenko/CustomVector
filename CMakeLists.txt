cmake_minimum_required(VERSION 3.13)
project(CustomVector)

set(CMAKE_CXX_STANDARD 14)

find_package(GTest REQUIRED)

include_directories(
        include
        ${GTEST_INCLUDE_DIRS}
)


add_executable(CustomVector tests/my_vector.test.cpp)
target_link_libraries(
        CustomVector ${GTEST_LIBRARIES} )

enable_testing()
add_test(CustomVector "./my_vector.test")
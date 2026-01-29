cmake_minimum_required(VERSION 3.21)

project(TravelTests VERSION 0.1 LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Test)

qt_standard_project_setup()

qt_add_executable(TravelTests
    tst_traveltests.cpp
)
add_test(NAME TravelTests COMMAND TravelTests)


set_target_properties(TravelTests PROPERTIES
    MACOSX_BUNDLE FALSE
    WIN32_EXECUTABLE FALSE
)

target_link_libraries(TravelTests
    PRIVATE Qt6::Test
)

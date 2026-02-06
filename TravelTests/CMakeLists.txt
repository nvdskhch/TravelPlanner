cmake_minimum_required(VERSION 3.5)

project(TravelTests LANGUAGES CXX)

enable_testing()

find_package(Qt6 REQUIRED COMPONENTS Core Test Network Widgets)

add_executable(TravelTests
    tst_traveltests.cpp
    ../trip.cpp
    ../tripmanager.cpp
    ../priceservice.cpp
)

target_include_directories(TravelTests PRIVATE ../)

target_link_libraries(TravelTests PRIVATE Qt6::Core Qt6::Test Qt6::Network Qt6::Widgets)

cmake_minimum_required(VERSION 3.5)

project(Travel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Network PrintSupport)

add_executable(Travel
    main.cpp
    mainwindow.cpp
    mainwindow.h
    # mainwindow.ui - УДАЛИ ЭТУ СТРОКУ, если она там есть (мы же удалили файл)
    trip.cpp
    trip.h
    tripmanager.cpp
    tripmanager.h
    priceservice.cpp
    priceservice.h
)

target_link_libraries(Travel PRIVATE Qt6::Widgets Qt6::Network Qt6::PrintSupport)

enable_testing()
add_subdirectory(TravelTests)

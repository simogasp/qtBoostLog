cmake_minimum_required(VERSION 3.8)

project(Logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS log thread date_time system filesystem)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

add_executable(logger
        eventlog.cpp
        eventlog.h
        heater.cpp
        heater.h
        logExample.cpp
        logExample.h
        logExample.ui
        main.cpp
        temperaturelog.cpp
        temperaturelog.h)

target_include_directories(logger PUBLIC )
target_link_libraries(logger  PUBLIC Boost::boost Boost::log Boost::thread Boost::date_time Boost::system Boost::filesystem Qt5::Widgets Qt5::Core Qt5::Gui)
target_compile_definitions(logger PUBLIC BOOST_LOG_DYN_LINK)
cmake_minimum_required(VERSION 3.3)

project(ca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost REQUIRED COMPONENTS program_options system)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/Include
    ${Boost_INCLUDE_DIRS}
)

add_executable(ca
    src/main.cpp

    Include/cache.h
    Include/server.h
    Include/session.h
    Include/args.h

    src/Network/Cache/cache.cpp
    src/Network/Server/server.cpp
    src/Network/Session/session.cpp
    src/CLI/args.cpp
)

target_link_libraries(ca
    PRIVATE Boost::program_options Boost::system
)

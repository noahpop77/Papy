cmake_minimum_required(VERSION 3.10)
project(papy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -Wall")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -g -O1")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

add_executable(papy
    src/apiClient.cpp
    src/cliHelper.cpp
    src/main.cpp
    src/mapping.cpp
    src/matchBuilder.cpp
    src/millisecondClock.cpp
    src/myRandom.cpp
    src/oceanBuilder.cpp
    src/threadWorks.cpp
)

target_include_directories(papy PUBLIC
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/src/dependencies"
)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)

target_link_libraries(papy PRIVATE 
    ZLIB::ZLIB
    OpenSSL::Crypto
    OpenSSL::SSL
)



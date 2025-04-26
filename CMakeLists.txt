cmake_minimum_required(VERSION 3.18)

project(Graffiti_Engine
    VERSION 0.1.0
    DESCRIPTION "SDU ZhengQi"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(

deps/spdlog/include

)
add_subdirectory(Graffiti)
add_subdirectory(Sandbox)
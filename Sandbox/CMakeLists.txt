
include_directories(
${CMAKE_SOURCE_DIR}/Graffiti/src
)
link_directories(
${CMAKE_SOURCE_DIR}/build/Graffiti/Release
)
add_executable(Sandbox

src/Sandbox.cpp
)
target_link_libraries(Sandbox
Graffiti.lib
)


if (MSVC)
    target_compile_options(Sandbox PRIVATE /utf-8)
endif()
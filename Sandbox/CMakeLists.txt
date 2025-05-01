
include_directories(
${CMAKE_SOURCE_DIR}/Graffiti
)
link_directories(
${CMAKE_SOURCE_DIR}/build/Graffiti/Release
)




add_executable(Sandbox
Sandbox.cpp

)
target_compile_definitions(Sandbox PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(Sandbox
Graffiti.lib
glfw3.lib
imgui.lib
glad.lib
)


if (MSVC)
    target_compile_options(Sandbox PRIVATE /utf-8)
endif()
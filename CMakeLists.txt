cmake_minimum_required(VERSION 3.18)

project(Graffiti_Engine
    VERSION 0.1.0
    DESCRIPTION "SDU ZhengQi"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
deps/GLFW/include
deps/spdlog/include
deps/imgui
deps/glm/include
deps/glad/include
)

link_directories(
deps/GLFW/lib
)
add_subdirectory(Graffiti)
add_subdirectory(Sandbox)

# -------- ImGui --------
file(GLOB IMGUI_SRC
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/deps/imgui/imgui_demo.cpp
${CMAKE_SOURCE_DIR}/deps/imgui/examples/imgui_impl_glfw.cpp
${CMAKE_SOURCE_DIR}/deps/imgui/examples/imgui_impl_opengl3.cpp
#${CMAKE_SOURCE_DIR}/deps/imgui/examples/imgui_impl_vulkan.cpp

)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC deps/imgui deps/imgui/examples)
target_compile_definitions(imgui PRIVATE  IMGUI_IMPL_OPENGL_LOADER_GLAD)


add_library(glad STATIC
   ${CMAKE_SOURCE_DIR}/deps/glad/src/glad.c
)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)
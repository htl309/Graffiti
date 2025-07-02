cmake_minimum_required(VERSION 3.18)

project(Graffiti_Engine
    VERSION 0.1.0
    DESCRIPTION "SDU ZhengQi"
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})

include_directories(
deps/
deps/GLFW/include
deps/spdlog/include
deps/glm/include
deps/glad/include
deps/tinygltf
)

link_directories(
deps/GLFW/lib
deps/glad/src
)


# -------- glad --------
add_library(glad STATIC
   ${CMAKE_SOURCE_DIR}/deps/glad/src/glad.c
)
set_target_properties(glad PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(Graffiti)

set(Example_Name_00  "00_Hello_Triangle")
set(Example_Name_01 "01_Bunny")
set(Example_Name_02 "02_GeometryShader")
set(Example_Name_03 "03_Texture")
set(Example_Name_04 "04_Material")
set(Example_Name_05 "05_MeshShader")
set(Example_Name_06 "06_PBR")

add_subdirectory(Examples/00_Hello_Triangle) 
add_subdirectory(Examples/01_Bunny) 
add_subdirectory(Examples/02_GeometryShader) 
add_subdirectory(Examples/03_Texture) 
add_subdirectory(Examples/04_Material) 
add_subdirectory(Examples/05_MeshShader) 
add_subdirectory(Examples/06_PBR) 
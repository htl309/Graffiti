
include_directories(
${CMAKE_SOURCE_DIR}/Graffiti
${CMAKE_SOURCE_DIR}/deps/meshoptimizer/include
)
link_directories(
${CMAKE_SOURCE_DIR}/build/Graffiti/Release
${CMAKE_SOURCE_DIR}/deps/meshoptimizer/lib
)




add_executable(${Example_Name_05}
main.cpp
MeshShader.h
MeshShader.cpp
MeshShaderModel.h
MeshShaderModel.cpp
)
target_link_libraries(${Example_Name_05}
Graffiti.lib
glfw3.lib
glad.lib
Vulkan::Vulkan
meshoptimizer.lib
)

set_target_properties(${Example_Name_05} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
if (MSVC)
    target_compile_options(${Example_Name_05} PRIVATE /utf-8)
endif()
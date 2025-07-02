
include_directories(
${CMAKE_SOURCE_DIR}/Graffiti
)
link_directories(
${CMAKE_SOURCE_DIR}/build/Graffiti/Release
)

add_executable(${Example_Name_02}
main.cpp

)
target_link_libraries(${Example_Name_02}
Graffiti.lib
glfw3.lib
glad.lib
Vulkan::Vulkan
)

set_target_properties(${Example_Name_02} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
if (MSVC)
    target_compile_options(${Example_Name_02} PRIVATE /utf-8)
endif()
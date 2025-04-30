
# --------Graffiti --------
# 设置工程根目录
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(
${ROOT_DIR}
)
# 递归收集所有 .cpp 和 .h 文件
file(GLOB_RECURSE ALL_SRC_FILES
    "${ROOT_DIR}/*.cpp"
    "${ROOT_DIR}/*.c"
    "${ROOT_DIR}/*.h"
    "${ROOT_DIR}/*.hpp"
)




# 添加可执行文件
add_library(Graffiti
 ${ALL_SRC_FILES}
)

set_target_properties(Graffiti PROPERTIES
    LINKER_LANGUAGE CXX  # 即使含有 .c 文件也指定为 C++
)

target_link_libraries(Graffiti 
glfw3.lib
)
target_compile_definitions(Graffiti PRIVATE GF_PLATFORM_WINDOWS )
target_precompile_headers(Graffiti PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/gfpch.h")


if (MSVC)
    target_compile_options(Graffiti PRIVATE /utf-8)
endif()

# 设置 source_group 让 VS 显示目录结构
foreach(FILE_PATH IN LISTS ALL_SRC_FILES)
    file(RELATIVE_PATH REL_PATH "${ROOT_DIR}" "${FILE_PATH}")
    get_filename_component(DIR_PATH "${REL_PATH}" PATH)
    string(REPLACE "/" "\\" GROUP_PATH "${DIR_PATH}")
    source_group("${GROUP_PATH}" FILES "${FILE_PATH}")
endforeach()


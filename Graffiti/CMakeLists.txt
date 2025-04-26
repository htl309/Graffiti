# Graffiti/CMakeLists.txt

file(GLOB_RECURSE GRAFFITI_HEADERS CONFIGURE_DEPENDS src/*.h)
file(GLOB_RECURSE GRAFFITI_SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(Graffiti  SHARED ${GRAFFITI_HEADERS} ${GRAFFITI_SOURCES})

target_include_directories(Graffiti PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# 把src下的文件放进VS的 "src" 分组
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "src" FILES ${GRAFFITI_HEADERS} ${GRAFFITI_SOURCES})

# 宏定义，编译选项
target_compile_definitions(Graffiti PRIVATE
    GF_PLATFORM_WINDOWS
    GF_BUILD_DLL
)

if (MSVC)
    target_compile_options(Graffiti PRIVATE /utf-8)
endif()

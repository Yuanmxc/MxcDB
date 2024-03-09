cmake_minimum_required(VERSION 3.10)
project(MxcDB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 添加src目录下的文件到变量SRC_FILES
file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cc")

# 指定头文件目录
include_directories(${PROJECT_SOURCE_DIR}/src)

find_package(Snappy REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 链接外部库
target_link_libraries(${PROJECT_NAME} 
    Snappy::snappy
    spdlog::spdlog
    GTest::GTest 
    benchmark::benchmark
)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

target_link_libraries(${PROJECT_NAME} crc32 skiplist)
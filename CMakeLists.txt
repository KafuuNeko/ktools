cmake_minimum_required(VERSION 3.9)
project(ktools)

include_directories(src third_party)

#设置使用C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#确保 Win32 环境下存在 WIN32 定义
if (WIN32)
    add_definitions(-DWIN32) 
endif ()

#必须是32位的 -fexec-charset=GBK
set(CMAKE_C_FLAGS "-m32")
set(CMAKE_CXX_FLAGS "-m32")

#强制静态链接并取消输出函数@后缀
set(CMAKE_SHARED_LINKER_FLAGS "-static -Wl,--kill-at,--enable-stdcall-fixup")

#set(CMAKE_CXX_FLAGS "-pthread -ldl ${CMAKE_CXX_FLAGS}")

add_subdirectory(third_party)

set(PROJECT_DIR src)
file(GLOB_RECURSE SRC_FILES ${PROJECT_DIR}/*.cc)

message(${SRC_FILES})

add_library(ktools SHARED ${SRC_FILES})
target_link_libraries(ktools lepton crc aes)

add_executable(test tests/test.cc)
add_dependencies(test ktools)
target_link_libraries(test ktools)

set_target_properties(ktools PROPERTIES PREFIX "")
set_target_properties(ktools PROPERTIES SUFFIX ".dll")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/output/bin)




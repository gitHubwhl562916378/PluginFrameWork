cmake_minimum_required(VERSION 2.8)

set(OpenCV_DIR /usr/local/opencv-3.4.5/share/OpenCV)
project(cpptest)
find_package(OpenCV REQUIRED)
add_compile_options(-std=c++11 -fpermissive)
set(CMAKE_BUILD_TYPE "Debug") //不加默认release 不能调试
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

FILE(GLOB SRC_FILES "./*.cpp")
FILE(GLOB HEAD_FILES "./*.h")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} pthread)
target_link_libraries(${PROJECT_NAME} dl)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../build)
set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic") 
set(OpenCV_DIR /usr/local/opencv-3.4.5/share/OpenCV)
project(AiPluginTest)
find_package(OpenCV REQUIRED)
add_compile_options(-std=c++11 -fpermissive)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

FILE(GLOB SRC_FILES "./*.cpp")
FILE(GLOB HEAD_FILES "./*.h")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES} ${HEAD_FILES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} pthread)
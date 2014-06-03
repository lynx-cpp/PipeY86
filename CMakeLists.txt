cmake_minimum_required(VERSION 2.8.11)
project(pipey86)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Declarative REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Declarative_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})

set(PIPELINE_SRCS y86pipeline.cpp instruction.cpp instructionprivate.cpp)

add_executable(pipecmd ${PIPELINE_SRCS} pipecmd.cpp)
add_executable(pipey86 ${PIPELINE_SRCS} main.cpp)
qt5_use_modules(pipey86 Widgets Declarative)
#target_link_libraries(pipey86 ${Qt5Declarative_LIBRARIES} ${Qt5Widgets_LIBRARIES})
#add_executable(pipecmd y86pipeline.cpp instruction.cpp instructionprivate.cpp pipecmd.cpp)
#add_executable(pipey86 y86pipeline.cpp instruction.cpp instructionprivate.cpp )

#install(TARGETS pipey86 RUNTIME DESTINATION bin)

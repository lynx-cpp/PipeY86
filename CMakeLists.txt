cmake_minimum_required(VERSION 2.8.0)
project(pipey86)
#find_package(Qt5Widgets REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Core REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} ${Qt5Qml_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${Qt5Gui_INCLUDE_DIRS} ${Qt5Qml_INCLUDE_DIRS} ${Qt5Quick_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS} ${Qt5Qml_DEFINITIONS})

set(INSTRUCTION_SRCS instruction/instructionirmovl.cpp instruction/instructionmrmovl.cpp instruction/instructionop.cpp instruction/instructionrmmovl.cpp instruction/instructionrrmovl.cpp instruction/instructioncall.cpp instruction/instructionjump.cpp instruction/instructionret.cpp instruction/instructionpush.cpp instruction/instructionpop.cpp)
set(PIPELINE_SRCS y86pipeline.cpp instruction.cpp instructionprivate.cpp ${INSTRUCTION_SRCS})
set(S2O_SRCS s2o/assembler.cpp)

add_executable(pipecmd ${PIPELINE_SRCS} pipecmd.cpp)
add_executable(s2o ${S2O_SRCS} s2o/main.cpp)

QT5_ADD_RESOURCES (RESOURCES resources.qrc)
add_executable(pipey86 ${PIPELINE_SRCS} main.cpp ${RESOURCES} pipelineloader.cpp ${S2O_SRCS})
qt5_use_modules(pipey86 Widgets Qml Gui Quick)
qt5_use_modules(pipecmd Core)
#target_link_libraries(pipey86 ${Qt5Declarative_LIBRARIES} ${Qt5Widgets_LIBRARIES})
#add_executable(pipecmd y86pipeline.cpp instruction.cpp instructionprivate.cpp pipecmd.cpp)
#add_executable(pipey86 y86pipeline.cpp instruction.cpp instructionprivate.cpp )

install(TARGETS pipey86 RUNTIME DESTINATION bin )

#add_subdirectory(qml/module)
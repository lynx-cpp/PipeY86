project(pipey86)

add_executable(pipey86 y86pipeline.cpp instruction.cpp instructionprivate.cpp main.cpp)

install(TARGETS pipey86 RUNTIME DESTINATION bin)

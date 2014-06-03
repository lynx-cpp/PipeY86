project(pipey86)

add_executable(pipecmd y86pipeline.cpp instruction.cpp instructionprivate.cpp pipecmd.cpp)

#install(TARGETS pipey86 RUNTIME DESTINATION bin)

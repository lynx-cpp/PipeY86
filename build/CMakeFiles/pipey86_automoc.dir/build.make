# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jedi/OS_Lab/Lab3/PipeY86

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jedi/OS_Lab/Lab3/PipeY86/build

# Utility rule file for pipey86_automoc.

# Include the progress variables for this target.
include CMakeFiles/pipey86_automoc.dir/progress.make

CMakeFiles/pipey86_automoc:
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jedi/OS_Lab/Lab3/PipeY86/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Automoc for target pipey86"
	/usr/bin/cmake -E cmake_automoc /home/jedi/OS_Lab/Lab3/PipeY86/build/CMakeFiles/pipey86_automoc.dir/

pipey86_automoc: CMakeFiles/pipey86_automoc
pipey86_automoc: CMakeFiles/pipey86_automoc.dir/build.make
.PHONY : pipey86_automoc

# Rule to build all files generated by this target.
CMakeFiles/pipey86_automoc.dir/build: pipey86_automoc
.PHONY : CMakeFiles/pipey86_automoc.dir/build

CMakeFiles/pipey86_automoc.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pipey86_automoc.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pipey86_automoc.dir/clean

CMakeFiles/pipey86_automoc.dir/depend:
	cd /home/jedi/OS_Lab/Lab3/PipeY86/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jedi/OS_Lab/Lab3/PipeY86 /home/jedi/OS_Lab/Lab3/PipeY86 /home/jedi/OS_Lab/Lab3/PipeY86/build /home/jedi/OS_Lab/Lab3/PipeY86/build /home/jedi/OS_Lab/Lab3/PipeY86/build/CMakeFiles/pipey86_automoc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pipey86_automoc.dir/depend

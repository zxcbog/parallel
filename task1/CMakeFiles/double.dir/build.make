# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/m.unzhakov/parallel/task1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/m.unzhakov/parallel/task1

# Include any dependencies generated for this target.
include CMakeFiles/double.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/double.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/double.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/double.dir/flags.make

CMakeFiles/double.dir/task_double.cpp.o: CMakeFiles/double.dir/flags.make
CMakeFiles/double.dir/task_double.cpp.o: task_double.cpp
CMakeFiles/double.dir/task_double.cpp.o: CMakeFiles/double.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/m.unzhakov/parallel/task1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/double.dir/task_double.cpp.o"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/double.dir/task_double.cpp.o -MF CMakeFiles/double.dir/task_double.cpp.o.d -o CMakeFiles/double.dir/task_double.cpp.o -c /home/m.unzhakov/parallel/task1/task_double.cpp

CMakeFiles/double.dir/task_double.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/double.dir/task_double.cpp.i"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/m.unzhakov/parallel/task1/task_double.cpp > CMakeFiles/double.dir/task_double.cpp.i

CMakeFiles/double.dir/task_double.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/double.dir/task_double.cpp.s"
	/opt/nvidia/hpc_sdk/Linux_x86_64/23.11/compilers/bin/nvc++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/m.unzhakov/parallel/task1/task_double.cpp -o CMakeFiles/double.dir/task_double.cpp.s

# Object files for target double
double_OBJECTS = \
"CMakeFiles/double.dir/task_double.cpp.o"

# External object files for target double
double_EXTERNAL_OBJECTS =

double: CMakeFiles/double.dir/task_double.cpp.o
double: CMakeFiles/double.dir/build.make
double: CMakeFiles/double.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/m.unzhakov/parallel/task1/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable double"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/double.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/double.dir/build: double
.PHONY : CMakeFiles/double.dir/build

CMakeFiles/double.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/double.dir/cmake_clean.cmake
.PHONY : CMakeFiles/double.dir/clean

CMakeFiles/double.dir/depend:
	cd /home/m.unzhakov/parallel/task1 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/m.unzhakov/parallel/task1 /home/m.unzhakov/parallel/task1 /home/m.unzhakov/parallel/task1 /home/m.unzhakov/parallel/task1 /home/m.unzhakov/parallel/task1/CMakeFiles/double.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/double.dir/depend


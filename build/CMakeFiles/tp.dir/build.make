# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b

# Include any dependencies generated for this target.
include CMakeFiles/tp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tp.dir/flags.make

CMakeFiles/tp.dir/sprite1.cpp.o: CMakeFiles/tp.dir/flags.make
CMakeFiles/tp.dir/sprite1.cpp.o: ../sprite1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tp.dir/sprite1.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tp.dir/sprite1.cpp.o -c /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/sprite1.cpp

CMakeFiles/tp.dir/sprite1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tp.dir/sprite1.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/sprite1.cpp > CMakeFiles/tp.dir/sprite1.cpp.i

CMakeFiles/tp.dir/sprite1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tp.dir/sprite1.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/sprite1.cpp -o CMakeFiles/tp.dir/sprite1.cpp.s

# Object files for target tp
tp_OBJECTS = \
"CMakeFiles/tp.dir/sprite1.cpp.o"

# External object files for target tp
tp_EXTERNAL_OBJECTS =

tp: CMakeFiles/tp.dir/sprite1.cpp.o
tp: CMakeFiles/tp.dir/build.make
tp: CMakeFiles/tp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tp.dir/build: tp

.PHONY : CMakeFiles/tp.dir/build

CMakeFiles/tp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tp.dir/clean

CMakeFiles/tp.dir/depend:
	cd /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b /home/jaz/Escritorio/jaz/fiuba/taller/tp4/worms/b/CMakeFiles/tp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tp.dir/depend


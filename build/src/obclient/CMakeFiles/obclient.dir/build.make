# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/xiaoyu/miniOB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/xiaoyu/miniOB/build

# Include any dependencies generated for this target.
include src/obclient/CMakeFiles/obclient.dir/depend.make

# Include the progress variables for this target.
include src/obclient/CMakeFiles/obclient.dir/progress.make

# Include the compile flags for this target's objects.
include src/obclient/CMakeFiles/obclient.dir/flags.make

src/obclient/CMakeFiles/obclient.dir/client.cpp.o: src/obclient/CMakeFiles/obclient.dir/flags.make
src/obclient/CMakeFiles/obclient.dir/client.cpp.o: ../src/obclient/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/xiaoyu/miniOB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/obclient/CMakeFiles/obclient.dir/client.cpp.o"
	cd /home/xiaoyu/miniOB/build/src/obclient && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/obclient.dir/client.cpp.o -c /home/xiaoyu/miniOB/src/obclient/client.cpp

src/obclient/CMakeFiles/obclient.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/obclient.dir/client.cpp.i"
	cd /home/xiaoyu/miniOB/build/src/obclient && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/xiaoyu/miniOB/src/obclient/client.cpp > CMakeFiles/obclient.dir/client.cpp.i

src/obclient/CMakeFiles/obclient.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/obclient.dir/client.cpp.s"
	cd /home/xiaoyu/miniOB/build/src/obclient && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/xiaoyu/miniOB/src/obclient/client.cpp -o CMakeFiles/obclient.dir/client.cpp.s

# Object files for target obclient
obclient_OBJECTS = \
"CMakeFiles/obclient.dir/client.cpp.o"

# External object files for target obclient
obclient_EXTERNAL_OBJECTS =

bin/obclient: src/obclient/CMakeFiles/obclient.dir/client.cpp.o
bin/obclient: src/obclient/CMakeFiles/obclient.dir/build.make
bin/obclient: lib/libcommon.so.1.0.0
bin/obclient: src/obclient/CMakeFiles/obclient.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/xiaoyu/miniOB/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../bin/obclient"
	cd /home/xiaoyu/miniOB/build/src/obclient && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/obclient.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/obclient/CMakeFiles/obclient.dir/build: bin/obclient

.PHONY : src/obclient/CMakeFiles/obclient.dir/build

src/obclient/CMakeFiles/obclient.dir/clean:
	cd /home/xiaoyu/miniOB/build/src/obclient && $(CMAKE_COMMAND) -P CMakeFiles/obclient.dir/cmake_clean.cmake
.PHONY : src/obclient/CMakeFiles/obclient.dir/clean

src/obclient/CMakeFiles/obclient.dir/depend:
	cd /home/xiaoyu/miniOB/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/xiaoyu/miniOB /home/xiaoyu/miniOB/src/obclient /home/xiaoyu/miniOB/build /home/xiaoyu/miniOB/build/src/obclient /home/xiaoyu/miniOB/build/src/obclient/CMakeFiles/obclient.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/obclient/CMakeFiles/obclient.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Volumes/BOOTCAMP/csc209/zhanzhih/lab9

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/lab9.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab9.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab9.dir/flags.make

CMakeFiles/lab9.dir/time_reads.c.o: CMakeFiles/lab9.dir/flags.make
CMakeFiles/lab9.dir/time_reads.c.o: ../time_reads.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/lab9.dir/time_reads.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lab9.dir/time_reads.c.o   -c /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/time_reads.c

CMakeFiles/lab9.dir/time_reads.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab9.dir/time_reads.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/time_reads.c > CMakeFiles/lab9.dir/time_reads.c.i

CMakeFiles/lab9.dir/time_reads.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab9.dir/time_reads.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/time_reads.c -o CMakeFiles/lab9.dir/time_reads.c.s

CMakeFiles/lab9.dir/write_test_file.c.o: CMakeFiles/lab9.dir/flags.make
CMakeFiles/lab9.dir/write_test_file.c.o: ../write_test_file.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/lab9.dir/write_test_file.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/lab9.dir/write_test_file.c.o   -c /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/write_test_file.c

CMakeFiles/lab9.dir/write_test_file.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/lab9.dir/write_test_file.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/write_test_file.c > CMakeFiles/lab9.dir/write_test_file.c.i

CMakeFiles/lab9.dir/write_test_file.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/lab9.dir/write_test_file.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/write_test_file.c -o CMakeFiles/lab9.dir/write_test_file.c.s

# Object files for target lab9
lab9_OBJECTS = \
"CMakeFiles/lab9.dir/time_reads.c.o" \
"CMakeFiles/lab9.dir/write_test_file.c.o"

# External object files for target lab9
lab9_EXTERNAL_OBJECTS =

lab9: CMakeFiles/lab9.dir/time_reads.c.o
lab9: CMakeFiles/lab9.dir/write_test_file.c.o
lab9: CMakeFiles/lab9.dir/build.make
lab9: CMakeFiles/lab9.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable lab9"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab9.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab9.dir/build: lab9

.PHONY : CMakeFiles/lab9.dir/build

CMakeFiles/lab9.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab9.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab9.dir/clean

CMakeFiles/lab9.dir/depend:
	cd /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Volumes/BOOTCAMP/csc209/zhanzhih/lab9 /Volumes/BOOTCAMP/csc209/zhanzhih/lab9 /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug /Volumes/BOOTCAMP/csc209/zhanzhih/lab9/cmake-build-debug/CMakeFiles/lab9.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab9.dir/depend
